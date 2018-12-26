#include "serial-lib.h"

#include <stdio.h>    // Standard input/output definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>    // File control definitions
#include <errno.h>    // Error number definitions
#include <termios.h>  // POSIX terminal control definitions
#include <string.h>   // String function definitions
#include <sys/ioctl.h>

int serialport_init(const char* serialport, int baudrate)
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

    speed_t brate = baud;
    	switch(baud) {
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

int serialWriteChars(int fd, const char* str) {
    	int len = strlen(str);
    	int n = write(fd, str, len);
    	if( n!=len ) {
        	perror("serialport_write: couldn't write whole string\n");
        	return -1;
    	}
	return 0;
}

int serialReadChars(int fd, char *buf, int len) {
	int r = read(fd, buf, len);
	if(len != r){
		return -1;
	}

	return r;
}

int serialport_flush(int fd) {
	sleep(2);
}