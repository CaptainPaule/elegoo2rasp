#ifndef __SERIAL_LIB_H__
#define __SERIAL_LIB_H__

#include <stdint.h>

int serialInit(const char *port, int baudrate);
int serialClose(int fd);
int serialWritebytes(int fd, uint8_t *b, int len);
int serialReadbytes(int fd, uint8_t *buf, int len);
int serialWriteChars(int fd, const char *str);
int serialReadChars(int fd, char *buf, int len);
int serialFlush(int fd);

#endif
