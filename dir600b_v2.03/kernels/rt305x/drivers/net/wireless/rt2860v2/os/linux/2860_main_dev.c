/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    2870_main_dev.c

    Abstract:
    Create and register network interface.

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
*/

#include <rt_config.h>

#define FORTY_MHZ_INTOLERANT_INTERVAL	(60*1000) // 1 min

#ifdef MULTIPLE_CARD_SUPPORT
// record whether the card in the card list is used in the card file
extern UINT8  MC_CardUsed[];
#endif // MULTIPLE_CARD_SUPPORT //

#ifdef CONFIG_AP_SUPPORT
//static RALINK_TIMER_STRUCT     PeriodicTimer;
extern VOID APDetectOverlappingExec(
			IN PVOID SystemSpecific1,
			IN PVOID FunctionContext, 
			IN PVOID SystemSpecific2, 
			IN PVOID SystemSpecific3);
#endif // CONFIG_AP_SUPPORT //

extern INT __devinit rt28xx_probe(IN void *_dev_p, IN void *_dev_id_p,
									IN UINT argc, OUT PRTMP_ADAPTER *ppAd);
//extern INT rt28xx_ioctl(struct net_device *net_dev, struct ifreq *rq, int cmd);

static void rx_done_tasklet(unsigned long data);
static void mgmt_dma_done_tasklet(unsigned long data);
static void ac0_dma_done_tasklet(unsigned long data);
static void ac1_dma_done_tasklet(unsigned long data);
static void ac2_dma_done_tasklet(unsigned long data);
static void ac3_dma_done_tasklet(unsigned long data);
static void hcca_dma_done_tasklet(unsigned long data);
static void fifo_statistic_full_tasklet(unsigned long data);

#ifdef CONFIG_AP_SUPPORT
#ifdef DFS_SUPPORT
void pulse_radar_detect_tasklet(unsigned long data);
void width_radar_detect_tasklet(unsigned long data);
#endif // DFS_SUPPORT //
#ifdef CARRIER_DETECTION_SUPPORT
void carrier_sense_tasklet(unsigned long data);
#endif // CARRIER_DETECTION_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

/*---------------------------------------------------------------------*/
/* Symbol & Macro Definitions                                          */
/*---------------------------------------------------------------------*/
#define RT2860_INT_RX_DLY				(1<<0)		// bit 0	
#define RT2860_INT_TX_DLY				(1<<1)		// bit 1
#define RT2860_INT_RX_DONE				(1<<2)		// bit 2
#define RT2860_INT_AC0_DMA_DONE			(1<<3)		// bit 3
#define RT2860_INT_AC1_DMA_DONE			(1<<4)		// bit 4
#define RT2860_INT_AC2_DMA_DONE			(1<<5)		// bit 5
#define RT2860_INT_AC3_DMA_DONE			(1<<6)		// bit 6
#define RT2860_INT_HCCA_DMA_DONE		(1<<7)		// bit 7
#define RT2860_INT_MGMT_DONE			(1<<8)		// bit 8
#ifdef CONFIG_RALINK_RT3052
#define RT2860_INT_TONE_RADAR			(1<<20)		// bit 20
#endif

#define INT_RX			RT2860_INT_RX_DONE

#define INT_AC0_DLY		(RT2860_INT_AC0_DMA_DONE) //| RT2860_INT_TX_DLY)
#define INT_AC1_DLY		(RT2860_INT_AC1_DMA_DONE) //| RT2860_INT_TX_DLY)
#define INT_AC2_DLY		(RT2860_INT_AC2_DMA_DONE) //| RT2860_INT_TX_DLY)
#define INT_AC3_DLY		(RT2860_INT_AC3_DMA_DONE) //| RT2860_INT_TX_DLY)
#define INT_HCCA_DLY 	(RT2860_INT_HCCA_DMA_DONE) //| RT2860_INT_TX_DLY)
#define INT_MGMT_DLY	RT2860_INT_MGMT_DONE
#ifdef CONFIG_RALINK_RT3052
#define INT_TONE_RADAR	(RT2860_INT_TONE_RADAR)
#endif

#ifdef CONFIG_STA_SUPPORT
#if 0
extern	const struct iw_handler_def rt2860_iw_handler_def;
#if WIRELESS_EXT >= 12
// This function will be called when query /proc
struct iw_statistics *rt2860_get_wireless_stats(
    IN struct net_device *net_dev);
#endif
#endif
#endif // CONFIG_STA_SUPPORT //

/*---------------------------------------------------------------------*/
/* Prototypes of Functions Used                                        */
/*---------------------------------------------------------------------*/
/* function declarations */

#if defined(CONFIG_RT2860V2_AP_RBUS_PCI) || defined(CONFIG_RT2860V2_STA_RBUS_PCI)
static INT __devinit rt2860_init_one (struct pci_dev *pci_dev, const struct pci_device_id  *ent);
static VOID __devexit rt2860_remove_one(struct pci_dev *pci_dev);
static INT __devinit rt2860_probe(struct pci_dev *pci_dev, const struct pci_device_id  *ent);
#endif
//void kill_thread_task(PRTMP_ADAPTER pAd);
void init_thread_task(PRTMP_ADAPTER pAd);
static void __exit rt2860_cleanup_module(void);
static int __init rt2860_init_module(void);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
#ifdef CONFIG_PM
static int rt2860_suspend(struct pci_dev *pci_dev, pm_message_t state);
static int rt2860_resume(struct pci_dev *pci_dev);
#endif // CONFIG_PM //
#endif


// Built-in 2860
extern int rbus_module_init(VOID);
extern VOID rbus_module_exit(VOID);

#if !defined(CONFIG_RA_NAT_NONE)
/* bruce+
 */
extern int (*ra_sw_nat_hook_tx)(struct sk_buff *skb);
#endif


#if defined(CONFIG_RT2860V2_AP_RBUS_PCI) || defined(CONFIG_RT2860V2_STA_RBUS_PCI)

//
// Ralink PCI device table, include all supported chipsets
//
static struct pci_device_id rt2860_pci_tbl[] __devinitdata =
{
	{PCI_DEVICE(NIC_PCI_VENDOR_ID, NIC2860_PCI_DEVICE_ID)},		//RT28602.4G
	{PCI_DEVICE(NIC_PCI_VENDOR_ID, NIC2860_PCIe_DEVICE_ID)},
	{PCI_DEVICE(NIC_PCI_VENDOR_ID, NIC2760_PCI_DEVICE_ID)},
	{PCI_DEVICE(NIC_PCI_VENDOR_ID, NIC2790_PCIe_DEVICE_ID)},
    {0,}		// terminate list
};

MODULE_DEVICE_TABLE(pci, rt2860_pci_tbl);
#ifdef CONFIG_STA_SUPPORT
MODULE_LICENSE("GPL");
MODULE_VERSION(STA_DRIVER_VERSION);
#endif // CONFIG_STA_SUPPORT //


//
// Our PCI driver structure
//
static struct pci_driver rt2860_driver =
{
    name:       "rt2860",
    id_table:   rt2860_pci_tbl,
    probe:      rt2860_init_one,
#if LINUX_VERSION_CODE >= 0x20412
    remove:     __devexit_p(rt2860_remove_one),
#else
    remove:     __devexit(rt2860_remove_one),
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
#ifdef CONFIG_PM
	suspend:	rt2860_suspend,
	resume:		rt2860_resume,
#endif
#endif
};

#endif // #if defined(CONFIG_RT2860V2_AP_RBUS_PCI) || defined(CONFIG_RT2860V2_STA_RBUS_PCI)

extern int rt28xx_close(IN struct net_device *net_dev);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
#ifdef CONFIG_PM
extern int rt28xx_open(struct net_device *net_dev);

VOID RT2860RejectPendingPackets(
	IN	PRTMP_ADAPTER	pAd)
{
	// clear PS packets
	// clear TxSw packets
}

static int rt2860_suspend(
	struct pci_dev *pci_dev,
	pm_message_t state)
{
	struct net_device *net_dev = pci_get_drvdata(pci_dev);
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)NULL;
	INT32 retval;


	DBGPRINT(RT_DEBUG_TRACE, ("===> rt2860_suspend()\n"));

	if (net_dev == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("net_dev == NULL!\n"));
	}
	else
	{
		pAd = (PRTMP_ADAPTER)net_dev->priv;

		// stop interface
		netif_carrier_off(net_dev);
		netif_stop_queue(net_dev);

		// mark device as removed from system and therefore no longer available
		netif_device_detach(net_dev);

		// mark halt flag
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);

		// take down the device
		rt28xx_close(net_dev);
	}

	// reference to http://vovo2000.com/type-lab/linux/kernel-api/linux-kernel-api.html
	// enable device to generate PME# when suspended
	// pci_choose_state(): Choose the power state of a PCI device to be suspended
	retval = pci_enable_wake(pci_dev, pci_choose_state(pci_dev, state), 1);
	// save the PCI configuration space of a device before suspending
	pci_save_state(pci_dev);
	// disable PCI device after use 
	pci_disable_device(pci_dev);

	retval = pci_set_power_state(pci_dev, pci_choose_state(pci_dev, state));

	DBGPRINT(RT_DEBUG_TRACE, ("<=== rt2860_suspend()\n"));
	return retval;
}

