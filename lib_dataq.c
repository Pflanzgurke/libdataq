/*! \file */
#include <unistd.h>
#include "lib_dataq.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

/*! Opens and initially configures the data logger device.
 *
 * must be given device name as input parameter
 * e.g. /dev/ttyACM0
 *
 * the block parameter is used make I/O blocking or non-blocking
 *
 * returns file descriptor or on failure whatever
 * open() returns on failure (-1)
 */

int dataq_open_dev(char *dev_name, dataq_conf *conf)
{
	if (dev_name == NULL || conf == NULL) {
		return -1;
	}

	int fd = -1;

	fd = open(dev_name, O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd != -1) {
		fcntl(fd, F_SETFL, FNDELAY);
		dataq_apply_config(fd, conf);
	}

	return fd;
}

int dataq_init_dev(int fd)
{
	if (fd < 0) {
		return -1;
	}

	//send "stop" to the device, just to make sure
	if (dataq_stop(fd) <= 0) {
		return -1;
	}

	//and read out what's left in the buffer, if anything

	//reset the scan list

	//set the min rate
}

int dataq_apply_config(int fd, dataq_conf *conf)
{
	int success = 0;

	if (dataq_set_mode(fd, conf->mode, conf) > 0) {
		++success;
	}

	if (dataq_set_rate(fd, conf->rate, conf) > 0) {
		++success;
	}

	return success;
}

/*! Initializes all members of the dataq_conf parameter to 0.
 *
 * Insd,nfsd.,nv sddsfkgj sdlvn sdkjfbsd.kjf sdkjfb sdkjfbvsdkfnv sdkjfbhs
 * srgjkhsd.kjh s.dfklgh sdbjkfn dkbj:sdfbi.
 *
 * param conf the config to be initialized
 */
void dataq_init_config(dataq_conf *conf)
{
	memset(conf, 0, sizeof(dataq_conf));

	//these are the default
	conf->mode = DATAQ_MODE_ASC;
	conf->rate = 75;

	conf->slist[0] = 0;
	
	for (int n = 1; n < DATAQ_NUM_PORTS; n++) {
		conf->slist[n] = -1;	
	}	
}

long __dataq_send_command(int fd, char *cmd, char *result, unsigned int size)
{
	if (fd < 0 || cmd == NULL || size <= 0) {
		return -1;
	}

	long res = 0;
	int total_bytes = 0;

	res = write(fd, cmd, strlen(cmd));

	//TODO: need to handle the case where res != strlen(cmd), i.e. the
	//full command has not been written
	if (res < 0) {
		return res;
	}

	fd_set read_set;
	struct timeval timeout;

	memset(&timeout, 0, sizeof(timeout));
	timeout.tv_sec = 3;

	FD_ZERO(&read_set);
	FD_SET(fd, &read_set);

	res = select(fd + 1, &read_set, NULL, NULL, &timeout);

	if (res <= 0) {
		return -1;
	}

	char *tmp_str = malloc(sizeof(char) * (unsigned long)size);

	while ((res = read(fd, tmp_str + total_bytes, size - (unsigned int) total_bytes - 1)) > 0) {
		total_bytes +=  res;

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

long dataq_set_mode(int fd, const int mode, dataq_conf *conf)
{
	if (fd < 0 || conf == NULL) {
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
	res = __dataq_send_command(fd, data, data, size);

	if (res > 0) {
		conf->mode = mode;
	}

	free(data);

	return res;
}


long dataq_set_rate(int fd, unsigned int rate, dataq_conf *conf)
{

	if (rate > DATAQ_MAX_RATE || rate < DATAQ_MIN_RATE || fd < 0) {
		return -1;
	}

	long res = -1;
	char data[13]; //"srate" + " " + "10000" (max) + "\r" + "\0"
	unsigned int size = 0;

	//need to convert the rate in Hz to the data logger internal
	//rate

	unsigned int dataq_rate = 750000 / rate;

	snprintf(data, 13, "%s %d%s", DATAQ_CMD_SCAN_RATE_PREFIX, rate, DATAQ_EOC);
	printf("%s\n", data);

	res = __dataq_send_command(fd, data, data, size);

	if (res > 0) {
		conf->rate = rate;
	}

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

int dataq_slist_add(int fd, unsigned int port, dataq_conf *conf)
{
	if (fd < 0 || port >= DATAQ_NUM_PORTS || conf == NULL) {
		return -1; //bad parameter
	}

	for (int n = 0; n < DATAQ_NUM_PORTS; n++) {
		if (conf->slist[n] == port) {
			return 0; //port already in the list
		}

		if (conf->slist[n] < 0 ) {
			int res = 0;
			char cmd[32];
			snprintf(cmd, 32, "%s %d x%4d%s", DATAQ_CMD_SCAN_LIST_PREFIX, n, port, DATAQ_EOC); 

		 	res = __dataq_send_command(fd, cmd, cmd, strlen(cmd));
			
			if (res > 0) {
				conf->slist[n] = port; //set port
			}
			return res;
		}
	}

	return -1; //non of the above --> error
}

int dataq_slist_remove(int fd, unsigned int port, dataq_conf *conf) 
{
	if (fd < 0 || port >= DATAQ_NUM_PORTS || conf == NULL) {
		return -1;
	}

	//for (int n = 0; n < DATAQ_NUM_PORTS; n++) {
	//	if (conf->
	//}

	return -1;
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
	long res = __dataq_send_command(fd, DATAQ_CMD_STOP, data, size);
	
	printf("%s\n", data);
	return res;
}



