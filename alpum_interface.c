#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <alloca.h>

#include "alpum_interface.h"

//#define DEBUGER_MODE	1

/*
 * parameter
 *      serialNo: a pointer to 8 byte unsigned char array to store the chipid.
 * return
 *      return zero (ERROR_CODE_TRUE) if success.
 */
int GetIdChipSerialNo(unsigned char *serialNo)
{
    unsigned char IDCHIP_ADDR   = 0x3d;

    unsigned char error_code;
    unsigned char idchip_type[] = { 0x00, 0x00, 0x00, 0x00,
                                    0x04, 0x7D, 0xC2, 0x40 };
    unsigned char buffer[8];
    bzero(buffer, 8);
    error_code = _i2c_read(IDCHIP_ADDR, 0x75, buffer, 8);
    if (error_code) return error_code;

    if (0 != memcmp(buffer, idchip_type, sizeof(idchip_type)))
        return ERROR_CODE_FALSE;
    memcpy(serialNo, idchip_type + 4, 4);
    usleep(10);

    bzero(buffer, 8);
    error_code = _i2c_read(IDCHIP_ADDR, 0x76, buffer, 8);
    if (error_code) return error_code;
    memcpy(serialNo + 4, buffer + 4, 4);
    return ERROR_CODE_TRUE;
}

unsigned char _alpu_rand(void)
{
    time_t t;
    static unsigned long seed; // 2byte, must be a static variable
    srand((unsigned) time(&t));
    seed = seed + rand();   // rand(); <-- add time value
    seed = seed * 1103515245 + 12345;
    return (seed/65536) % 32768;
}

/*
 * suspend execution for microsecond (1/1,000,000 second) intervals
 */
void _alpu_delay_ms(unsigned int i)
{
//    usleep(i * 100);

#if 1
    struct timeval delay;
    struct timeval oldtm, tm;
    delay.tv_sec = 0;
    delay.tv_usec = i * 1000;
//        gettimeofday(&oldtm, NULL);
    select(0, NULL, NULL, NULL, &delay);
 //       gettimeofday(&tm, NULL);
#if DEBUGER_MODE
    printf("delay %d\n", i);
#endif
#endif
}

//unsigned char _i2c_read(unsigned char device_addr,
//                        unsigned char sub_addr,
//                        unsigned char *buff,
//                        int ByteNo)
//{
//    int file;
//    int i;
//    file = open("/dev/i2c-0", O_RDWR);
//    if (file < 0) {
//    /* ERROR HANDLING; you can check errno to see what went wrong */
//        return ERROR_CODE_READ_ADDR;
//    }
//    if (ioctl(file, I2C_SLAVE, device_addr) < 0) {
//        #if DEBUGER_MODE
//        printf("\n\r Rx device_addr Error - 0x%02x ",device_addr);
//        #endif
//        return ERROR_CODE_READ_ADDR;
//    }
//    ioctl(file, I2C_TIMEOUT, 2);
//    ioctl(file, I2C_RETRIES, 1);

//    unsigned char addr = sub_addr;
//    bzero(buff, ByteNo);
//    if (write(file, &addr, 1) < 0)
//    {
//        #if DEBUGER_MODE
//        printf("\n\r Rx sub_addr Error - 0x%02x 0x%02x ",device_addr,sub_addr);
//        #endif
//        return ERROR_CODE_READ_ADDR;
//    }
//    if (read(file, buff, ByteNo) < 0)
//    {
//        #if DEBUGER_MODE
//        printf("\n\r Rx Data Err : ");
//        printf("0x%02x 0x%02x ",device_addr,sub_addr);
//        for (i=0; i<ByteNo; i++) printf("0x%02x ", buff[i]);
//        #endif
//        return ERROR_CODE_READ_ADDR;
//    }
//    close(file);
//#if DEBUGER_MODE
//    printf("\n\r Rx Data : ");
//    printf("0x%02x 0x%02x ",device_addr,sub_addr);
//    for (i=0; i<ByteNo; i++) printf("0x%02x ", buff[i]);
//#endif
//    return ERROR_CODE_TRUE;
//}

//unsigned char _i2c_write(unsigned char device_addr,
//                         unsigned char sub_addr,
//                         unsigned char *buff,
//                         int ByteNo)
//{
//    int file;
//    int i;
//    unsigned char retCode;

//    file = open("/dev/i2c-0", O_RDWR);
//    if (file < 0) {
//    /* ERROR HANDLING; you can check errno to see what went wrong */
//        return ERROR_CODE_WRITE_ADDR;
//    }
//    if (ioctl(file, I2C_SLAVE, device_addr) < 0) {
//        #if DEBUGER_MODE
//        printf("\n\r Tx device_addr Error - 0x%02x ",device_addr);
//        #endif
//        return ERROR_CODE_WRITE_ADDR;
//    }
//    ioctl(file, I2C_TIMEOUT, 2);
//    ioctl(file, I2C_RETRIES, 1);
//    unsigned char *buffer = (unsigned char*)malloc(ByteNo + 1);
//    if (NULL == buffer) return ERROR_CODE_WRITE_DATA;
//    bzero(buffer, ByteNo + 1);
//    buffer[0] = sub_addr;
//    memcpy(buffer + 1, buff, ByteNo);
//    if (write(file, buffer, ByteNo + 1 ) < 0)
//    {
//#if DEBUGER_MODE
//        printf("\n\r Tx Data Err : ");
//        printf("0x%02x 0x%02x ",device_addr, sub_addr);
//        for (i=0; i<ByteNo; i++) printf("0x%02x ", buff[i]);
//#endif
//        return ERROR_CODE_WRITE_DATA;
//    }
//#if DEBUGER_MODE
//    printf("\n\r Tx Data : ");
//    printf("0x%02x 0x%02x ",device_addr,sub_addr);
//    for (i=0; i<ByteNo; i++) printf("0x%02x ", buff[i]);
//#endif
//    free(buffer);
//    return ERROR_CODE_TRUE;

