#include <log.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if(argc != 1) {
        log_error("wrong number of arguments!");
        return -1;
    }

    log_info("looking for serial port %s", argv[0]);
    struct stat buffer;
    int exist = stat(argv[0], &buffer);
    if(exist != 0) {
        log_error("serial port not found!");
        return -1;
    }

    log_info("open connection...");
    return 0;
}