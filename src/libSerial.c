#include "libSerial.h"

#include <stdio.h>    // Standard input/output definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>    // File control definitions
#include <errno.h>    // Error number definitions
#include <termios.h>  // POSIX terminal control definitions
#include <string.h>   // String function definitions
#include <sys/ioctl.h>
#include <stdlib.h>

static PyObject *SerialError;

int serialWritebytes( int fd, uint8_t *b, int len) {
    	int n = write(fd, &b, len);
    	if(n != len)
        	return -1;

    	return n;
}

int serialReadBytes(int fd, uint8_t *buf, int len) {
	int r = read(fd, buf, len);
	if(r != len){
		return -1;
	}

	return r;
}

int recv(int fd, DATAFRAME *data) {
	int *size = NULL;
	uint8_t *sizeBuffer = (uint8_t*) malloc(sizeof(int));
	uint8_t *dataBuffer = NULL;

	// read data size
	int read = serialReadBytes(fd, sizeBuffer, sizeof(int));
	size = (int*) sizeBuffer;

	if(*size == -1) {
		return -1;
	}

	// read data blob
	read = serialReadBytes(fd, dataBuffer, *size);
	data = (DATAFRAME*) dataBuffer;

	return read;
}

int serialResetDevice(int fd) {
	char *str = "r";
	uint8_t  *b = (uint8_t*)str;
	int writtenBytes = serialWritebytes(fd, b, 1);
	return writtenBytes;
}

int serialInit(const char* serialport, int baudrate)
{
    struct termios toptions;
    int fd;

    fd = open(serialport, O_RDWR | O_NONBLOCK );
    if (fd == -1)  {
        perror("serialInit: Unable to open port ");
        return -1;
    }

    if (tcgetattr(fd, &toptions) < 0) {
        perror("serialport_init: Couldn't get term attributes");
        return -1;
    }

    speed_t brate = baudrate;
    	switch(baudrate) {
    		case 4800:
			brate=B4800;
			break;

    		case 9600:
			brate=B9600;
			break;

		#ifdef B14400
    		case 14400:
			brate=B14400;
			break;
		#endif

    		case 19200:
			brate=B19200;
			break;

		#ifdef B28800
    		case 28800:
			brate=B28800;
			break;
		#endif

    		case 38400:
			brate=B38400;
			break;

    		case 57600:
			brate=B57600;
			break;

    		case 115200:
			brate=B115200;
			break;
    		}
    		cfsetispeed(&toptions, brate);
    		cfsetospeed(&toptions, brate);

    		// 8N1
    		toptions.c_cflag &= ~PARENB;
    		toptions.c_cflag &= ~CSTOPB;
    		toptions.c_cflag &= ~CSIZE;
    		toptions.c_cflag |= CS8;

    		// no flow control
    		toptions.c_cflag &= ~CRTSCTS;
    		toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    		toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
    		toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    		toptions.c_oflag &= ~OPOST; // make raw


    		toptions.c_cc[VMIN]  = 0;
    		toptions.c_cc[VTIME] = 0;

    		tcsetattr(fd, TCSANOW, &toptions);
    		if( tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
        		perror("init_serialport: Couldn't set term attributes");
        		return -1;
    		}

    		return fd;
}

int serialClose( int fd ) {
    	return close( fd );
}


static PyObject* serial_init(PyObject *self, PyObject *args) {
	const char *port;
	int baudrate;
	int err = 0;

	if(!PyArg_ParseTuple(args, "si", &port, baudrate)) {
		return NULL;
	}

	err = serialInit(port, baudrate);
	if(err == -1) {
		PyErr_SetString(SerialError, "can not open device");
		return NULL;
	}
	return Py_BuildValue("i", err);
}

static PyObject* serial_reset(PyObject *self, PyObject *args) {
	int *fd;
	int err = 0;

	if(!PyArg_ParseTuple(args, "i", &fd)) {
		return NULL;
	}

	err = serialResetDevice(*fd);
	if(err == -1) {
		PyErr_SetString(SerialError, "can not reset device");
		return NULL;
	}
	return Py_BuildValue("i", err);
}

static PyObject* serial_recv(PyObject *self, PyObject *args) {
	int *fd = malloc(sizeof(int));
	DATA *dat = malloc(sizeof(DATA));
	int err = 0;

	if(!(args, "i", &fd)) {
		return NULL;
	}

	err =  recv(*fd, dat);
	if(err == -1) {
		PyErr_SetString(SerialError, "can not read from device");
		return NULL;
	}

	// build python dict
	return Py_BuildValue("{s:i, s:f, s:f}",
			    &dat->num,
			    &dat->temp.mean,
			    &dat->temp.std
			    );
}

static PyMethodDef SerialMethods[] = {
	{"init", serial_init, METH_VARARGS,
		"Initialize serial connection to elegoo device."},

	{"recv", serial_recv, METH_VARARGS,
		"receive data from serial port."},

	{"reset", serial_reset, METH_VARARGS,
		"reset device."},

	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef serialmodule = {
	PyModuleDef_HEAD_INIT,
	"serial",
	NULL,
	-1,
	SerialMethods
};

PyMODINIT_FUNC PyInit_serial(void) {
	PyObject *m = PyModule_Create(&serialmodule);
	if(m == NULL) {
		return NULL;
	}

	SerialError = PyErr_NewException("serial.error", NULL, NULL);
	Py_INCREF(SerialError);
	PyModule_AddObject(m, "error", SerialError);
	return m;
}