#ifndef __ALPUC0A9_H_
#define __ALPUC0A9_H_

//unsigned char alpuc_process(unsigned char *, unsigned char *);

extern unsigned char _alpu_rand(void);
extern void _alpu_delay_ms(unsigned int);
extern unsigned char _i2c_write(unsigned char , unsigned char, unsigned char *, int);
extern unsigned char _i2c_read(unsigned char , unsigned char, unsigned char *, int);

#endif // __ALPUC0A9_H_
