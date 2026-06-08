# Keychron Q7 OpenRGB Toggle Firmware

This is a modified firmware for Keychron Q7 keyboard to add support for OpenRGB firmware
Tested and working fine, I haven't found any issues.
Essentially this is just a default Q7 firmware with added rules to enable OpenRGB support as follows :

VIA_ENABLE = yes

OPENRGB_ENABLE = yes

RGB_MATRIX_KEYREACTIVE_ENABLED = yes


# Flashing Instructions
make keychron/q7/ansi:default
Flashing example for this keyboard (after setting up the bootloadHID flashing environment)

make keychron/q7/ansi:default:flash

Reset Key :
Hold down the key located at K00, commonly programmed as holding 'Esc' while plugging in the keyboard
