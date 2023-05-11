# Notes for XDMAC controller

Steps to use for data reception.
use the conf_uart_serial to set device specific channels macro based
on peripheral that it uses (e.g. XBEE_XDMAC_CHANNEL -> XDMAC_CHANNEL_HWID_UART1_TX)

we should initialize these channels during the device_init() function, more
specifically in the devices own intialization function.

NOTE: In general, the chunk size is set as ‘1 data’ in most of the peripherals (example: - UART, SPI, TWI, etc.), as the maximum size of their receive register is ‘1 data’


CACHE COHERENCY
If Data Cache is enabled:
- 'Clean D-Cache Operation' should be performed before the XDMAC transfer
- 'Invalidate D-Cache Operation' should be performed after the XDMAC transfer


SINGLE BLOCK
A basic single block of DMA transfer can be done by just configuring XDMAC
channel registers directly. After configuring XDMAC channel registers, the
corresponding channel needs to be enabled. This will trigger the data
transaction, if it is a memory to memory transfer. The peripheral synchronized
transfer will still wait for the hardware or software trigger to occur.
Refer to the device datasheet for more information on initialization sequence
and software flow.

The channel configuration parameters remain same throughout the block
transfer. They cannot be modified between microblocks.

(example: source/destination addresses, block/microblock length, stride length, etc.)

MULTI-BLOCK
Multi block memory transfer is needed when there is a change in XDMAC
channel configuration parameters (example: source/destination addresses,
block/microblock length, stride length, etc.) between blocks transferred in
the same channel

XDMAC usage

1. initialize source and destination buffer
    (we are using peripheral as source - UART - and memory as destination)

2. select a free channel using the XDMAC_GS register

    xdmac_channel_get_status(xdmac) 

    returns a bitmask of all the available
    registers, we can write a function to get the first available channel.
    A channel is disabled when its corresponding bit is zero and enabled
    when set to one.

3. clear pending interrupt status register by reading channel x interrupt status register
    ex. XDMAC_CISx

    xdmac_disable_interrupt(xdmac, channel_x);

Note: we can write all our data to a struct from xdmac driver from ASF
        there is a function to set these configurations as well.

4. write source address to XDMAC_CSAx

5. write source address to XDMAC_CDAx

6. write XDMAC_CCx configurable parameteres to register

7. write XDMAC_CUBCx.UBLEN

8. Program XDMAC_CBCx.BLEN with the number of microblocks of data.

Clear the following 5 registers for a single microblock transfer
– XDMAC Channel x Next Descriptor Control Register (XDMAC_CNDCx)
– XDMAC Channel x Block Control Register (XDMAC_CBCx)
– XDMAC Channel x Data Stride Memory Set Pattern Register (XDMAC_CDS_MSPx)
– XDMAC Channel x Source Microblock Stride Register (XDMAC_CSUSx)
– XDMAC Channel x Destination Microblock Stride Register (XDMAC_CDUSx)
This indicates that the linked list is disabled, there is only one block and striding is disabled.

Clear the following 3 registers for multiple microblock transfer
– XDMAC_CNDCx               - Next Descriptor Control Register
– XDMAC_CDS_MSPx            - Data Stride Memory Set Pattern Register
– XDMAC_CSUSx XDMAC_CDUSx   - Destination Microblock Stride Register

9. enable interrupt using 
    xdmac_enable_interrupt(xdmac, channel_x);

10. enable channel
    //if using cache, update DCache before transit
    SCB_CleanInvalidateDCache()
    xdmac_enable_channel(xdmac, channel_x);

FINISHED:

    Once completed, the DMA channel sets XDMAC_CISx.BIS 
    (End of Block Interrupt Status bit) and generates an interrupt.

    XDMAC_GS.STx is cleared by hardware. 

    The software can either wait for an interrupt
    or poll the channel status bit.

QUESTIONS:

    Q: lets say we want to transfer data from IO to memory after making a
    request.  We do not know how large the data sent will be, although
    we do have a limit. Will the transfer complete even though the device
    stops sending memory that will be under the microblock count?

    A: After testing put response here...
