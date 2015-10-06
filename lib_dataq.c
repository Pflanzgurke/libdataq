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

int dataq_open_dev(char *dev_name, bool block)
{

	if (dev_name == NULL) {
		return -1;
	}

	int fd = -1;

	fd = open(dev_name, O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd != -1) {
		dataq_set_mode(fd, DATAQ_MODE_ASC);

		if (block) {
			fcntl(fd, F_SETFL, 0);
		} else {
			fcntl(fd, F_SETFL, FNDELAY);
		}
	}

	return fd;
}

long __dataq_send_command(int fd, char *cmd, char *result, unsigned int size)
{

	if (fd < 0 || cmd == NULL || size <= 0) {
		return -1;
	}

	long bytes = 0;
	int total_bytes = 0;

	bytes = write(fd, cmd, strlen(cmd));

	//TODO: need to handle the case where bytes != strlen(cmd), i.e. the
	//full command has not been written
	if (bytes < 0) {
		return bytes;
	}

	char *tmp_str = malloc(sizeof(char) * (unsigned long)size);

	while ((bytes = read(fd, tmp_str + total_bytes, size - (unsigned int) total_bytes - 1)) > 0) {
		total_bytes +=  bytes;

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

long dataq_set_mode(int fd, const int mode)
{
	if (fd < 0) {
		return -1;
	}

	unsigned int size = 0;
	char *data = NULL;

	switch (mode) {
	case DATAQ_MODE_ASC:
		size = strlen(DATAQ_CMD_ASC);
		data = malloc(sizeof(char) * size);
		break;

	case DATAQ_MODE_BIN:
		size = strlen(DATAQ_CMD_BIN);
		data = malloc(sizeof(char) * size);
		break;

	case DATAQ_MODE_FLOAT:
		size = strlen(DATAQ_CMD_FLOAT);
		data = malloc(sizeof(char) * size);
		break;

	default:
		return -1;
	}

	long res = -1;
	//potential TODO: look at the returned string
	res = __dataq_send_command(fd, data, data, size);
	free(data);

	return res;
}

long dataq_reset_counter(int fd)
{
	if (fd < 0) {
		return -1;
	}

	unsigned int size = strlen(DATAQ_CMD_RESET_COUNTER);
	char data[size];

	//potential TODO: look at the returned string
	return (__dataq_send_command(fd, DATAQ_CMD_RESET_COUNTER, data, size));

}


long dataq_model(int fd, char *model_name, unsigned int size)
{
	if (fd < 0 || model_name == NULL || size == 0) {
		return - 1;
	}

	return (__dataq_send_command(fd, DATAQ_CMD_MODEL, model_name, size));
}

long dataq_vendor(int fd, char *vendor_name, unsigned int size)
{
	if (fd < 0 || vendor_name == NULL || size == 0) {
		return -1;
	}

	return (__dataq_send_command(fd, DATAQ_CMD_VENDOR, vendor_name, size));
}

long dataq_firmware(int fd, char *firmware_version, unsigned int size)
{
	if (fd < 0 || firmware_version == NULL || size == 0) {
		return -1;
	}

	//potential TODO: turn hex firmware version into decimal firmware version
	return (__dataq_send_command(fd, DATAQ_CMD_FIRMWARE, firmware_version, size));
}

long dataq_serial_number(int fd, char *serial_number, unsigned int size)
{
	if (fd < 0 || serial_number == NULL || size == 0) {
		return -1;
	}

	return (__dataq_send_command(fd, DATAQ_CMD_SERIAL_NUM, serial_number, size));
}

long dataq_start(int fd)
{
	if (fd < 0) {
		return -1;
	}

	unsigned int size = strlen(DATAQ_CMD_START);
	char data[size];

	//potential TODO: look at the returned string
	return (__dataq_send_command(fd, DATAQ_CMD_START, data, size));
}

long dataq_stop(int fd)
{
	if (fd < 0) {
		return -1;
	}

	unsigned int size = strlen(DATAQ_CMD_STOP);
	char data[size];

	//potential TODO: look at the returned string
	return (__dataq_send_command(fd, DATAQ_CMD_STOP, data, size));
}

