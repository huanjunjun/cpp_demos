#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

void daemonize() {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process
        exit(EXIT_SUCCESS);
    }

    // Child process
    if (setsid() < 0) {
        std::cerr << "setsid failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Ignore signal sent from child to parent process
    signal(SIGCHLD, SIG_IGN);

    pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask
    umask(0);

    // Change the current working directory to root
    if (chdir("/") < 0) {
        std::cerr << "chdir failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Close all open file descriptors
    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    // Open log file
    int log_fd = open("/tmp/daemon.log", O_RDWR | O_CREAT | O_APPEND, 0600);
    if (log_fd < 0) {
        std::cerr << "Failed to open log file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Redirect standard file descriptors to /dev/null
    open("/dev/null", O_RDONLY); // stdin
    open("/dev/null", O_WRONLY); // stdout
    open("/dev/null", O_WRONLY); // stderr

    // Daemon process logic
    while (true) {
        dprintf(log_fd, "Daemon is running...\n");
        sleep(10); // Sleep for 10 seconds
    }

    close(log_fd);
}

int main() {
    daemonize();
    return 0;
}