static int rt2860_resume(
	struct pci_dev *pci_dev)
{
	struct net_device *net_dev = pci_get_drvdata(pci_dev);
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)NULL;
	INT32 retval;


	// set the power state of a PCI device
	// PCI has 4 power states, DO (normal) ~ D3(less power)
	// in include/linux/pci.h, you can find that
	// #define PCI_D0          ((pci_power_t __force) 0)
	// #define PCI_D1          ((pci_power_t __force) 1)
	// #define PCI_D2          ((pci_power_t __force) 2)
	// #define PCI_D3hot       ((pci_power_t __force) 3)
	// #define PCI_D3cold      ((pci_power_t __force) 4)
	// #define PCI_UNKNOWN     ((pci_power_t __force) 5)
	// #define PCI_POWER_ERROR ((pci_power_t __force) -1)
	retval = pci_set_power_state(pci_dev, PCI_D0);

	// restore the saved state of a PCI device
	pci_restore_state(pci_dev);

	// initialize device before it's used by a driver
	if (pci_enable_device(pci_dev))
	{
		printk("pci enable fail!\n");
		return 0;
	}

	DBGPRINT(RT_DEBUG_TRACE, ("===> rt2860_resume()\n"));

	if (net_dev == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("net_dev == NULL!\n"));
	}
	else
		pAd = (PRTMP_ADAPTER)net_dev->priv;

		if (pAd != NULL)
	{
		// mark device as attached from system and restart if needed
		netif_device_attach(net_dev);

		if (rt28xx_open(net_dev) != 0)
		{
			// open fail
			DBGPRINT(RT_DEBUG_TRACE, ("<=== rt2860_resume()\n"));
			return 0;
		}

		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
		netif_start_queue(net_dev);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("<=== rt2860_resume()\n"));
	return 0;
}
#endif // CONFIG_PM //
#endif


static INT __init rt2860_init_module(VOID)
{
    int ret;

    ret=rbus_module_init();// Roger debug without PCI card

#if defined(CONFIG_RT2860V2_AP_RBUS_PCI) || defined(CONFIG_RT2860V2_STA_RBUS_PCI)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	ret = pci_register_driver(&rt2860_driver);
#else
    ret = pci_module_init(&rt2860_driver);
#endif
#endif
    return ret;
}


//
// Driver module unload function
//
static VOID __exit rt2860_cleanup_module(VOID)
{
#if defined (AP_LED) || defined (STA_LED)
	extern RALINK_TIMER_STRUCT LedCheckTimer;
	extern unsigned char CheckTimerEbl;
	{
		BOOLEAN  Cancelled;
		RTMPCancelTimer(&LedCheckTimer, &Cancelled);
		CheckTimerEbl=0;
	}
#endif // (AP_LED) || (STA_LED) //

    rbus_module_exit();// Roger debug without PCI card

#if defined(CONFIG_RT2860V2_AP_RBUS_PCI) || defined(CONFIG_RT2860V2_STA_RBUS_PCI)
    pci_unregister_driver(&rt2860_driver);
#endif
}

module_init(rt2860_init_module);
module_exit(rt2860_cleanup_module);

#if defined(CONFIG_RT2860V2_AP_RBUS_PCI) || defined(CONFIG_RT2860V2_STA_RBUS_PCI)

static INT __devinit rt2860_init_one (
    IN  struct pci_dev              *pci_dev,
    IN  const struct pci_device_id  *ent)
{
    INT rc;

    DBGPRINT(RT_DEBUG_TRACE, ("===> rt2860_init_one\n"));

    // wake up and enable device
    if (pci_enable_device (pci_dev))
    {
        rc = -EIO;
    }
    else
    {
        rc = rt2860_probe(pci_dev, ent);
    }

    DBGPRINT(RT_DEBUG_TRACE, ("<=== rt2860_init_one\n"));
    return rc;
}


static VOID __devexit rt2860_remove_one(
    IN  struct pci_dev  *pci_dev)
{
    struct net_device   *net_dev = pci_get_drvdata(pci_dev);
    RTMP_ADAPTER        *pAd;

    DBGPRINT(RT_DEBUG_TRACE, ("===> rt2860_remove_one\n"));

	if ( net_dev!=NULL && net_dev->priv != NULL)
	{
		pAd = net_dev->priv;
#ifdef MULTIPLE_CARD_SUPPORT
		if ((pAd->MC_RowID >= 0) && (pAd->MC_RowID <= MAX_NUM_OF_MULTIPLE_CARD))
			MC_CardUsed[pAd->MC_RowID] = 0; // not clear MAC address
#endif // MULTIPLE_CARD_SUPPORT //

#ifdef WSC_INCLUDED
		if (pAd->write_dat_file_pid >= 0)
		{
			int ret;
			pAd->time_to_die = 1;
			up(&(pAd->write_dat_file_semaphore));
			wmb(); // need to check
			ret = kill_proc (pAd->write_dat_file_pid, SIGTERM, 1);
			if (ret)
			{
				printk (KERN_ERR "%s: unable to signal thread\n", pAd->net_dev->name);
				return;
			}
			wait_for_completion (&pAd->write_dat_file_notify);
		}

		if (pAd->pHmacData)
			kfree(pAd->pHmacData);
#endif // WSC_INCLUDED //
    
#ifdef MESH_SUPPORT
	RTMP_Mesh_Remove(pAd);
#endif // MESH_SUPPORT //

#ifdef CONFIG_AP_SUPPORT
#ifdef APCLI_SUPPORT
		// remove all AP-client virtual interfaces.
		RT28xx_ApCli_Remove(pAd);
#endif // APCLI_SUPPORT //

#ifdef WDS_SUPPORT
		// remove all WDS virtual interfaces.
		RT28xx_WDS_Remove(pAd);
#endif // WDS_SUPPORT //

#ifdef MBSS_SUPPORT
		RT28xx_MBSS_Remove(pAd);
#endif // MBSS_SUPPORT //
#endif // CONFIG_AP_SUPPORT //

		// Unregister network device
		unregister_netdev(net_dev);

		// Unmap CSR base address
		iounmap((char *)(net_dev->base_addr));

		RTMPFreeAdapter(pAd);

		// release memory region
		release_mem_region(pci_resource_start(pci_dev, 0), pci_resource_len(pci_dev, 0));

	// Free pre-allocated net_device memory
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	free_netdev(net_dev);
#else
	kfree(net_dev);
#endif
	}
	else
	{
		// Unregister network device
		unregister_netdev(net_dev);

		// Unmap CSR base address
		iounmap((char *)(net_dev->base_addr));

		// release memory region
		release_mem_region(pci_resource_start(pci_dev, 0), pci_resource_len(pci_dev, 0));
	}
}

//
// PCI device probe & initialization function
//
static INT __devinit   rt2860_probe(
    IN  struct pci_dev              *pci_dev, 
    IN  const struct pci_device_id  *ent)
{
	PRTMP_ADAPTER pAd;
    INT rv = 0;
    USHORT  device_id;
    BOOLEAN isNoneAdvancedPSdevice = TRUE;

    rv = (INT)rt28xx_probe((void *)pci_dev, (void *)ent, 0, &pAd);
    pci_read_config_word(pci_dev, PCI_DEVICE_ID, &device_id);
    device_id = le2cpu16(device_id);
    if ((device_id == NIC2860_PCIe_DEVICE_ID) || 
        (device_id == NIC2790_PCIe_DEVICE_ID))
    {
        UINT32 MacCsr0 = 0, Index= 0;
        do 
    	{
    		RTMP_IO_READ32(pAd, MAC_CSR0, &MacCsr0);

    		if ((MacCsr0 != 0x00) && (MacCsr0 != 0xFFFFFFFF))
    			break;

    		RTMPusecDelay(10);
    	} while (Index++ < 100);

        // Support advanced power save after 2892/2790.
        // MAC version at offset 0x1000 is 0x2872XXXX/0x2870XXXX(PCIe, USB, SDIO).
        if ((MacCsr0&0xffff0000) != 0x28600000)
        {
            // Need to record pci_dev point for accessing PCIe link control field.
            pAd->pci_dev = pci_dev;
            OPSTATUS_SET_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE);
            isNoneAdvancedPSdevice = FALSE;
        }
    }
    if (isNoneAdvancedPSdevice)
    {
        pAd->pci_dev = NULL;
        pAd->parent_pci_dev = NULL;
        OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE);
    }
    
	return rv;
}
#endif


