#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib_dataq.h"

#define DATA_BUFFER_SIZE 255

int main()
{

	int fd = -1;
	char data[DATA_BUFFER_SIZE];

	dataq_conf conf;

	// Before opening the data logger device, always init the config!
	// You can do this manually, but why would you... use dataq_init_config()
	// instead
	dataq_init_config(&conf);

	printf("Opening DATAQ data logger serial device\n");

	// Opening the device... using the config in conf
	
	fd = dataq_open_dev("/dev/ttyACM0", &conf); 
	if (fd < 0) {
		perror("Unable to open device...exiting\n");
		exit(1);
	}

	if (dataq_init_dev(fd) < 0) {
		printf("Failed to init the device\n");
		exit(1);
	}

	// The next couple of functions read out the data logger information

	if (dataq_vendor(fd, data, DATA_BUFFER_SIZE) > 0) {
		printf("%s\n", data);
	} else {
		printf("Error occured while reading vendor name");
	}

	if (dataq_model(fd, data, DATA_BUFFER_SIZE) > 0) {
		printf("%s\n", data);
	} else {
		printf("Error occured while reading model name");
	}

	if (dataq_firmware(fd, data, DATA_BUFFER_SIZE) > 0) {
		printf("Firmware version %s\n", data);
	} else {
		printf("Error occured while reading firmware version");
	}

	if (dataq_serial_number(fd, data, DATA_BUFFER_SIZE) > 0) {
		printf("Serial number %s\n", data);
	} else {
		printf("Error occured while reading serial number");
	}

	// Now we configure the scan list of the logger...i.e. the list of
	// inputs to sample.



	close(fd);

	return 0;
}

