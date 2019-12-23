#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "libpolhemus.h"

#define BUF_SIZE 1000

#define PASS(sec) printf("PASS: %s\n", (sec))
#define PASSM(sec, num) printf("PASS: %s: %d\n", (sec), (num))

#define FAIL(sec, err) printf("FAIL: %s: %d\n", (sec), (err))

#define FATAL(sec, err)                          \
    do {                                         \
        printf("FATAL: %s: %d\n", (sec), (err)); \
        return -1;                               \
    } while (0)

#define CHECK2(str, func) \
    do {                  \
        if (r)            \
            func(str, r); \
        else              \
            PASS(str);    \
    } while (0)

#define CHECK(str) CHECK2(str, FAIL)
#define CHECKF(str) CHECK2(str, FATAL)

int main(void) {
    libpolhemus_device_handle* handle = NULL;
    int r = 0, transferred = 0;
    uint8_t* buf = NULL;

    // Init //
    r = libpolhemus_init();
    CHECKF("init");

    // Open //
    r = libpolhemus_open(PATRIOT_HS, &handle);
    CHECKF("open");

    // Raw Write //
    r = libpolhemus_write(handle, "\r", 1, &transferred);
    if (r < 0)
        FAIL("write", r);
    else
        PASSM("write", transferred);

    // Raw Read //
    buf = malloc(BUF_SIZE * sizeof(uint8_t));
    r = libpolhemus_read(handle, buf, BUF_SIZE, &transferred);
    if (r < 0)
        FAIL("read", r);
    else
        PASSM("read", transferred);

    // Actual Transfer //
    while (!transferred) {
        r = libpolhemus_write(handle, "\r", 1, NULL);
        if (r < 0)
            FATAL("data", -1);
        usleep(100000);
        r = libpolhemus_read(handle, buf, BUF_SIZE, &transferred);
        if (r < 0)
            FATAL("data", -1);
    }

    char* command = calloc(16, sizeof(char));
    ssize_t cnt;
    size_t cmd_buf_size;
    printf("Input: ");
    while ((cnt = getline(&command, &cmd_buf_size, stdin)) != -1) {
        printf("Read: %ld\n", cnt);
        command[cnt - 1] = '\r';
        command[cnt] = '\n';
        cnt++;
        printf("Command given: %s\n", command);
        printf("Sending %ld bytes...", cnt);
        r = libpolhemus_write(handle, command, cnt, &transferred);
        if (r < 0) {
            FAIL("data", -1);
            break;
        } else if (cnt != transferred) {
            printf("FAILED!\n");
            break;
        } else {
            printf("SUCCESS!\n");
        }

        usleep(50);

        r = libpolhemus_read(handle, buf, BUF_SIZE, &transferred);
        printf("Received %d bytes\n", transferred);
        if (transferred > 0 && transferred < BUF_SIZE) {
            buf[transferred] = 0; // Null term.
            printf("Station number: %u\n", buf[2]);
            printf("Initiating command: 0x%x\n", buf[3]);
            printf("Error indicator: 0x%x\n", buf[4]);
            short response_size = (buf[7] << 8) + buf[6];
            printf("Response size: %u\n", response_size);
            printf("Data: %s\n", buf + 8);
        }
        printf("Input: ");
        memset(command, 0, 16 * sizeof(char));
    }
    puts("");

    free(command);

    // Close //
    libpolhemus_close(handle);
    PASS("close");

    // Exit //
    libpolhemus_exit();
    PASS("exit");

    return 0;
}