void init_thread_task(IN PRTMP_ADAPTER pAd)
{
	POS_COOKIE pObj;

	pObj = (POS_COOKIE) pAd->OS_Cookie;
	
	tasklet_init(&pObj->rx_done_task, rx_done_tasklet, (unsigned long)pAd);
	tasklet_init(&pObj->mgmt_dma_done_task, mgmt_dma_done_tasklet, (unsigned long)pAd);
	tasklet_init(&pObj->ac0_dma_done_task, ac0_dma_done_tasklet, (unsigned long)pAd);
	tasklet_init(&pObj->ac1_dma_done_task, ac1_dma_done_tasklet, (unsigned long)pAd);
	tasklet_init(&pObj->ac2_dma_done_task, ac2_dma_done_tasklet, (unsigned long)pAd);
	tasklet_init(&pObj->ac3_dma_done_task, ac3_dma_done_tasklet, (unsigned long)pAd);
	tasklet_init(&pObj->hcca_dma_done_task, hcca_dma_done_tasklet, (unsigned long)pAd);
	tasklet_init(&pObj->tbtt_task, tbtt_tasklet, (unsigned long)pAd);
	tasklet_init(&pObj->fifo_statistic_full_task, fifo_statistic_full_tasklet, (unsigned long)pAd);

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
#ifdef DFS_SUPPORT
		tasklet_init(&pObj->pulse_radar_detect_task, pulse_radar_detect_tasklet, (unsigned long)pAd);
		tasklet_init(&pObj->width_radar_detect_task, width_radar_detect_tasklet, (unsigned long)pAd);
#endif // DFS_SUPPORT //
#ifdef CARRIER_DETECTION_SUPPORT
		tasklet_init(&pObj->carrier_sense_task, carrier_sense_tasklet, (unsigned long)pAd);
#endif // CARRIER_DETECTION_SUPPORT //
	}
#endif // CONFIG_AP_SUPPORT //
}

void kill_thread_task(IN PRTMP_ADAPTER pAd)
{
	POS_COOKIE pObj;

	pObj = (POS_COOKIE) pAd->OS_Cookie;

	tasklet_kill(&pObj->rx_done_task);
	tasklet_kill(&pObj->mgmt_dma_done_task);
	tasklet_kill(&pObj->ac0_dma_done_task);
	tasklet_kill(&pObj->ac1_dma_done_task);
	tasklet_kill(&pObj->ac2_dma_done_task);
	tasklet_kill(&pObj->ac3_dma_done_task);
	tasklet_kill(&pObj->hcca_dma_done_task);
	tasklet_kill(&pObj->tbtt_task);
	tasklet_kill(&pObj->fifo_statistic_full_task);
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
#ifdef CARRIER_DETECTION_SUPPORT
		tasklet_kill(&pObj->carrier_sense_task);
#endif // CARRIER_DETECTION_SUPPORT //
#ifdef DFS_SUPPORT
		tasklet_kill(&pObj->width_radar_detect_task);
		tasklet_kill(&pObj->pulse_radar_detect_task);
#endif // DFS_SUPPORT //
	}
#endif // CONFIG_AP_SUPPORT //
}


static void rt2860_int_enable(PRTMP_ADAPTER pAd, unsigned int mode)
{
	u32 regValue;

	pAd->int_disable_mask &= ~(mode);
	regValue = pAd->int_enable_reg & ~(pAd->int_disable_mask);		
	//if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_DOZE))
	{
		RTMP_IO_WRITE32(pAd, INT_MASK_CSR, regValue);     // 1:enable
	}
	//else
	//	DBGPRINT(RT_DEBUG_TRACE, ("fOP_STATUS_DOZE !\n"));

	if (regValue != 0)
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE);
}


static void rt2860_int_disable(PRTMP_ADAPTER pAd, unsigned int mode)
{
	u32 regValue;

	pAd->int_disable_mask |= mode;
	regValue = 	pAd->int_enable_reg & ~(pAd->int_disable_mask);
	RTMP_IO_WRITE32(pAd, INT_MASK_CSR, regValue);     // 0: disable 
	
	if (regValue == 0)
	{
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE);		
	}
}

static void mgmt_dma_done_tasklet(unsigned long data)
{
	unsigned long flags;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;
	POS_COOKIE pObj;
	
    pObj = (POS_COOKIE) pAd->OS_Cookie;

//	printk("mgmt_dma_done_process\n");
	IntSource.word = 0;
	IntSource.field.MgmtDmaDone = 1;
	pAd->int_pending &= ~INT_MGMT_DLY;
	
	RTMPHandleMgmtRingDmaDoneInterrupt(pAd);

	// if you use RTMP_SEM_LOCK, sometimes kernel will hang up, no any
	// bug report output
	RTMP_INT_LOCK(&pAd->irq_lock, flags);
	/*
	 * double check to avoid lose of interrupts
	 */
	if (pAd->int_pending & INT_MGMT_DLY) 
	{
		tasklet_hi_schedule(&pObj->mgmt_dma_done_task);
		RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
		return;
	}

	/* enable TxDataInt again */
	rt2860_int_enable(pAd, INT_MGMT_DLY);
	RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
}

static void rx_done_tasklet(unsigned long data)
{
	unsigned long flags;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) data;
	BOOLEAN	bReschedule = 0;
	POS_COOKIE pObj;
	
    pObj = (POS_COOKIE) pAd->OS_Cookie;
	
	pAd->int_pending &= ~(INT_RX);
#ifdef CONFIG_AP_SUPPORT	
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
		bReschedule = APRxDoneInterruptHandle(pAd);
#endif // CONFIG_AP_SUPPORT //	
#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
		bReschedule = STARxDoneInterruptHandle(pAd, 0);
#endif // CONFIG_STA_SUPPORT //

	RTMP_INT_LOCK(&pAd->irq_lock, flags);
	/*
	 * double check to avoid rotting packet 
	 */
	if (pAd->int_pending & INT_RX || bReschedule) 
	{
		tasklet_hi_schedule(&pObj->rx_done_task);
		RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
		return;
	}

	/* enable RxINT again */
	rt2860_int_enable(pAd, INT_RX);
	RTMP_INT_UNLOCK(&pAd->irq_lock, flags);

}

void fifo_statistic_full_tasklet(unsigned long data)
{
	unsigned long flags;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) data;
	POS_COOKIE pObj;
	
    pObj = (POS_COOKIE) pAd->OS_Cookie;

	pAd->int_pending &= ~(FifoStaFullInt); 
	NICUpdateFifoStaCounters(pAd);
	
	RTMP_INT_LOCK(&pAd->irq_lock, flags);  
	/*
	 * double check to avoid rotting packet 
	 */
	if (pAd->int_pending & FifoStaFullInt) 
	{
		tasklet_hi_schedule(&pObj->fifo_statistic_full_task);
		RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
		return;
	}

	/* enable RxINT again */

	rt2860_int_enable(pAd, FifoStaFullInt);
	RTMP_INT_UNLOCK(&pAd->irq_lock, flags);

}


static void hcca_dma_done_tasklet(unsigned long data)
{
	unsigned long flags;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;
	POS_COOKIE pObj;
	
    pObj = (POS_COOKIE) pAd->OS_Cookie;


	IntSource.word = 0;
	IntSource.field.HccaDmaDone = 1;
	pAd->int_pending &= ~INT_HCCA_DLY;

	RTMPHandleTxRingDmaDoneInterrupt(pAd, IntSource);

	RTMP_INT_LOCK(&pAd->irq_lock, flags);
	/*
	 * double check to avoid lose of interrupts
	 */
	if (pAd->int_pending & INT_HCCA_DLY)
	{
		tasklet_hi_schedule(&pObj->hcca_dma_done_task);
		RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
		return;
	}

	/* enable TxDataInt again */
	rt2860_int_enable(pAd, INT_HCCA_DLY);
	RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
}

static void ac3_dma_done_tasklet(unsigned long data)
{
	unsigned long flags;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;
	POS_COOKIE pObj;
	BOOLEAN bReschedule = 0;

	
    pObj = (POS_COOKIE) pAd->OS_Cookie;

//	printk("ac0_dma_done_process\n");
	IntSource.word = 0;
	IntSource.field.Ac3DmaDone = 1;
	pAd->int_pending &= ~INT_AC3_DLY;

	bReschedule = RTMPHandleTxRingDmaDoneInterrupt(pAd, IntSource);

	RTMP_INT_LOCK(&pAd->irq_lock, flags);
	/*
	 * double check to avoid lose of interrupts
	 */
	if ((pAd->int_pending & INT_AC3_DLY) || bReschedule)
	{
		tasklet_hi_schedule(&pObj->ac3_dma_done_task);
		RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
		return;
	}

	/* enable TxDataInt again */
	rt2860_int_enable(pAd, INT_AC3_DLY);
	RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
}

