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

#define DATAQ_MODE_ASC 1
#define DATAQ_MODE_BIN 2
#define DATAQ_MODE_FLOAT 3

#define DATAQ_EOC "\r" //end of command




int __send_command(int fd, char *cmd, char *result, unsigned int size);
int dataq_init(int fd, int mode);

int dataq_open_dev(char *dev_name, bool block);

int dataq_model(int fd, char *model_name, unsigned int size);
int dataq_vendor(int fd, char *vendor_name, unsigned int size);
int dataq_firmware(int fd, char *firmware_version, unsigned int size);
int dataq_serial_number(int fd, char *serial_number, unsigned int size);

int dataq_start(int fd);
int dataq_stop(int fd);


#endif
