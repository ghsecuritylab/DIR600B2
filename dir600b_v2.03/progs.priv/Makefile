#
# Makefile for progs.priv
#

.EXPORT_ALL_VARIABLES:

-include arch.mk

all:
	@echo -e "\033[32mBuilding progs.priv ...\033[0m"

install:
	@echo -e "\033[32mInstalling progs.priv ...\033[0m"
	@for i in $(PROGS_PRIV_SUBDIRS); do make -C $$i install || exit $? ; done

clean:
	@echo -e "\033[32mCleaning progs.priv ...\033[0m"

.PHONY: all install clean