static void ac2_dma_done_tasklet(unsigned long data)
{
	unsigned long flags;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;
	POS_COOKIE pObj;
	BOOLEAN bReschedule = 0;
	
    pObj = (POS_COOKIE) pAd->OS_Cookie;

	IntSource.word = 0;
	IntSource.field.Ac2DmaDone = 1;
	pAd->int_pending &= ~INT_AC2_DLY;

	bReschedule = RTMPHandleTxRingDmaDoneInterrupt(pAd, IntSource);

	RTMP_INT_LOCK(&pAd->irq_lock, flags);

	/*
	 * double check to avoid lose of interrupts
	 */
	if ((pAd->int_pending & INT_AC2_DLY) || bReschedule) 
	{
		tasklet_hi_schedule(&pObj->ac2_dma_done_task);
		RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
		return;
	}

	/* enable TxDataInt again */
	rt2860_int_enable(pAd, INT_AC2_DLY);
	RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
}

static void ac1_dma_done_tasklet(unsigned long data)
{
	unsigned long flags;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;
	POS_COOKIE pObj;
	BOOLEAN bReschedule = 0;

	
    pObj = (POS_COOKIE) pAd->OS_Cookie;

//	printk("ac0_dma_done_process\n");
	IntSource.word = 0;
	IntSource.field.Ac1DmaDone = 1;
	pAd->int_pending &= ~INT_AC1_DLY;

	bReschedule = RTMPHandleTxRingDmaDoneInterrupt(pAd, IntSource);

	RTMP_INT_LOCK(&pAd->irq_lock, flags);
	/*
	 * double check to avoid lose of interrupts
	 */
	if ((pAd->int_pending & INT_AC1_DLY) || bReschedule) 
	{
		tasklet_hi_schedule(&pObj->ac1_dma_done_task);
		RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
		return;
	}

	/* enable TxDataInt again */
	rt2860_int_enable(pAd, INT_AC1_DLY);
	RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
}

static void ac0_dma_done_tasklet(unsigned long data)
{
	unsigned long flags;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) data;
    INT_SOURCE_CSR_STRUC	IntSource;
	POS_COOKIE pObj;
	BOOLEAN bReschedule = 0;

	
    pObj = (POS_COOKIE) pAd->OS_Cookie;

//	printk("ac0_dma_done_process\n");
	IntSource.word = 0;
	IntSource.field.Ac0DmaDone = 1;
	pAd->int_pending &= ~INT_AC0_DLY;

//	RTMPHandleMgmtRingDmaDoneInterrupt(pAd);
	bReschedule = RTMPHandleTxRingDmaDoneInterrupt(pAd, IntSource);
	
	RTMP_INT_LOCK(&pAd->irq_lock, flags);
	/*
	 * double check to avoid lose of interrupts
	 */
	if ((pAd->int_pending & INT_AC0_DLY) || bReschedule)
	{
		tasklet_hi_schedule(&pObj->ac0_dma_done_task);
		RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
		return;
	}

	/* enable TxDataInt again */
	rt2860_int_enable(pAd, INT_AC0_DLY);
	RTMP_INT_UNLOCK(&pAd->irq_lock, flags);    
}


int print_int_count;

IRQ_HANDLE_TYPE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19))
rt2860_interrupt(int irq, void *dev_instance)
#else
rt2860_interrupt(int irq, void *dev_instance, struct pt_regs *regs)
#endif
{
	struct net_device *net_dev = (struct net_device *) dev_instance;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) net_dev->priv;
	INT_SOURCE_CSR_STRUC	IntSource;
	POS_COOKIE pObj;
	
	pObj = (POS_COOKIE) pAd->OS_Cookie;

	//
	// Inital the Interrupt source.
	//
	IntSource.word = 0x00000000L;
//	McuIntSource.word = 0x00000000L;



	//
	// Get the interrupt sources & saved to local variable
	//
	//RTMP_IO_READ32(pAd, where, &McuIntSource.word);
	//RTMP_IO_WRITE32(pAd, , McuIntSource.word);

	//
	// Flag fOP_STATUS_DOZE On, means ASIC put to sleep, elase means ASICK WakeUp
	// And at the same time, clock maybe turned off that say there is no DMA service.
	// when ASIC get to sleep. 
	// To prevent system hang on power saving.
	// We need to check it before handle the INT_SOURCE_CSR, ASIC must be wake up.
	//
	// RT2661 => when ASIC is sleeping, MAC register cannot be read and written.
	// RT2860 => when ASIC is sleeping, MAC register can be read and written.
//	if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_DOZE))
	{
		RTMP_IO_READ32(pAd, INT_SOURCE_CSR, &IntSource.word);
		RTMP_IO_WRITE32(pAd, INT_SOURCE_CSR, IntSource.word); // write 1 to clear

		 // Do nothing if Reset in progress
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS) ||
			RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
		{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
		    return  IRQ_HANDLED;
#else
		    return;
#endif
		}

	}
//	else
//		DBGPRINT(RT_DEBUG_TRACE, (">>>fOP_STATUS_DOZE<<<\n"));

//	RTMP_IO_READ32(pAd, INT_SOURCE_CSR, &IsrAfterClear);
//	RTMP_IO_READ32(pAd, MCU_INT_SOURCE_CSR, &McuIsrAfterClear);
//	DBGPRINT(RT_DEBUG_INFO, ("====> RTMPHandleInterrupt(ISR=%08x,Mcu ISR=%08x, After clear ISR=%08x, MCU ISR=%08x)\n",
//			IntSource.word, McuIntSource.word, IsrAfterClear, McuIsrAfterClear));

	//
	// Handle interrupt, walk through all bits
	// Should start from highest priority interrupt
	// The priority can be adjust by altering processing if statement
	//
#if 0
{
	u32 regValue;

	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &regValue);
	printk("MAC_SYS_CTRL = %08x\n", regValue);

	RTMP_IO_READ32(pAd, 0x400, &regValue);
	printk("MCU 0x400 = %08x\n", regValue);

	RTMP_IO_READ32(pAd, 0x404, &regValue);
	printk("MCU 0x404 = %08x\n", regValue);

	RTMP_IO_READ32(pAd, 0x408, &regValue);
	printk("MCU 0x408 = %08x\n", regValue);

	RTMP_IO_READ32(pAd, 0x40C, &regValue);
	printk("MCU 0x40C = %08x\n", regValue);

	RTMP_IO_READ32(pAd, 0x410, &regValue);
	printk("MCU 0x410 = %08x\n", regValue);

	RTMP_IO_READ32(pAd, 0x414, &regValue);
	printk("MCU_INT_STA = %08x\n", regValue);
}
#endif 

#ifdef DBG
	if ((RTDebugLevel == RT_DEBUG_LOUD) && (((++print_int_count) % 100) == 0))
	{
		UINT32 reg = 0;
		int Count, free;

		RTMP_IO_READ32(pAd, INT_MASK_CSR, &reg);     // 1:enable
		printk("%d: INT_MASK_CSR = %08x, IntSource %08x\n", print_int_count, reg, IntSource.word);
		RTMP_IO_READ32(pAd, TX_CTX_IDX0 + 0 * 0x10 , &reg);
		printk("TX_CTX_IDX0 = %08x\n", reg);
		RTMP_IO_READ32(pAd, TX_DTX_IDX0 + 0 * 0x10 , &reg);
		printk("TX_DTX_IDX0 = %08x\n", reg);
		RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &reg);
		printk("WPDMA_GLO_CFG = %08x\n", reg);
		for (Count = 0; Count < 1; Count++)
		{
			if (pAd->TxRing[Count].TxSwFreeIdx> pAd->TxRing[Count].TxCpuIdx)
				free = pAd->TxRing[Count].TxSwFreeIdx - pAd->TxRing[Count].TxCpuIdx -1;
			else
				free = pAd->TxRing[Count].TxSwFreeIdx + TX_RING_SIZE - pAd->TxRing[Count].TxCpuIdx -1;
		
			printk("%d: Free = %d TxSwFreeIdx = %d\n", Count, free, pAd->TxRing[Count].TxSwFreeIdx); 
		}
		printk("pAd->int_disable_mask = %08x\n", pAd->int_disable_mask);
		printk("pAd->int_enable_reg = %08x\n", pAd->int_enable_reg);
		printk("pAd->int_pending = %08x\n", pAd->int_pending);
		RTMP_IO_READ32(pAd, RX_DRX_IDX , &reg);
		printk("pAd->RxRing.RxSwReadIdx = %08x, RX_DRX_IDX = %08x\n", pAd->RxRing.RxSwReadIdx, reg);
