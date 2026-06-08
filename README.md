# Keychron Q7 OpenRGB Toggle Firmware

This is a modified firmware for Keychron Q7 keyboard to add support for OpenRGB firmware
Tested and working fine, I havent found any issues, the rest of the keyboard is as per 

Essentially this is the default Q7 firmware with added rules :
VIA_ENABLE = yes
OPENRGB_ENABLE = yes
RGB_MATRIX_KEYREACTIVE_ENABLED = yes

# Flashing Instructions
make keychron/q7/ansi:default
Flashing example for this keyboard (after setting up the bootloadHID flashing environment)
make keychron/q7/ansi:default:flash

Reset Key: Hold down the key located at K00, commonly programmed as holding 'Esc' while plugging in the keyboard
