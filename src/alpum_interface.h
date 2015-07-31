#ifndef __ALPUM_INTERFACE_H__
#define __ALPUM_INTERFACE_H__

#define DEBUGER_MODE 1
#define ERROR_CODE_TRUE			0
#define ERROR_CODE_FALSE		1
#define ERROR_CODE_WRITE_ADDR	10
#define ERROR_CODE_WRITE_DATA	20
#define ERROR_CODE_READ_ADDR	30
#define ERROR_CODE_READ_DATA	40
#define ERROR_CODE_START_BIT	50
#define ERROR_CODE_APROCESS		60
#define ERROR_CODE_DENY			70

int GetIdChipSerialNo(unsigned char *serialNo);

unsigned char _alpu_rand(void);
void _alpu_delay_ms(unsigned int i);

unsigned char _i2c_write(unsigned char , unsigned char, unsigned char *, int);
unsigned char _i2c_read(unsigned char , unsigned char, unsigned char *, int);

#endif // __ALPUM_INTERFACE_H__
/* EOF */
