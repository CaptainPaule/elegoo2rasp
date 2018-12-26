#include <log.h>
#include <sys/stat.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

int main(int argc, char *argv[]) {
    if(argc != 2) {
        log_error("wrong number of arguments!");
        return -1;
    }

    printf("[I]: looking for serial port %s\n", argv[1]);
    struct stat buffer;
    int exist = stat(argv[1], &buffer);
    if(exist != 0) {
        log_error("serial port not found!");
        return -1;
    }

    log_info("open port...");
    int fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1) {
    	log_error("unable to open port");
	return -1;
    }

    log_info("setup control structure");
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_flag |= (CLOCAL | CREAD);
    tcsetattr(fd, TCSANOW, &options);

    
    return 0;
}
