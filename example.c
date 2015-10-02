#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib_dataq.h"

#define DATA_BUFFER_SIZE 255

int main(int argc, char** argv) {

	int fd = -1;
	char data[DATA_BUFFER_SIZE];

	printf("Opening DATAQ data logger serial device\n");

	fd = open_dev("/dev/ttyACM0", true);

	if(fd == -1) {
		perror("Unable to open device...exiting\n");
		exit(1);
	}
	
	if(vendor(fd, data, DATA_BUFFER_SIZE) > 0) {
		printf("%s\n", data);
	}
	else {
		printf("Error occured while reading vendor name");
	}
	
	if(model(fd, data, DATA_BUFFER_SIZE) > 0) {
		printf("%s\n", data);
	}
	else {
		printf("Error occured while reading model name");
	}

	if(firmware(fd, data, DATA_BUFFER_SIZE) > 0) {
		printf("Firmware version %s\n", data);
	}
	else {
		printf("Error occured while reading firmware version");
	}

	if(serial_number(fd, data, DATA_BUFFER_SIZE) > 0) {
		printf("Serial number %s\n", data);
	}
	else {
		printf("Error occured while reading serial number");
	}
	
	close(fd);

	return 0;
}

