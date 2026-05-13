#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>

#define GPIO_PIN "73"
#define LED_TRIG "/sys/class/leds/red_led/trigger"
#define LED_BRIGHT "/sys/class/leds/red_led/brightness"

int main() {
    int fd;
    char path[64];
    struct pollfd pfd;
    char dummy;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd != -1) {
        write(fd, GPIO_PIN, strlen(GPIO_PIN));
        close(fd);
    }

    usleep(100000);

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s/direction", GPIO_PIN);
    fd = open(path, O_WRONLY);
    if (fd != -1) {
        write(fd, "in", 2);
        close(fd);
    }

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s/edge", GPIO_PIN);
    fd = open(path, O_WRONLY);
    if (fd != -1) {
        write(fd, "rising", 6);
        close(fd);
    }

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s/value", GPIO_PIN);
    fd = open(path, O_RDONLY);
    if (fd < 0) return 1;

    pfd.fd = fd;
    pfd.events = POLLPRI;

    lseek(fd, 0, SEEK_SET);
    read(fd, &dummy, 1);

    poll(&pfd, 1, -1);

    int t_fd = open(LED_TRIG, O_WRONLY);
    if (t_fd != -1) {
        write(t_fd, "none", 4);
        close(t_fd);
    }
    int b_fd = open(LED_BRIGHT, O_WRONLY);
    if (b_fd != -1) {
        write(b_fd, "1", 1);
        close(b_fd);
    }

    system("poweroff");

    close(fd);
    return 0;
}