//		NetJobAdd(APRxDoneIntProcess, pAd, 0, 0);
	}
#endif
		

    pAd->bPCIclkOff = FALSE;

	// If required spinlock, each interrupt service routine has to acquire
	// and release itself.
	//
	
	if (IntSource.word & TxCoherent)
	{
		DBGPRINT(RT_DEBUG_ERROR, (">>>TxCoherent<<<\n"));
		RTMPHandleRxCoherentInterrupt(pAd);
	}

	if (IntSource.word & RxCoherent)
	{
		DBGPRINT(RT_DEBUG_ERROR, (">>>RxCoherent<<<\n"));
		RTMPHandleRxCoherentInterrupt(pAd);
	}

	if (IntSource.word & FifoStaFullInt) 
	{
#if 1
		if ((pAd->int_disable_mask & FifoStaFullInt) == 0) 
		{
			/* mask FifoStaFullInt */
			rt2860_int_disable(pAd, FifoStaFullInt);
			tasklet_hi_schedule(&pObj->fifo_statistic_full_task);
		}
		pAd->int_pending |= FifoStaFullInt; 
#else
		NICUpdateFifoStaCounters(pAd);		
#endif
	}

	if (IntSource.word & INT_MGMT_DLY) 
	{
		if ((pAd->int_disable_mask & INT_MGMT_DLY) ==0 )
		{
			rt2860_int_disable(pAd, INT_MGMT_DLY);
			tasklet_hi_schedule(&pObj->mgmt_dma_done_task);			
		}
		//RTMPHandleMgmtRingDmaDoneInterrupt(pAd);
		pAd->int_pending |= INT_MGMT_DLY ;
	}

	if (IntSource.word & INT_RX)
	{
		if ((pAd->int_disable_mask & INT_RX) == 0) 
		{
			/* mask RxINT */
			rt2860_int_disable(pAd, INT_RX);
			tasklet_hi_schedule(&pObj->rx_done_task);
		}
		pAd->int_pending |= INT_RX; 		
	}

	if (IntSource.word & INT_HCCA_DLY)
	{

		if ((pAd->int_disable_mask & INT_HCCA_DLY) == 0) 
		{
			/* mask TxDataInt */
			rt2860_int_disable(pAd, INT_HCCA_DLY);
			tasklet_hi_schedule(&pObj->hcca_dma_done_task);
		}
		pAd->int_pending |= INT_HCCA_DLY;						
	}

	if (IntSource.word & INT_AC3_DLY)
	{

		if ((pAd->int_disable_mask & INT_AC3_DLY) == 0) 
		{
			/* mask TxDataInt */
			rt2860_int_disable(pAd, INT_AC3_DLY);
			tasklet_hi_schedule(&pObj->ac3_dma_done_task);
		}
		pAd->int_pending |= INT_AC3_DLY;						
	}

	if (IntSource.word & INT_AC2_DLY)
	{

		if ((pAd->int_disable_mask & INT_AC2_DLY) == 0) 
		{
			/* mask TxDataInt */
			rt2860_int_disable(pAd, INT_AC2_DLY);
			tasklet_hi_schedule(&pObj->ac2_dma_done_task);
		}
		pAd->int_pending |= INT_AC2_DLY;						
	}

	if (IntSource.word & INT_AC1_DLY)
	{

		pAd->int_pending |= INT_AC1_DLY;						

		if ((pAd->int_disable_mask & INT_AC1_DLY) == 0) 
		{
			/* mask TxDataInt */
			rt2860_int_disable(pAd, INT_AC1_DLY);
			tasklet_hi_schedule(&pObj->ac1_dma_done_task);
		}
		
	}

	if (IntSource.word & INT_AC0_DLY)
	{

/*
		if (IntSource.word & 0x2) {
			u32 reg;
			RTMP_IO_READ32(pAd, DELAY_INT_CFG, &reg);
			printk("IntSource.word = %08x, DELAY_REG = %08x\n", IntSource.word, reg);
		}
*/
		pAd->int_pending |= INT_AC0_DLY;

		if ((pAd->int_disable_mask & INT_AC0_DLY) == 0) 
		{
			/* mask TxDataInt */
			rt2860_int_disable(pAd, INT_AC0_DLY);
			tasklet_hi_schedule(&pObj->ac0_dma_done_task);
		}
								
	}

#ifdef CARRIER_DETECTION_SUPPORT
#ifdef CONFIG_RALINK_RT3052
	if (IntSource.word & INT_TONE_RADAR)
	{
		RTMPHandleRadarInterrupt(pAd);
	}
#endif
#endif



    if (IntSource.word & PreTBTTInt)
	{
		RTMPHandlePreTBTTInterrupt(pAd);
#if 0
#ifdef DFS_SUPPORT
		if ((pAd->CommonCfg.bIEEE80211H) && (pAd->CommonCfg.RadarDetect.bFastDfs == 1) && (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W56))
		{
			ULONG Value;
			// Restore BBP
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R65, pAd->CommonCfg._R65);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R66, pAd->CommonCfg._R66);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, pAd->CommonCfg._R69);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R70, pAd->CommonCfg._R70);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, pAd->CommonCfg._R73);
			BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R113, 0x01);

			// EnableNormalTx
			RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);
			Value |= 0x04;
			RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Value);

			RTMP_IO_WRITE32(pAd, TX_RTS_CFG, pAd->CommonCfg.OldRtsRetryLimit);

			// give up detection to let beacon go
			pAd->CommonCfg.McuRadarState = FREE_FOR_TX; // timeout 5ms, give up
			pAd->CommonCfg.McuRadarEvent &= ~(RADAR_EVENT_RADAR_DETECTING | RADAR_EVENT_CTS_KICKED);

		}
#endif // DFS_SUPPORT //
#endif
	}

	if (IntSource.word & TBTTInt)
	{
		RTMPHandleTBTTInterrupt(pAd);
	}

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
	{
		if (IntSource.word & McuCommand)
		{
			UINT32 McuIntSrc = 0;

			RTMP_IO_READ32(pAd, 0x7024, &McuIntSrc);

			// clear MCU Int source register.
			RTMP_IO_WRITE32(pAd, 0x7024, 0);

#ifdef DFS_SUPPORT
			if (pAd->CommonCfg.bIEEE80211H)
			{
				// pulse radar signal Int.
				if (McuIntSrc & 0x40)
				{
					tasklet_hi_schedule(&pObj->pulse_radar_detect_task);
				}

				// width radar signal Int.
				if(((pAd->CommonCfg.RadarDetect.RDDurRegion == FCC)
						|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W56))
					&& (McuIntSrc & 0x80))
				{
					tasklet_hi_schedule(&pObj->width_radar_detect_task);
				}

				// long pulse radar signal detection.
				if(((pAd->CommonCfg.RadarDetect.RDDurRegion == FCC)
						|| (pAd->CommonCfg.RadarDetect.RDDurRegion == JAP_W56)
						|| (JapRadarType(pAd) == JAP_W56))
				&& (McuIntSrc & 0x10))
				{
					tasklet_hi_schedule(&pObj->width_radar_detect_task);
				}
			}
#endif // DFS_SUPPORT //
#ifdef CARRIER_DETECTION_SUPPORT
			if ((pAd->CommonCfg.CarrierDetect.Enable)
				&& (McuIntSrc & 0x04))
			{
				tasklet_hi_schedule(&pObj->carrier_sense_task);
			}
#endif // CARRIER_DETECTION_SUPPORT //
		}
	}
#endif // CONFIG_AP_SUPPORT //


#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		if (IntSource.word & AutoWakeupInt)
			RTMPHandleTwakeupInterrupt(pAd);
	}
#endif // CONFIG_STA_SUPPORT //

	// Do nothing if Reset in progress
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS) ||
		RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
	{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
        return  IRQ_HANDLED;
#else
        return;
#endif
	}


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
    return  IRQ_HANDLED;
#endif

}

#if 0
/*
========================================================================
Routine Description:
    Close kernel threads.

Arguments:
	*pAd				the raxx interface data pointer

Return Value:
    NONE

Note:
========================================================================
*/
VOID RT28xxThreadTerminate(
	IN RTMP_ADAPTER *pAd)
{
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
	kill_thread_task(pAd);
}
#endif


