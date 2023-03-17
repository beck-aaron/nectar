#ifndef _DMA_H_
#define _DMA_H_

#include <xdmac.h>
#include <logger.h>

#define XDMAC_NUM_CHANNELS 24

void xdmac_configure_peripheral_to_memory(Usart *usart, uint8_t hwid, uint8_t *memory, size_t length, uint32_t channel_num);
void xdmac_enable_peripheral_to_memory(uint32_t channel_num);
void xdmac_disable_peripheral_to_memory(uint32_t channel_num);
void xdmac_suspend_channel(uint32_t channel_num);

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
