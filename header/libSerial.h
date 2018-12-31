#ifndef __LIBSERIAL_H__
#define __LIBSERIAL_H__

#include <Python.h>
#include "definitions.h"
#include <stdint.h>

// basic functions
int serialInit(const char *port, int baudrate);
int serialClose(int fd);
int serialWritebytes(int fd, uint8_t *b, int len);
int serialReadBytes(int fd, uint8_t *buf, int len);
int serialWriteChars(int fd, const char *str);
int serialReadChars(int fd, char *buf, int len);
int serialFlush(int fd);
int serialResetDevice(int fd);

// communication related functions
int recv(int fd, DATAFRAME *data);

#endif
