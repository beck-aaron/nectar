# Notes on USART/UART drivers

### Make sure to put device protocols in device folder!

- header file should contain standard device struct, see xbee.h for details
- each driver should implement functions like "transmit" and "receive" which can be called to send messages to the sensor from the application

### Usage
When the device driver has been implemented, add its initialization function
to the "devices_init()" function in the devices.c file.
Make sure to set the device state if it is CONNECTED after the init function
or if it is DISCONNECTED. This is needed for the nectar driver to determine
which values to use in the bitmap.  For any new sensors, add the corresponding
data points to the nectar subpayload. Currently, the bitmap is set manually but
should be determined by the amount of sensors that are confirmed to be CONNECTED.

"src/config/conf_uart_serial.h" is an important file. This contains definitions
for integrating the UART peripheral properties with the driver.  Refer to the other
implementations for more information.