/*
========================================================================
Routine Description:
    Check the chipset vendor/product ID.

Arguments:
    _dev_p				Point to the PCI or USB device

Return Value:
    TRUE				Check ok
	FALSE				Check fail

Note:
========================================================================
*/
BOOLEAN RT28XXChipsetCheck(
	IN void *_dev_p)
{
	/* always TRUE */
	return TRUE;
}

#if defined(CONFIG_RT2860V2_AP_RBUS_PCI) || defined(CONFIG_RT2860V2_STA_RBUS_PCI)
/*
========================================================================
Routine Description:
    Init net device structure.

Arguments:
    _dev_p				Point to the PCI or USB device
    *net_dev			Point to the net device
	*pAd				the raxx interface data pointer

Return Value:
    TRUE				Init ok
	FALSE				Init fail

Note:
========================================================================
*/
BOOLEAN RT28XXNetDevInit(
	IN void 				*_dev_p,
	IN struct  net_device	*net_dev,
	IN RTMP_ADAPTER 		*pAd)
{
	struct pci_dev *pci_dev = (struct pci_dev *)_dev_p;
    CHAR	*print_name;
    ULONG	csr_addr;


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
    print_name = pci_dev ? pci_name(pci_dev) : "rt2860";
#else
    print_name = pci_dev ? pci_dev->slot_name : "rt2860";
#endif // LINUX_VERSION_CODE //

	net_dev->base_addr = 0;
	net_dev->irq = 0;

    if (pci_request_regions(pci_dev, print_name))
        goto err_out_free_netdev;

    // interrupt IRQ number
    net_dev->irq = pci_dev->irq;

    // map physical address to virtual address for accessing register
    csr_addr = (unsigned long) ioremap(pci_resource_start(pci_dev, 0),
										pci_resource_len(pci_dev, 0));

    if (!csr_addr)
    {
        DBGPRINT(RT_DEBUG_ERROR,
				("ioremap failed for device %s, region 0x%lX @ 0x%lX\n",
				print_name, (ULONG)pci_resource_len(pci_dev, 0),
				(ULONG)pci_resource_start(pci_dev, 0)));
        goto err_out_free_res;
    }

    // Save CSR virtual address and irq to device structure
    net_dev->base_addr = csr_addr;
    pAd->CSRBaseAddress = (PUCHAR)net_dev->base_addr;

    // Set DMA master
    pci_set_master(pci_dev);

    net_dev->priv_flags = INT_MAIN;

    //net_dev->set_multicast_list = rt2860_set_rx_mode;
    //net_dev->priv_flags = INT_MAIN;

    DBGPRINT(RT_DEBUG_TRACE, ("%s: at 0x%lx, VA 0x%lx, IRQ %d. \n", 
        	net_dev->name, (ULONG)pci_resource_start(pci_dev, 0),
			(ULONG)csr_addr, pci_dev->irq));
	return TRUE;


	/* --------------------------- ERROR HANDLE --------------------------- */
err_out_free_res:
    pci_release_regions(pci_dev);
err_out_free_netdev:
	/* free netdev in caller, not here */
	return FALSE;
}
#endif // #if defined(CONFIG_RT2860V2_AP_RBUS_PCI) || defined(CONFIG_RT2860V2_STA_RBUS_PCI) //


/*
========================================================================
Routine Description:
    Init net device structure.

Arguments:
    _dev_p				Point to the PCI or USB device
	*pAd				the raxx interface data pointer

Return Value:
    TRUE				Config ok
	FALSE				Config fail

Note:
========================================================================
*/
BOOLEAN RT28XXProbePostConfig(
	IN void 				*_dev_p,
	IN RTMP_ADAPTER 		*pAd,
	IN INT32				argc)
{
	/* no use */
	return TRUE;
}


/*
========================================================================
Routine Description:
    Disable DMA.

Arguments:
	*pAd				the raxx interface data pointer

Return Value:
	None

Note:
========================================================================
*/
VOID RT28XXDMADisable(
	IN RTMP_ADAPTER 		*pAd)
{
	WPDMA_GLO_CFG_STRUC     GloCfg;


	RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);
	GloCfg.word &= 0xff0;
	GloCfg.field.EnTXWriteBackDDONE =1;
	RTMP_IO_WRITE32(pAd, WPDMA_GLO_CFG, GloCfg.word);
}


/*
========================================================================
Routine Description:
    Enable DMA.

Arguments:
	*pAd				the raxx interface data pointer

Return Value:
	None

Note:
========================================================================
*/
VOID RT28XXDMAEnable(
	IN RTMP_ADAPTER 		*pAd)
{
	WPDMA_GLO_CFG_STRUC	GloCfg;
	int i = 0;
	
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x4);
	do
	{
		RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);
		if ((GloCfg.field.TxDMABusy == 0)  && (GloCfg.field.RxDMABusy == 0))
			break;
		
		DBGPRINT(RT_DEBUG_TRACE, ("==>  DMABusy\n"));
		RTMPusecDelay(1000);
		i++;
	}while ( i <200);

	RTMPusecDelay(50);
	
#ifdef RX_SCATTERED
	GloCfg.field.EnTXWriteBackDDONE = 1;
	GloCfg.field.WPDMABurstSIZE = 2;
	GloCfg.field.EnableRxDMA = 1;
	GloCfg.field.EnableTxDMA = 1;
	GloCfg.field.HDR_SEG_LEN = RX_SCATTER_SIZE;
#else
	GloCfg.field.EnTXWriteBackDDONE = 1;
	GloCfg.field.WPDMABurstSIZE = 2;
	GloCfg.field.EnableRxDMA = 1;
	GloCfg.field.EnableTxDMA = 1;
#endif // RX_SCATTERED //

	DBGPRINT(RT_DEBUG_TRACE, ("<== WRITE DMA offset 0x208 = 0x%x\n", GloCfg.word));	
	RTMP_IO_WRITE32(pAd, WPDMA_GLO_CFG, GloCfg.word);

}


