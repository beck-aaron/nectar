/*******************************************************************************
 * File:    dma.h
 * Author:  Aaron Beck
 *
 * Description:
 *
 ******************************************************************************/
#ifndef _DMA_H_
#define _DMA_H_

#include <xdmac.h>

#define XDMAC_NUM_CHANNELS 24

inline static size_t xdmac_get_bytes_transferred(size_t ublock_size, uint32_t channel_num)
{
    return ublock_size - XDMAC->XDMAC_CHID[channel_num].XDMAC_CUBC;
}

inline static void xdmac_flush_channel(uint32_t channel_num)
{
    XDMAC->XDMAC_GSWF = (XDMAC_GSWF_SWF0 << channel_num);
}

inline static void xdmac_configure_peripheral_to_memory(Usart *usart, uint8_t hwid, uint8_t *memory, size_t length, uint32_t channel_num)
{
    xdmac_channel_config_t xdmac_channel_config = 
    {
        .mbr_ubc = XDMAC_CUBC_UBLEN(length),    // microblock length
        .mbr_sa = (uint32_t)&(usart->US_RHR),   // source address
        .mbr_da = (uint32_t)memory,             // destination address

        .mbr_cfg = XDMAC_CC_TYPE_PER_TRAN       | // peripheral transfer
                   XDMAC_CC_MBSIZE_SINGLE       | // memory burst length
                   XDMAC_CC_DSYNC_PER2MEM       | // peripheral to memory
                   XDMAC_CC_SWREQ_HWR_CONNECTED | // hardware request line
                   XDMAC_CC_MEMSET_NORMAL_MODE  | // memset not activated
                   XDMAC_CC_CSIZE_CHK_1         | // chunk size is 1 data 
                   XDMAC_CC_DWIDTH_BYTE         | // data width
                   XDMAC_CC_SIF_AHB_IF1         | // read bus interface 1
                   XDMAC_CC_DIF_AHB_IF0         | // write bus interface 0
                   XDMAC_CC_SAM_FIXED_AM        | // fixed source address
                   XDMAC_CC_DAM_INCREMENTED_AM  | // incremented destination address
                   XDMAC_CC_PERID(hwid),          // hardware request line id

        // indicate the linked list is disabled for single block transfer
        .mbr_bc  = 0, // clear block control register
        .mbr_ds  = 0, // clear next descriptor control register
        .mbr_sus = 0, // clear source microblock stride register
        .mbr_dus = 0, // clear destination microblock stride register
    };

    xdmac_configure_transfer(XDMAC, channel_num, &xdmac_channel_config);

    xdmac_channel_enable_interrupt(
         XDMAC,             // xdmac peripheral address
         channel_num,       // channel number
         XDMAC_CIE_BIE  |   // block end interrupt
         XDMAC_CIE_FIE  |   // end of flush interrupt
         XDMAC_CIE_RBIE |   // read bus error interrupt
         XDMAC_CIE_WBIE |   // write bus error interrupt
         XDMAC_CIE_ROIE     // request overflow interrupt
    );

    xdmac_enable_interrupt(XDMAC, channel_num);
}

inline static void xdmac_configure_memory_to_peripheral(uint8_t* memory, size_t length, Usart* usart, uint8_t hwid, uint32_t channel_num)
{
    xdmac_channel_config_t xdmac_channel_config = 
    {
        .mbr_ubc = XDMAC_CUBC_UBLEN(length),    // set microblock length to xbee header length
        .mbr_sa  = (uint32_t)memory,            // source address
        .mbr_da  = (uint32_t)&(usart->US_RHR),  // destination address

        .mbr_cfg = XDMAC_CC_TYPE_PER_TRAN       | // peripheral transfer
                   XDMAC_CC_MBSIZE_SINGLE       | // memory burst length
                   XDMAC_CC_DSYNC_MEM2PER       | // memory to peripheral
                   XDMAC_CC_SWREQ_HWR_CONNECTED | // hardware request line
                   XDMAC_CC_MEMSET_NORMAL_MODE  | // memset not activated
                   XDMAC_CC_CSIZE_CHK_1         | // chunk size is 1 data 
                   XDMAC_CC_DWIDTH_BYTE         | // data width
                   XDMAC_CC_SIF_AHB_IF0         | // read bus interface 0
                   XDMAC_CC_DIF_AHB_IF1         | // write bus interface 1
                   XDMAC_CC_SAM_INCREMENTED_AM  | // incremented source address
                   XDMAC_CC_DAM_FIXED_AM        | // fixed destination address
                   XDMAC_CC_PERID(hwid),          // hardware request line id

        // indicate the linked list is disabled for single block transfer
        .mbr_bc  = 0, // clear block control register
        .mbr_ds  = 0, // clear next descriptor control register
        .mbr_sus = 0, // clear source microblock stride register
        .mbr_dus = 0, // clear destination microblock stride register
    };

    xdmac_configure_transfer(XDMAC, channel_num, &xdmac_channel_config);

    xdmac_channel_enable_interrupt(
         XDMAC,             // xdmac peripheral address
         channel_num,       // channel number
         XDMAC_CIE_BIE  |   // block end interrupt
         XDMAC_CIE_FIE  |   // end of flush interrupt
         XDMAC_CIE_RBIE |   // read bus error interrupt
         XDMAC_CIE_WBIE |   // write bus error interrupt
         XDMAC_CIE_ROIE     // request overflow interrupt
    );

    xdmac_enable_interrupt(XDMAC, channel_num);
}

#endif /* _DMA_H_ */

// hold this here for now, we might want to use the xdmac interrupt later
/**
 * @brief Interrupt Handler for XDMAC (direct memory access) peripheral
 *
 * When this triggers we need to determine which channel is interrupting,
 * and what interrupts are pending on that channel.
 *
 * The xdmac contains 24 channels, each of which can have up to 7 interrupts
 * enabled.  This program currently uses only 5 interrupts at this time.
 *
 * XDMAC_CIS_BIS: end of block interrupt, set when the block is finished transferring
 * XDMAC_CIS_FIS: end of flush interrupt, set when the FIFO is flushed completely (need to determine if this includes all data)
 * XDMAC_CIS_RBEIS: read bus error interrupt.  check SIF_AHB_IFx and DIF_AHB_IFx, try switching the ones and zeros.
 * XDMAC_CIS_WBEIS: write bus error interrupt.  check SIF_AHB_IFx and DIF_AHB_IFx, try switching the ones and zeros.
 *
 */
/*
ISR(XDMAC_Handler)
{
    uint32_t channel_status;
    channel_status = xdmac_get_interrupt_status(XDMAC);

    uint32_t status;
    for (uint8_t i = 0; i < XDMAC_NUM_CHANNELS; ++i, channel_status >>= 1)
    {
        if (!(channel_status & 0x1)) // channel has no interrupt pending
            continue;

        status = xdmac_channel_get_interrupt_status(XDMAC, i);

        if (i == XBEE_CHANNEL_RX)
        {
            if (status & XDMAC_CIS_BIS)
            {
                printf("        end of block interrupt         ");
            }

            if (status & XDMAC_CIS_FIS)
            {
                printf("        end of flush interrupt         ");
            }
        }
    }
}
*/
