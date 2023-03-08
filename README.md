# Nectar Driver - Developer's Guide

#### Note:

    Building and flashing instructions are for the SAME70 Xplained evaluation
    board. Refer to the PTLS README for flashing on the PTLS board.
#### Note:

    Currently this has only been tested on macOS.  For windows specific instructions,
    please add them to this file when you figure that out.  In the meantime,
    wsl2 should work.

___

## Setup Environment

1. Install Dependencies
2. Clone repository
3. Build firmware
4. Flash to device
___

### Dependencies

    - openocd                       // https://github.com/openocd-org/openocd
    - picocom                       // https://github.com/npat-efault/picocom
    - bear                          // needed to generating compile_commands.json for clangd
                                    //   if you don't know what clangd is, you don't need bear.
    - microchip arm-gcc toolchain

#### Install Packages
    $ sudo apt install openocd picocom          // debian/ubuntu
    $ brew install openocd picocom              // macOS

#### Install Microchip ARM-GCC toolchain

Download the release for your operating system:

    https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers_

Install by extracting to the directory of your choice.  This install directory is needed when building.

---

### Clone Repository

    $ git clone https://github.com/beck-aaron/nectar.git

___

### Build Instructions
    1. cd to root directory of repo
    2. Connect micro-usb from SAME70 Xplained to computer before uploading firmware.
    3. Edit makefile GCCLIB variable to the directory where you installed the arm-gcc toolchain.

    $ make -j                       // builds ELF file for programming MCU
    $ make upload                   // builds ELF file and uses openocd to program SAME70 flash
    $ make clean                    // removes build artifacts
    $ make bear                     // used for generating compile_commands.json


### Viewing the Logger
    
    Ensure picocom is installed.  For more info not listed here, see the manpage.

    // ABCD will be a random number, you should be able to TAB to autocomplete
    $ picocom --quiet --baud 115200 /dev/tty.usbmodemABCD

    To pipe the output to a logfile, use --logfile "filepath.log"

    ----------------------------------------------------------------------------

    Within picocom you can use several shortcut commands, but the key ones are:

    CTR-A CTR-X   - exits picocom and closes serial port
    CTR-A CTR-H   - displays a list of all shortcuts
    CTR-A CTR-S   - send a file with the program configured by --send-cmd

    To clear the screen of an output, use CMD-K on a mac (WINDOWS-K on windows?)
___