#ifdef CONFIG_AP_SUPPORT
/*
========================================================================
Routine Description:
    Write Beacon buffer to Asic.

Arguments:
	*pAd				the raxx interface data pointer

Return Value:
	None

Note:
========================================================================
*/
VOID RT28xx_APUpdateBeaconToAsic(
	IN RTMP_ADAPTER		*pAd,
	IN INT				apidx,
	IN ULONG			FrameLen, 
	IN ULONG			UpdatePos)
{
	//PTXWI_STRUC    	pTxWI = &pAd->BeaconTxWI;
	//PUCHAR        	pBeaconFrame = pAd->ApCfg.MBSSID[apidx].BeaconBuf;
	ULONG			CapInfoPos = pAd->ApCfg.MBSSID[apidx].CapabilityInfoLocationInBeacon;
	//HTTRANSMIT_SETTING	BeaconTransmit;   // MGMT frame PHY rate setting when operatin at Ht rate.
	UCHAR  			*ptr;
	UINT  			i;


	//if ((pAd->WdsTab.Mode == WDS_BRIDGE_MODE)
	//	|| ((pAd->ApCfg.MBSSID[apidx].MSSIDDev == NULL) 
	//		|| !(pAd->ApCfg.MBSSID[apidx].MSSIDDev->flags & IFF_UP))
	//	)
	if (BeaconTransmitRequired(pAd, apidx) == FALSE)
	{
		/* when the ra interface is down, do not send its beacon frame */
		/* clear all zero */
		for(i=0; i<TXWI_SIZE; i+=4)
			RTMP_IO_WRITE32(pAd, pAd->BeaconOffset[apidx] + i, 0x00);
	}
	else
	{
		ptr = (PUCHAR)&pAd->BeaconTxWI;
#ifdef BIG_ENDIAN
		RTMPWIEndianChange(ptr, TYPE_TXWI);
#endif
		for (i=0; i<TXWI_SIZE; i++)  // 16-byte TXINFO field
		{
			RTMP_IO_WRITE8(pAd, pAd->BeaconOffset[apidx] + i, *ptr);
			ptr ++;
		}

		// Update CapabilityInfo in Beacon
		ptr = &pAd->ApCfg.MBSSID[apidx].BeaconBuf[CapInfoPos];
		for (i = CapInfoPos; i < (CapInfoPos+2); i++)
		{
			RTMP_IO_WRITE8(pAd, pAd->BeaconOffset[apidx] + TXWI_SIZE + i, *ptr); 
			ptr ++;
		}

		if (FrameLen > UpdatePos)
		{
			ptr = &pAd->ApCfg.MBSSID[apidx].BeaconBuf[UpdatePos];
			for (i= UpdatePos; i< (FrameLen); i++)
			{
				RTMP_IO_WRITE8(pAd, pAd->BeaconOffset[apidx] + TXWI_SIZE + i, *ptr); 
				ptr ++;
			}
		}
	}
	
}
#endif // CONFIG_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
VOID RTMPInitPCIeLinkCtrlValue(
	IN	PRTMP_ADAPTER	pAd)
{
    INT     pos;
    USHORT	reg16, data2, PCIePowerSaveLevel, Configuration;
    BOOLEAN	bFindIntel = FALSE;

    // Support advanced power save after 2892 . MAC version at offset 0x1000 is 2872(PCIe, USB, SDIO).
	//if ((pAd->MACVersion&0xffff0000) == 0x28600000)
	if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE))
		return;   

    DBGPRINT(RT_DEBUG_TRACE, ("%s.===>\n", __FUNCTION__));
	// Init EEPROM, and save settings
	PCIePowerSaveLevel = RTMP_EEPROM_READ16(pAd, 0x22);
	pAd->PCIePowerSaveLevel = PCIePowerSaveLevel & 0xff;

    if ((PCIePowerSaveLevel&0xff) != 0xff)
	{
		PCIePowerSaveLevel &= 0x3;
		data2 = RTMP_EEPROM_READ16(pAd, 0x24);

		if( !(((data2&0xff00) == 0x9200) && ((data2&0x80) !=0)) )
		{
			if (PCIePowerSaveLevel > 1 ) 
				PCIePowerSaveLevel = 1;
		}

		DBGPRINT(RT_DEBUG_TRACE, ("====> Write 0x83 = 0x%x.\n", PCIePowerSaveLevel));
		AsicSendCommandToMcu(pAd, 0x83, 0xff, (UCHAR)PCIePowerSaveLevel, 0x00);
		PCIePowerSaveLevel = RTMP_EEPROM_READ16(pAd, 0x22);
		PCIePowerSaveLevel &= 0xff;
		PCIePowerSaveLevel = PCIePowerSaveLevel >> 6;
		switch(PCIePowerSaveLevel)
		{
			case 0:
				pAd->LnkCtrlBitMask = 0;
				break;
			case 1:
				pAd->LnkCtrlBitMask = 1;
				break;
			case 2:
				pAd->LnkCtrlBitMask = 3;
				break;
			case 3:
				pAd->LnkCtrlBitMask = 0x103;
				break;
		}
		DBGPRINT(RT_DEBUG_TRACE, ("====> LnkCtrlBitMask = 0x%x.\n", pAd->LnkCtrlBitMask));
	}   
    
    // Find Ralink PCIe Device's Express Capability Offset
	pos = pci_find_capability(pAd->pci_dev, PCI_CAP_ID_EXP);

    if (pos != 0)
    {
        // Ralink PCIe Device's Link Control Register Offset
        pAd->RLnkCtrlOffset = pos + PCI_EXP_LNKCTL;
    	pci_read_config_word(pAd->pci_dev, pAd->RLnkCtrlOffset, &reg16);
        Configuration = le2cpu16(reg16);
        DBGPRINT(RT_DEBUG_TRACE, ("Read (Ralink PCIe Link Control Register) offset 0x%x = 0x%x\n", 
                                    pAd->RLnkCtrlOffset, Configuration));
        pAd->RLnkCtrlConfiguration = (Configuration & 0x103);
        Configuration &= 0xfefc;
        // value with 0 is more stable than value with 1, 
        // but value with 1 is more power save than value with 0.
        Configuration |= (0x0); //(0x1); 
        reg16 = cpu2le16(Configuration);
        pci_write_config_word(pAd->pci_dev, pAd->RLnkCtrlOffset, reg16);
        DBGPRINT(RT_DEBUG_TRACE, ("Write (Ralink PCIe Link Control Register)  offset 0x%x = 0x%x\n", 
                                    pos + PCI_EXP_LNKCTL, Configuration));
        
        RTMPFindHostPCIDev(pAd);
        if (pAd->parent_pci_dev)
        {
            USHORT  vendor_id;
            
            pci_read_config_word(pAd->parent_pci_dev, PCI_VENDOR_ID, &vendor_id);
            vendor_id = le2cpu16(vendor_id);
            if (vendor_id == PCIBUS_INTEL_VENDOR)
				bFindIntel = TRUE;
            
            // Find PCI-to-PCI Bridge Express Capability Offset
            pos = pci_find_capability(pAd->parent_pci_dev, PCI_CAP_ID_EXP);

            if (pos != 0)
            {
                // PCI-to-PCI Bridge Link Control Register Offset
                pAd->HostLnkCtrlOffset = pos + PCI_EXP_LNKCTL;
            	pci_read_config_word(pAd->parent_pci_dev, pAd->HostLnkCtrlOffset, &reg16);    
                Configuration = le2cpu16(reg16);
                DBGPRINT(RT_DEBUG_TRACE, ("Read (Host PCI-to-PCI Bridge Link Control Register) offset 0x%x = 0x%x\n", 
                                            pAd->HostLnkCtrlOffset, Configuration));    
                pAd->HostLnkCtrlConfiguration = (Configuration & 0x103);
                Configuration &= 0xfefc;
                // value with 0 is more stable than value with 1, 
                // but value with 1 is more power save than value with 0.
                Configuration |= (0x0); //(0x1);
                reg16 = cpu2le16(Configuration);
                pci_write_config_word(pAd->parent_pci_dev, pAd->HostLnkCtrlOffset, reg16);
                DBGPRINT(RT_DEBUG_TRACE, ("Write (Host PCI-to-PCI Bridge Link Control Register) offset 0x%x = 0x%x\n", 
                                            pAd->HostLnkCtrlOffset, Configuration));
            }
            else
            {
                pAd->HostLnkCtrlOffset = 0;
                DBGPRINT(RT_DEBUG_ERROR, ("%s: cannot find PCI-to-PCI Bridge PCI Express Capability!\n", __FUNCTION__));
            }
        }
    }
    else
    {
        pAd->RLnkCtrlOffset = 0;
        pAd->HostLnkCtrlOffset = 0;
        DBGPRINT(RT_DEBUG_ERROR, ("%s: cannot find Ralink PCIe Device's PCI Express Capability!\n", __FUNCTION__));
    }

    if (bFindIntel == FALSE)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Doesn't find Intel PCI host controller. \n"));
		// Doesn't switch L0, L1, So set PCIePowerSaveLevel to 0xff, which means we doesn't
		pAd->PCIePowerSaveLevel = 0xff;
	}
}

VOID RTMPFindHostPCIDev(
    IN	PRTMP_ADAPTER	pAd)
{
    USHORT  reg16;
    UCHAR   reg8, DevFn;
    PPCI_DEV    pPci_dev;

    // Support advanced power save after 2892 . MAC version at offset 0x1000 is 2872(PCIe, USB, SDIO).
	//if ((pAd->MACVersion&0xffff0000) == 0x28600000)
	if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE))
		return;
    
    DBGPRINT(RT_DEBUG_TRACE, ("%s.===>\n", __FUNCTION__));

    pAd->parent_pci_dev = NULL;
    if (pAd->pci_dev->bus->parent)
    {
        for (DevFn = 0; DevFn < 255; DevFn++)
        {
#if 0 // 2880 does not support multiple card now
            pPci_dev = pci_get_slot(pAd->pci_dev->bus->parent, DevFn);
#else
			pPci_dev = NULL;
#endif
            if (pPci_dev)
            {                
                pci_read_config_word(pPci_dev, PCI_CLASS_DEVICE, &reg16);
                reg16 = le2cpu16(reg16);
                pci_read_config_byte(pPci_dev, PCI_CB_CARD_BUS, &reg8);
                if ((reg16 == PCI_CLASS_BRIDGE_PCI) && 
                    (reg8 == pAd->pci_dev->bus->number))
                {
                    pAd->parent_pci_dev = pPci_dev;
                }
            }
        }
    }
}

