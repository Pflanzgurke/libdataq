/*! \file libdataq.h
 *
 */

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

#define DATAQ_MIN_RATE 12
#define DATAQ_MAX_RATE 10000

#define DATAQ_ANALOG_IN_0 "x0000"
#define DATAQ_ANALOG_IN_1 "x0001"
#define DATAQ_ANALOG_IN_2 "x0002"
#define DATAQ_ANALOG_IN_3 "x0003"
#define DATAQ_ANALOG_IN_4 "x0004"
#define DATAQ_ANALOG_IN_5 "x0005"
#define DATAQ_ANALOG_IN_6 "x0006"
#define DATAQ_ANALOG_IN_7 "x0007"

#define DATAQ_DIGITAL_IN_0 "x0008"



/*! 
 * \brief Data logger configuration struct.
 *
 * For some operations, it is necessary to remember the config of the device,
 * which cannot be queried. Therefore, the conf is stored (outside the lib)
 * and manipulated when device settings are changed. A conf can of course
 * to set the configuration of the device.
 */
typedef struct {
	int mode; // 0 = uninitialized, 1 = asc, 2 = bin, 3 = float
	int slist;
	unsigned int rate;
} dataq_conf;

long __dataq_send_command(int fd, char *cmd, char *result, unsigned int size);

/*! 
 * \fn inline void dataq_init_config(dataq_conf *conf)
 * \brief Initializes all members of the dataq_conf parameter to 0.
 *
 * Insd,nfsd.,nv sddsfkgj sdlvn sdkjfbsd.kjf sdkjfb sdkjfbvsdkfnv sdkjfbhs
 * srgjkhsd.kjh s.dfklgh sdbjkfn dkbj:sdfbi.
 *
 * param conf the config to be initialized
 */
inline void dataq_init_config(dataq_conf *conf);

int dataq_open_dev(char *dev_name, dataq_conf *conf);
long dataq_set_mode(int fd, int mode, dataq_conf *conf);
long dataq_reset_counter(int fd);

long dataq_model(int fd, char *model_name, unsigned int size);
long dataq_vendor(int fd, char *vendor_name, unsigned int size);
long dataq_firmware(int fd, char *firmware_version, unsigned int size);
long dataq_serial_number(int fd, char *serial_number, unsigned int size);

int dataq_slist_add(int fd, char *input, unsigned int size);
//int dataq_slist_remove(char *input);

//for a sampling rate between 12 and 10000 Hz (for all inputs)
long dataq_set_rate(int fd, unsigned int rate);
int dataq_apply_config(int fd, dataq_conf *conf);

long dataq_start(int fd);
long dataq_stop(int fd);


#endif
