#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "libpolhemus.h"

#define BUF_SIZE 1000
#define DEF_TIMEOUT 50

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
    int r = 0;
    uint8_t* buf = malloc(BUF_SIZE * sizeof(uint8_t));

    // Init //
    r = libpolhemus_init();
    CHECKF("init");

    // Open //
    r = libpolhemus_open(PATRIOT_HS, &handle);
    CHECKF("open");

    char* command = calloc(16, sizeof(char));
    int sent = 0, rcvd = 0;
    ssize_t cnt = 1;
    size_t cmd_buf_size;

    do {
        if (command[0] == 'P' || command[0] == 'p')
            cnt = 1;
        else
            command[cnt - 1] = '\r';

        printf("Command given: %s\n", command);
        printf("Counted %ld bytes\n", cnt);

        r = libpolhemus_snd(handle, command, cnt, &sent, DEF_TIMEOUT);
        if (r < 0) {
            FAIL("send", r);
            break;
        } else if (cnt != sent) {
            printf("FAILED!\n");
            break;
        } else
            printf("SUCCESS!\n");

        r = libpolhemus_rcv(handle, buf, BUF_SIZE, &rcvd, DEF_TIMEOUT);
        if (r < 0)
            FAIL("receive", r);

        printf("Received %d bytes\n", rcvd);

        if (rcvd > 0 && rcvd < BUF_SIZE) {
            short response_size = (buf[7] << 8) + buf[6];

            buf[rcvd] = 0;  // Null term.
            printf("Station number: %u\n", buf[2]);
            printf("Initiating command: 0x%x\n", buf[3]);
            printf("Error indicator: 0x%x\n", buf[4]);
            printf("Response size: %u\n", response_size);
            printf("Data: %s\n", buf + 8);
        }

        printf("Input: ");
        memset(command, 0, 16 * sizeof(char));
    } while ((cnt = getline(&command, &cmd_buf_size, stdin)) != -1);

    puts("");

    free(command);
    free(buf);

    // Close //
    libpolhemus_close(handle);
    PASS("close");

    // Exit //
    libpolhemus_exit();
    PASS("exit");

    return 0;
}
