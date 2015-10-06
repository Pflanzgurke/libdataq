#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib_dataq.h"

#define DATA_BUFFER_SIZE 255

int main()
{

	int fd = -1;
	char data[DATA_BUFFER_SIZE];

	printf("Opening DATAQ data logger serial device\n");

	fd = dataq_open_dev("/dev/ttyACM0", true);

	if (fd == -1) {
		perror("Unable to open device...exiting\n");
		exit(1);
	}

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

	close(fd);

	return 0;
}

