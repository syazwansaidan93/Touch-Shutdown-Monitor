#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define GPIO_PIN "73"
#define LED_TRIG "/sys/class/leds/red_led/trigger"
#define LED_BRIGHT "/sys/class/leds/red_led/brightness"

int main() {
    int fd;
    char value;
    char path[64];

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

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s/value", GPIO_PIN);
    while (1) {
        fd = open(path, O_RDONLY);
        if (fd != -1) {
            read(fd, &value, 1);
            close(fd);

            if (value == '1') {
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
                break;
            }
        }
        usleep(100000);
    }

    return 0;
}
