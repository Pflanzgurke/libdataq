#ifndef LIB_DATAQ_H
#define LIB_DATAQ_H

#include <stdbool.h>
#include <fcntl.h>
#include <string.h>

#define DATAQ_CMD_VENDOR "info 0\r"
#define DATAQ_CMD_MODEL "info 1\r"
#define DATAQ_CMD_FIRMWARE "info 2\r"
#define DATAQ_CMD_SERIAL_NUM "info 6\r"

#define DATAQ_CMD_SCAN_LIST_PREFIX "slist"
#define DATAQ_CMD_SCAN_RATE_PREFIX "srate"

#define DATAQ_CMD_ASC "asc\r"
#define DATAQ_CMD_BIN "bin\r"
#define DATAQ_CMD_FLOAT "float\r"
#define DATAQ_CMD_START "start\r"
#define DATAQ_CMD_STOP "stop\r"
#define DATAQ_CMD_RESET_COUNTER "reset 1\r"

#define DATAQ_MODE_ASC 1
#define DATAQ_MODE_BIN 2
#define DATAQ_MODE_FLOAT 3

#define DATAQ_EOC "\r" //end of command



long __dataq_send_command(int fd, char *cmd, char *result, unsigned int size);

int dataq_open_dev(char *dev_name, bool block);
long dataq_set_mode(int fd, int mode);
long dataq_reset_counter(int fd);

long dataq_model(int fd, char *model_name, unsigned int size);
long dataq_vendor(int fd, char *vendor_name, unsigned int size);
long dataq_firmware(int fd, char *firmware_version, unsigned int size);
long dataq_serial_number(int fd, char *serial_number, unsigned int size);

long dataq_start(int fd);
long dataq_stop(int fd);


#endif