//}
#if 1

int _i2c_write_byte(unsigned dev_addr, unsigned char sub_addr, unsigned char data)
{
    int file;

    file = open("/dev/i2c-0", O_RDWR);
    if (file < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
        return ERROR_CODE_WRITE_ADDR;
    }
    if (ioctl(file, I2C_SLAVE, dev_addr) < 0) {
        #if DEBUGER_MODE
        printf("\n\r Rx device_addr Error - 0x%02x ",dev_addr);
        #endif
        return ERROR_CODE_WRITE_ADDR;
    }
    ioctl(file, I2C_TIMEOUT, 2);
    ioctl(file, I2C_RETRIES, 1);

    struct i2c_rdwr_ioctl_data i2c_data;
    struct i2c_msg msgs[1];

    msgs[0].addr = dev_addr;
    msgs[0].flags = 4;
    msgs[0].len = 2;
    msgs[0].buf = (unsigned char *)alloca(2);
    msgs[0].buf[0] = sub_addr;
    msgs[0].buf[1] = data;

    i2c_data.msgs = msgs;
    i2c_data.nmsgs = 1;

    ioctl(file, I2C_RDWR, &i2c_data);
    close(file);
    return ERROR_CODE_TRUE;
}
#endif

unsigned char _i2c_write(unsigned char dev_addr,
               unsigned char sub_addr,
               unsigned char *data, int len)
{
    int file, i, ret = 0;

    file = open("/dev/i2c-0", O_RDWR);
    if (file < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */
        return ERROR_CODE_WRITE_ADDR;
    }
    if (ioctl(file, I2C_SLAVE, dev_addr) < 0) {
        #if DEBUGER_MODE
        printf("\n\r Rx device_addr Error - 0x%02x ",dev_addr);
        #endif
        return ERROR_CODE_WRITE_ADDR;
    }
    ioctl(file, I2C_TIMEOUT, 2);
    ioctl(file, I2C_RETRIES, 3);

    struct i2c_rdwr_ioctl_data i2c_data;
    struct i2c_msg msgs[1];

    msgs[0].addr = dev_addr;
    msgs[0].flags = 0;
    msgs[0].len = len+1;
    msgs[0].buf = (unsigned char *)alloca(len+1);
    msgs[0].buf[0] = sub_addr;
    memcpy(msgs[0].buf+1, data, len);

    i2c_data.msgs = msgs;
    i2c_data.nmsgs = 1;

    ret = ioctl(file, I2C_RDWR, &i2c_data);
    close(file);
    #if DEBUGER_MODE
        printf("\n\r Tx Data : ");
        printf("0x%02x 0x%02x \t%d\n",dev_addr,sub_addr, ret);
        for (i=0; i<len; i++) printf("0x%02x ", data[i]);
    #endif
    return ERROR_CODE_TRUE;
}

unsigned char _i2c_read(unsigned char dev_addr,
               unsigned char sub_addr,
               unsigned char *data, int len)
{
    int file, i, ret = 0;
    file = open("/dev/i2c-0", O_RDWR);
    if (file < 0) {
    /* ERROR HANDLING; you can check errno to see what went wrong */                   
    /* ERROR HANDLING; you can check errno to see what went wrong */
        return ERROR_CODE_READ_ADDR;
    }
    if (ioctl(file, I2C_SLAVE, dev_addr) < 0) {
        #if DEBUGER_MODE
        printf("\n\r Rx device_addr Error - 0x%02x ",dev_addr);
        #endif
        return ERROR_CODE_READ_ADDR;
    }
    ioctl(file, I2C_TIMEOUT, 2);
    ioctl(file, I2C_RETRIES, 1);

    struct i2c_rdwr_ioctl_data i2c_data;
    struct i2c_msg msgs[2];

    msgs[0].addr = dev_addr;
    msgs[0].flags = 4;      //non-stop
    msgs[0].len = 1;
    msgs[0].buf = &sub_addr;

    msgs[1].addr = dev_addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = len;
    msgs[1].buf = data;

    i2c_data.msgs = msgs;
    i2c_data.nmsgs = 2;

    ioctl(file, I2C_RDWR, &i2c_data);
    close(file);
    #if DEBUGER_MODE
        printf("\n\r Rx Data : ");
        printf("0x%02x 0x%02x \t%d\n",dev_addr,sub_addr, ret);
        for (i=0; i<len; i++) printf("0x%02x ", data[i]);
    #endif
    return ERROR_CODE_TRUE;
}




/* EOF */

