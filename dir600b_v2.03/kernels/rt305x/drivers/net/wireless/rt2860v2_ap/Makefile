EXTRA_CFLAGS = -Idrivers/net/wireless/rt2860v2/include

obj-$(CONFIG_RT2860V2_AP) += rt2860v2_ap.o

rt2860v2_ap-objs += ../rt2860v2/common/md5.o
rt2860v2_ap-objs += ../rt2860v2/common/mlme.o
rt2860v2_ap-objs += ../rt2860v2/common/rtmp_wep.o
rt2860v2_ap-objs += ../rt2860v2/common/action.o
rt2860v2_ap-objs += ../rt2860v2/common/ba_action.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_data.o
rt2860v2_ap-objs += ../rt2860v2/common/rtmp_init.o
rt2860v2_ap-objs += ../rt2860v2/common/rtmp_tkip.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_sync.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_sanity.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_info.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_wpa.o
rt2860v2_ap-objs += ../rt2860v2/common/2860_rtmp_init.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_data_2860.o
rt2860v2_ap-objs += ../rt2860v2/common/dfs.o
rt2860v2_ap-objs += ../rt2860v2/common/dfs_mcu.o


rt2860v2_ap-objs += ../rt2860v2/ap/ap.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_assoc.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_auth.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_authrsp.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_connect.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_mlme.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_sanity.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_sync.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_wpa.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_data.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_uapsd.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_autoChSel.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_mbss.o
rt2860v2_ap-objs += ../rt2860v2/ap/ap_qload.o


rt2860v2_ap-objs += ../rt2860v2/os/linux/rt_linux.o
rt2860v2_ap-objs += ../rt2860v2/os/linux/rt_profile.o
rt2860v2_ap-objs += ../rt2860v2/os/linux/rt_main_dev.o
rt2860v2_ap-objs += ../rt2860v2/os/linux/ap_ioctl.o
rt2860v2_ap-objs += ../rt2860v2/os/linux/rt_2860_rbus.o
rt2860v2_ap-objs += ../rt2860v2/os/linux/rt_eeprom.o
rt2860v2_ap-objs += ../rt2860v2/os/linux/2860_main_dev.o
rt2860v2_ap-objs += ../rt2860v2/os/linux/rt_ate.o


ifeq ($(CONFIG_RT2860V2_AP_LED),y)
rt2860v2_ap-objs += ../rt2860v2/common/led.o
endif

ifeq ($(CONFIG_RT2860V2_AP_WSC),y)
rt2860v2_ap-objs += ../rt2860v2/common/wsc.o 
rt2860v2_ap-objs += ../rt2860v2/common/wsc_tlv.o
rt2860v2_ap-objs += ../rt2860v2/common/sha2.o
rt2860v2_ap-objs += ../rt2860v2/common/hmac.o
rt2860v2_ap-objs += ../rt2860v2/common/dh_key.o
rt2860v2_ap-objs += ../rt2860v2/common/evp_enc.o
endif


ifeq ($(CONFIG_RT2860V2_AP_WDS),y)
rt2860v2_ap-objs += ../rt2860v2/ap/ap_wds.o
endif

ifeq ($(CONFIG_RT2860V2_AP_MBSS),y)
rt2860v2_ap-objs += ../rt2860v2/ap/ap_mbss.o
endif

ifeq ($(CONFIG_RT2860V2_AP_APCLI),y)
rt2860v2_ap-objs += ../rt2860v2/ap/ap_apcli.o
rt2860v2_ap-objs += ../rt2860v2/ap/apcli_assoc.o
rt2860v2_ap-objs += ../rt2860v2/ap/apcli_auth.o
rt2860v2_ap-objs += ../rt2860v2/ap/apcli_ctrl.o
rt2860v2_ap-objs += ../rt2860v2/ap/apcli_sync.o
rt2860v2_ap-objs += ../rt2860v2/common/mlme_ex.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_mat.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_mat_iparp.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_mat_pppoe.o
rt2860v2_ap-objs += ../rt2860v2/common/cmm_mat_ipv6.o
endif

ifeq ($(CONFIG_RT2860V2_AP_IGMP_SNOOP),y)
rt2860v2_ap-objs += ../rt2860v2/common/igmp_snoop.o
endif

