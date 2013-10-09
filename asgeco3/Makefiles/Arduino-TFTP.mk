#
# embedXcode
# ----------------------------------
# Embedded Computing on Xcode 4
#
# Copyright © Rei VILO, 2010-2013
# Licence CC = BY NC SA
#

# References and contribution
# ----------------------------------
# See About folder
#


# Arduino specifics
# ----------------------------------
# Automatic 0023 or 1. selection based on version.txt
#
include $(MAKEFILE_PATH)/Arduino1.mk

PLATFORM_TAG     := ARDUINO=105

BOARDS_TXT       := ./hardware/arduino/boards.txt

UPLOADER          = tftp
TFTP_PATH         =
TFTP_REBOOT       =

OBJCOPYFLAGS      = -j .text -j .data -O binary
