# Write down notes on USART/UART drivers for the next guy...

### Make sure to put device protocols in device folder!

- Header file should contain standard device struct, see XBEE.h for details
- TODO: write up standard for device structs
- this is useful for anyone trying to understand the project
- explain tree view of filestructure, with examples for each part of the project

# Usage
When the device driver has been implemented, add its initialization function
to the "devices_init()" function in the devices.c file.
Make sure to set the device state if it is CONNECTED after the init function
or if it is DISCONNECTED. This is needed for the nectar driver to determine
which values to use in the bitmap.  For any new sensors, add the corresponding
data points to the nectar subpayload.