ifeq ($(CONFIG_RT2860V2_AP_NETIF_BLOCK),y)
rt2860v2_ap-objs += ../rt2860v2/common/netif_block.o
endif

ifeq ($(CONFIG_RT2860V2_AP_DLS),y)
rt2860v2_ap-objs += ../rt2860v2/ap/ap_dls.o
endif

ifeq ($(CONFIG_RT2860V2_AP_IDS),y)
rt2860v2_ap-objs += ../rt2860v2/ap/ap_ids.o
endif

ifeq ($(CONFIG_RT2860V2_AP_MESH),y)
rt2860v2_ap-objs += ../rt2860v2/common/mesh_bmpkt.o 
rt2860v2_ap-objs += ../rt2860v2/common/mesh_ctrl.o 
rt2860v2_ap-objs += ../rt2860v2/common/mesh_link_mng.o 
rt2860v2_ap-objs += ../rt2860v2/common/mesh_sanity.o 
rt2860v2_ap-objs += ../rt2860v2/common/mesh_tlv.o 
rt2860v2_ap-objs += ../rt2860v2/common/mesh.o 
rt2860v2_ap-objs += ../rt2860v2/common/mesh_forwarding.o 
rt2860v2_ap-objs += ../rt2860v2/common/mesh_path_mng.o 
rt2860v2_ap-objs += ../rt2860v2/common/mlme_ex.o 
endif


###################
#  CFLAGS
##################
EXTRA_CFLAGS += -DAGGREGATION_SUPPORT -DPIGGYBACK_SUPPORT -DWMM_SUPPORT  -DLINUX \
               -Wall -Wstrict-prototypes -Wno-trigraphs

EXTRA_CFLAGS += -DCONFIG_AP_SUPPORT -DUAPSD_AP_SUPPORT -DDBG -DRT2860 # -DDFS_DEBUG -DIAPP_SUPPORT
EXTRA_CFLAGS += -DRALINK_ATE -DRALINK_28xx_QA -DCONFIG_RT2880_ATE_CMD_NEW

ifeq ($(CONFIG_RT2860V2_AP_DLS),y)
EXTRA_CFLAGS += -DQOS_DLS_SUPPORT
endif

ifeq ($(CONFIG_RT2860V2_AP_IDS),y)
EXTRA_CFLAGS += -DIDS_SUPPORT
endif

ifeq ($(CONFIG_RT2860V2_AP_DFS),y)
EXTRA_CFLAGS += -DDFS_SUPPORT
endif

ifeq ($(CONFIG_RT2860V2_AP_CARRIER),y)
EXTRA_CFLAGS += -DCARRIER_DETECTION_SUPPORT
endif

ifeq ($(CONFIG_RT2860V2_AP_LED),y)
EXTRA_CFLAGS += -DAP_LED
endif


ifeq ($(CONFIG_RT2860V2_AP_WSC),y)
EXTRA_CFLAGS += -DWSC_AP_SUPPORT
endif

ifeq ($(CONFIG_RT2860V2_AP_LLTD),y)
EXTRA_CFLAGS += -DLLTD_SUPPORT
endif

ifeq ($(CONFIG_RT2860V2_AP_WDS),y)
EXTRA_CFLAGS += -DWDS_SUPPORT
endif

ifeq ($(CONFIG_RT2860V2_AP_MBSS),y)
EXTRA_CFLAGS += -DMBSS_SUPPORT
endif

ifeq ($(CONFIG_RT2860V2_AP_APCLI),y)
EXTRA_CFLAGS += -DAPCLI_SUPPORT
EXTRA_CFLAGS += -DMAT_SUPPORT
EXTRA_CFLAGS += -DMLME_EX

endif

ifeq ($(CONFIG_RT2860V2_AP_IGMP_SNOOP),y)
EXTRA_CFLAGS += -DIGMP_SNOOP_SUPPORT
endif

ifeq ($(CONFIG_RT2860V2_AP_NETIF_BLOCK),y)
EXTRA_CFLAGS += -DBLOCK_NET_IF
endif

ifeq ($(CONFIG_RT2860V2_AP_MESH),y)
EXTRA_CFLAGS   += -DMESH_SUPPORT
EXTRA_CFLAGS += -DMLME_EX
endif

