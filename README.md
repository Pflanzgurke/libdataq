libdataq
========

<i>Please note: this lib is work in progress and incomplete! It is not a product of
DATAQ nor is it in any way endorsed by or affiliated with DATAQ.</i>

A library to interface with [DATAQ](http://www.dataq.com/) usb data loggers implementing
the [DATAQ protocol](http://www.dataq.com/resources/pdfs/misc/di-149-protocol.pdf).  

The library has been tested with a Raspberry 2, a [DATAQ 149](http://www.dataq.com/products/di-149/) and an 
[E+E 80 Sensor](http://www.epluse.com/en/products/co2-measurement/co2-carbon-dioxide-transmitters/ee80/).

Test setup
==========

Here are some steps you should follow if you want to reproduce the Raspberry 2 and DATAQ 149 
test setup used during development of libdataq. The Pi had Raspian with a 3.18 kernel install.

1. Change/add the following to /boot/config.txt on your Raspberry 2

max_usb_current=1

this will allow you to drive external HW with max 1.2A. The Dataq 149 datasheet
says that the data logger draws less than 1A, so that should be sufficient
For more config options on the Pi see: http://elinux.org/RPiconfig

