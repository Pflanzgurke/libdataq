#include <unistd.h>
#include "lib_dataq.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Opens the data logger device 
 * 
 * must be given device name as input parameter
 * e.g. /dev/ttyACM0
 *
 * the block parameter is used make I/O blocking or non-blocking 
 * 
 * returns file descriptor or on failure whatever
 * open() returns on failure (-1) 
 */

int open_dev(char* dev_name, bool block) {

	int fd = -1;

	fd = open(dev_name, O_RDWR | O_NOCTTY | O_NDELAY);

	if(fd != -1 && block) {
		fcntl(fd, F_SETFL, 0);
	}

	if(fd != -1 && !block) {
		fcntl(fd, F_SETFL, FNDELAY);
	}

	return fd;	
}

int send_command(int fd, char* cmd, char* result, unsigned int size) {
	int bytes = 0;
	int total_bytes = 0;

	char* tmp_str = malloc(sizeof(char) * size);

	bytes = write(fd, cmd, strlen(cmd)); 
	
	//TODO: need to handle the case where bytes != strlen(cmd), i.e. the
	//full command has not been written
	if(bytes < 0) {
		return bytes;
	}
	
	while ((bytes = read(fd, tmp_str + total_bytes, size - total_bytes - 1)) > 0)
	{
		total_bytes += bytes;	
		
		if (tmp_str[total_bytes - 1] == '\r' || tmp_str[total_bytes - 1] == '\n') {
			break;
		}

	}

	tmp_str[total_bytes] = '\0';
	
	//now we remove the command, which is echoed in every response
	strncpy(result, tmp_str + strlen(cmd), size);

	free(tmp_str);
	//returns -1 on error
	return total_bytes;

}

int model(int fd, char* model_name, unsigned int size) {
	return(send_command(fd, DATAQ_CMD_MODEL, model_name, size)); 
}

int vendor(int fd, char* vendor_name, unsigned int size) {
	return(send_command(fd, DATAQ_CMD_VENDOR, vendor_name, size)); 
}

int firmware(int fd, char* firmware_version, unsigned int size) {
	//potential TODO for the future: turn hex firmware version
	//into decimal firmware version
	return(send_command(fd, DATAQ_CMD_FIRMWARE, firmware_version, size));
}

int serial_number(int fd, char* serial_number, unsigned int size) {
	return(send_command(fd, DATAQ_CMD_SERIAL_NUM, serial_number, size));
}
