#include <log.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        log_error("wrong number of arguments!");
        return -1;
    }

    printf("[I]: looking for serial port %s\n", argv[1]);
    struct stat buffer;
    int exist = stat(argv[0], &buffer);
    if(exist != 0) {
        log_error("serial port not found!");
        return -1;
    }

    log_info("open connection...");
    return 0;
}