/*
	========================================================================
	
	Routine Description:

	Arguments:
		Level = RESTORE_HALT : Restore PCI host and Ralink PCIe Link Control field to its default value.
		Level = Other Value : Restore from dot11 power save or radio off status. And force PCI host Link Control fields to 0x1

	========================================================================
*/
VOID RTMPPCIeLinkCtrlValueRestore(
	IN	PRTMP_ADAPTER	pAd,
	IN   UCHAR		Level)
{
#if 1 // Linux embedded STA and Linux PC STA need stable more than power save.
	return;
#else // Code for advance power save is not stable now
	USHORT  PCIePowerSaveLevel, reg16;
	USHORT	Configuration;

	// Support advanced power save after 2892 . MAC version at offset 0x1000 is 2872(PCIe, USB, SDIO).
	//if ((pAd->MACVersion&0xffff0000) == 0x28600000)
	if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE))
		return;
    
	DBGPRINT(RT_DEBUG_TRACE, ("%s.===>\n", __FUNCTION__));
	PCIePowerSaveLevel = pAd->PCIePowerSaveLevel;
	if ((PCIePowerSaveLevel&0xff) == 0xff)
	{
		DBGPRINT(RT_DEBUG_TRACE,("return  \n"));
		return;
	}
    
    if (pAd->pci_dev && (pAd->RLnkCtrlOffset != 0))
    {           
        PCI_REG_READ_WORD(pAd->pci_dev, pAd->RLnkCtrlOffset, Configuration);
        if ((Configuration != 0) &&
            (Configuration != 0xFFFF))
        {
    		Configuration &= 0xfefc;
            if (Level == RESTORE_CLOSE)
            {
                Configuration |= pAd->RLnkCtrlConfiguration;
            }
    		else if (Level != RESTORE_HALT)
    		{
                // value with 0 is more stable than value with 1, 
                // but value with 1 is more power save than value with 0.    		
    			Configuration |= 0x0; //0x1;
                pAd->bPCIclkOffCommand = FALSE;
    		}
            PCI_REG_WIRTE_WORD(pAd->pci_dev, pAd->RLnkCtrlOffset, Configuration);
    		DBGPRINT(RT_DEBUG_TRACE, ("Restore Ralink : offset 0x%x = 0x%x\n", pAd->RLnkCtrlOffset, Configuration));
        }
        else
            DBGPRINT(RT_DEBUG_ERROR, ("Restore Ralink : PCI_REG_READ_WORD failed (Configuration = 0x%x)\n", Configuration));
	}

	if (pAd->parent_pci_dev && (pAd->HostLnkCtrlOffset != 0))
    {
        PCI_REG_READ_WORD(pAd->parent_pci_dev, pAd->HostLnkCtrlOffset, Configuration);
        if ((Configuration != 0) &&
            (Configuration != 0xFFFF))
        {
    		Configuration &= 0xfefc;
    		// If call from Halt, restore to default setting.
    		if (Level == RESTORE_CLOSE)
    		{
    			Configuration |= pAd->HostLnkCtrlConfiguration;
    		}
    		// Otherwise, like power save of radio off command, we set to 1. save more than 0.
    		else
                // value with 0 is more stable than value with 1, 
                // but value with 1 is more power save than value with 0.
    			Configuration |= 0x0; //0x1;
            PCI_REG_WIRTE_WORD(pAd->parent_pci_dev, pAd->HostLnkCtrlOffset, Configuration);
    		DBGPRINT(RT_DEBUG_TRACE, ("Restore PCI host : offset 0x%x = 0x%x\n", pAd->HostLnkCtrlOffset, Configuration));
        }
        else
            DBGPRINT(RT_DEBUG_ERROR, ("Restore PCI host : PCI_REG_READ_WORD failed (Configuration = 0x%x)\n", Configuration));
    }
    
	DBGPRINT(RT_DEBUG_TRACE,("%s <===\n", __FUNCTION__));
#endif
}

/*
	========================================================================
	
	Routine Description:

	Arguments:
		Max : limit Host PCI and Ralink PCIe device's LINK CONTROL field's value. 
		Because now frequently set our device to mode 1 or mode 3 will cause problem.
		
	========================================================================
*/
VOID RTMPPCIeLinkCtrlSetting(
	IN	PRTMP_ADAPTER	pAd,
	IN 	USHORT		Max)
{
#if 1 // Linux embedded STA and Linux PC STA need stable more than power save.
	return;
#else // Code for advance power save is not stable now
	USHORT  PCIePowerSaveLevel, reg16;
	USHORT	Configuration;

	// Support advanced power save after 2892 . MAC version at offset 0x1000 is 2872(PCIe, USB, SDIO).
	//if ((pAd->MACVersion&0xffff0000) == 0x28600000)
	if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE))
		return;

	DBGPRINT(RT_DEBUG_TRACE,("%s===>\n", __FUNCTION__));
	PCIePowerSaveLevel = pAd->PCIePowerSaveLevel;
	if ((PCIePowerSaveLevel&0xff) == 0xff)
	{
		DBGPRINT(RT_DEBUG_TRACE,("return  \n"));
		return;
	}
	PCIePowerSaveLevel = PCIePowerSaveLevel>>6;	    

	if (pAd->pci_dev && (pAd->RLnkCtrlOffset != 0))
	{
		// first 2892 chip not allow to frequently set mode 3. will cause hang problem.
		if (PCIePowerSaveLevel > Max)
			PCIePowerSaveLevel = Max;        

        PCI_REG_READ_WORD(pAd->pci_dev, pAd->RLnkCtrlOffset, Configuration);
		switch (PCIePowerSaveLevel)
		{
			case 0:
				// No PCI power safe
				// Set b0 and b1 of LinkControl (both 2892 and PCIe bridge) to 00 . 
				Configuration &= 0xfefc;
				break;
			case 1:
				//  L0
				// Set b0 and b1 of LinkControl (both 2892 and PCIe bridge) to 01 .
				Configuration &= 0xfefc;
				Configuration |= 0x1;
				break;
			case 2:
				// L0 and L1
				//  Set b0 and b1 of LinkControl (both 2892 and PCIe bridge) to 11 
				Configuration &= 0xfefc;
				Configuration |= 0x3;
				break;
			case 3:
				// L0 , L1 and clock management.
				// Set b0 and b1 of LinkControl (both 2892 and PCIe bridge) to 11 and bit 8 of LinkControl of 2892 to 1
				Configuration &= 0xfefc;
				Configuration |= 0x103;
				break;
		}
        PCI_REG_WIRTE_WORD(pAd->pci_dev, pAd->RLnkCtrlOffset, Configuration);
		DBGPRINT(RT_DEBUG_TRACE, ("Write Ralink device : offset 0x%x = 0x%x\n", pAd->RLnkCtrlOffset, Configuration));
	}

    // Skip non-exist deice right away
	if (pAd->parent_pci_dev && (pAd->HostLnkCtrlOffset != 0))
	{
        PCI_REG_READ_WORD(pAd->parent_pci_dev, pAd->HostLnkCtrlOffset, Configuration);
		switch (PCIePowerSaveLevel)
		{
			case 0:
				// Set b0 and b1 of LinkControl (both 2892 and PCIe bridge) to 00 
				Configuration &= 0xfefc;
				break;
			case 1:
				// Set b0 and b1 of LinkControl (both 2892 and PCIe bridge) to 01 
				Configuration &= 0xfefc;
				Configuration |= 0x1;
				break;
			case 2:
				//  Set b0 and b1 of LinkControl (both 2892 and PCIe bridge) to 11 
				Configuration &= 0xfefc;
				Configuration |= 0x3;
                pAd->bPCIclkOff = TRUE;
                pAd->bPCIclkOffCommand = TRUE;
				break;
			case 3:
				// Set b0 and b1 of LinkControl (both 2892 and PCIe bridge) to 11 and bit 8 of LinkControl of 2892 to 1
				Configuration &= 0xfefc;
				Configuration |= 0x103;				
                pAd->bPCIclkOff = TRUE;
                pAd->bPCIclkOffCommand = TRUE;
				break;
		}
        PCI_REG_WIRTE_WORD(pAd->parent_pci_dev, pAd->HostLnkCtrlOffset, Configuration);
		DBGPRINT(RT_DEBUG_TRACE, ("Write PCI host offset 0x%x = 0x%x\n", pAd->HostLnkCtrlOffset, Configuration));

	}
	DBGPRINT(RT_DEBUG_TRACE,("RTMPPCIePowerLinkCtrl <==============\n"));
#endif
}
#endif // CONFIG_STA_SUPPORT //

VOID rt2860_stop(struct net_device *net_dev)
{
    PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)NULL;
    if (net_dev == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("net_dev == NULL!\n"));
	}
	else
		pAd = (PRTMP_ADAPTER)net_dev->priv;
 
	if (pAd != NULL)
	{
	    // stop interface
		netif_carrier_off(net_dev);
		netif_stop_queue(net_dev);

		// mark device as removed from system and therefore no longer available
		netif_device_detach(net_dev);

		// mark halt flag
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);

		// take down the device
		rt28xx_close(net_dev);
	}
    return;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
static int rt2860_shutdown(
	struct pci_dev *pci_dev)
{
	struct net_device *net_dev = pci_get_drvdata(pci_dev);

	DBGPRINT(RT_DEBUG_TRACE, ("===> %s\n", __FUNCTION__));

	rt2860_stop(net_dev);
	DBGPRINT(RT_DEBUG_TRACE, ("<=== %s\n", __FUNCTION__));
    
    pci_disable_device(pci_dev);
    
	return 0;
}
#endif

