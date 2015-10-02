

Tested with a Raspberry 2

1. changed or add the following to /boot/config.txt

max_usb_current=1

this will allow you to drive external HW with max 1.2A. The Dataq 149 datasheet
says that the data logger draws less than 1A, so that should be sufficient
For more config options on the Pi see:

http://elinux.org/RPiconfig

