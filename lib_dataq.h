#ifndef LIB_DATAQ_H
#define LIB_DATAQ_H

#include <stdbool.h>
#include <fcntl.h>
#include <string.h>

#define DATAQ_CMD_VENDOR "info 0\r"
#define DATAQ_CMD_MODEL "info 1\r"
#define DATAQ_CMD_FIRMWARE "info 2\r"
#define DATAQ_CMD_SERIAL_NUM "info 6\r"

int open_dev(char* dev_name, bool block);
int send_command(int fd, char* cmd, char* result, unsigned int size);

int model(int fd, char* model_name, unsigned int size);
int vendor(int fd, char* vendor_name, unsigned int size);
int firmware(int fd, char* firmware_version, unsigned int size);
int serial_number(int fd, char* serial_number, unsigned int size);


#endif
