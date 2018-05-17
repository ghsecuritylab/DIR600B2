#
# Board dependent Makefile for WRG-N23
#

MYNAME	:= WRG-N23
MKSQFS	:= ./tools/sqlzma/sqlzma-3.2-443-r2/mksquashfs
SEAMA	:= ./tools/seama/seama
PIMGS	:= ./tools/buildimg/packimgs
LZMA	:= ./tools/lzma/lzma
MYMAKE	:= $(Q)make V=$(V) DEBUG=$(DEBUG)
FWDEV	:= /dev/mtdblock/2
LANGDEV := /dev/mtdblock/5

ifdef PREDEFINE_RELIMAGE
RELIMAGE:=$(PREDEFINE_RELIMAGE)
else
BUILDNO	:=$(shell cat buildno)
RELIMAGE:=$(shell echo $(ELBOX_MODEL_NAME)_v$(ELBOX_FIRMWARE_VERSION)_$(BUILDNO))
endif

ifeq ($(strip $(ELBOX_USE_IPV6)),y)
KERNELCONFIG := kernel.aries.ipv6.config
else
KERNELCONFIG := kernel.aries.config
endif

#############################################################################
# This one will be make in fakeroot.
fakeroot_rootfs_image:
	@rm -f fakeroot.rootfs.img
	@./progs.board/template.aries/makedevnodes rootfs
	@./tools/sqlzma/sqlzma-3.2-443-r2/mksquashfs rootfs fakeroot.rootfs.img

.PHONY: rootfs_image

#############################################################################
# The real image files

$(ROOTFS_IMG): ./tools/sqlzma/sqlzma-3.2-443-r2/mksquashfs
	@echo -e "\033[32m$(MYNAME): building squashfs (LZMA)!\033[0m"
	$(Q)make clean_CVS
	$(Q)fakeroot make -f progs.board/template.aries/config.mk fakeroot_rootfs_image
	$(Q)mv fakeroot.rootfs.img $(ROOTFS_IMG)
	$(Q)chmod 664 $(ROOTFS_IMG)

$(KERNEL_IMG): ./tools/lzma/lzma $(KERNELDIR)/vmlinux
	@echo -e "\033[32m$(MYNAME): building kernel image (LZMA)...\033[0m"
	$(Q)rm -f vmlinux.bin $(KERNEL_IMG)
	$(Q)mipsel-linux-objcopy -O binary -R .note -R .comment -S $(KERNELDIR)/vmlinux vmlinux.bin
	$(Q)$(LZMA) -9 -f -S .lzma vmlinux.bin
	$(Q)mv vmlinux.bin.lzma $(KERNEL_IMG)

$(KERNELDIR)/vmlinux:
	$(MYMAKE) kernel

./tools/sqlzma/sqlzma-3.2-443-r2/mksquashfs:
	$(Q)make -C ./tools/sqlzma/sqlzma-3.2-443-r2

./tools/seama/seama:
	$(Q)make -C ./tools/seama

./tools/buildimg/packimgs:
	$(Q)make -C ./tools/buildimg

./tools/lzma/lzma:
	$(Q)make -C ./tools/lzma

##########################################################################

kernel_image:
	@echo -e "\033[32m$(MYNAME): creating kernel image\033[0m"
	$(Q)rm -f $(KERNEL_IMG)
	$(MYMAKE) $(KERNEL_IMG)

rootfs_image:
	@echo -e "\033[32m$(MYNAME): creating rootfs image ...\033[0m"
	$(Q)rm -f $(ROOTFS_IMG)
	$(MYMAKE) $(ROOTFS_IMG)

.PHONY: rootfs_image kernel_image

##########################################################################
#
#	Major targets: kernel, kernel_clean, release & tftpimage
#
##########################################################################

kernel_clean:
	@echo -e "\033[32m$(MYNAME): cleaning kernel ...\033[0m"
	$(Q)make -C kernel mrproper

kernel: kernel_clean
	@echo -e "\033[32m$(MYNAME) Building kernel ...\033[0m"
	$(Q)cp progs.board/$(KERNELCONFIG) kernel/.config
	$(Q)make -C kernel oldconfig
	$(Q)make -C kernel dep
	$(Q)make -C kernel

ifeq (buildno, $(wildcard buildno))
BUILDNO	:=$(shell cat buildno)

release: kernel_image rootfs_image ./tools/buildimg/packimgs ./tools/seama/seama
	@echo -e "\033[32m"; \
	echo "=====================================";	\
	echo "You are going to build release image.";	\
	echo "=====================================";	\
	echo -e "\033[32m$(MYNAME) make release image... \033[0m"
	$(Q)[ -d images ] || mkdir -p images
	@echo -e "\033[32m$(MYNAME) prepare image...\033[0m"
	$(Q)$(PIMGS) -o raw.img -i $(KERNEL_IMG) -i $(ROOTFS_IMG)
	$(Q)$(SEAMA) -i raw.img -m dev=$(FWDEV) -m type=firmware 
	$(Q)$(SEAMA) -s web.img -i raw.img.seama -m signature=$(ELBOX_SIGNATURE)
	$(Q)$(SEAMA) -d web.img
	$(Q)rm -f raw.img raw.img.seama
	$(Q)./tools/release.sh web.img $(RELIMAGE).bin
	$(Q)make sealpac_template
	$(Q)if [ -f sealpac.slt ]; then ./tools/release.sh sealpac.slt $(RELIMAGE).slt; fi

magic_release: kernel_image rootfs_image ./tools/buildimg/packimgs ./tools/seama/seama
	@echo -e "\033[32m"; \
	echo "===========================================";	\
	echo "You are going to build magic release image.";	\
	echo "===========================================";	\
	echo -e "\033[32m$(MYNAME) make magic release image... \033[0m"
	$(Q)[ -d images ] || mkdir -p images
	@echo -e "\033[32m$(MYNAME) prepare image...\033[0m"
	$(Q)$(PIMGS) -o raw.img -i $(KERNEL_IMG) -i $(ROOTFS_IMG)
	$(Q)$(SEAMA) -i raw.img -m dev=$(FWDEV) -m type=firmware 
	$(Q)$(SEAMA) -s web.img -i raw.img.seama -m signature=$(ELBOX_BOARD_NAME)_aLpHa
	$(Q)$(SEAMA) -d web.img
	$(Q)rm -f raw.img raw.img.seama
	$(Q)./tools/release.sh web.img $(RELIMAGE).magic.bin

tftpimage: kernel_image rootfs_image ./tools/buildimg/packimgs ./tools/seama/seama
	@echo -e "\033[32mThe tftpimage of $(MYNAME) is identical to the release image!\033[0m"
	$(Q)$(PIMGS) -o raw.img -i $(KERNEL_IMG) -i $(ROOTFS_IMG)
	$(Q)$(SEAMA) -i raw.img -m dev=$(FWDEV) -m type=firmware
	$(Q)rm -f raw.img; mv raw.img.seama raw.img
	$(Q)$(SEAMA) -d raw.img
	$(Q)./tools/tftpimage.sh $(TFTPIMG)

else
release tftpimage:
	@echo -e "\033[32m$(MYNAME): Can not build image, ROOTFS is not created yet !\033[0m"
endif

.PHONY: kernel release tftpimage kernel_clean magic_release
