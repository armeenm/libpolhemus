#include <stdbool.h>
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

#define CHECK2(str, func, val) \
    do {                       \
        if (r != val)          \
            func(str, r);      \
        else                   \
            PASS(str);         \
    } while (0)

#define CHECK(str) CHECK2(str, FAIL, 0)
#define CHECKV(str, val) CHECK2(str, FAIL, val)
#define CHECKF(str) CHECK2(str, FATAL, 0)
#define CHECKFV(str, val) CHECK2(str, FATAL, val)

int main(void) {
    /*
    uint8_t handle_idx;
    int r = 0;

    // Init //
    r = libpolhemus_init();
    CHECKF("init");

    r = libpolhemus_open(PATRIOT_HS, &handle_idx);
    CHECKF("open");

    r = libpolhemus_check_connection(handle_idx);
    CHECK("check conn");

    Buffer cmd = {calloc(16, sizeof(unsigned char)), 0};
    size_t cmd_buf_size;

    Buffer resp = {malloc(1000 * sizeof(unsigned char)), 1000};

    printf("Input: ");
    while ((cmd.len = getline((char**)&cmd.data, &cmd_buf_size, stdin)) != -1) {
        cmd.len--;  // Remove newline
        r = libpolhemus_send_cmd(handle_idx, cmd, resp);
        if (r < 0) {
            printf("Oops! %d", r);
            break;
        }

        printf("Received %d bytes\n", r);
        resp.data[r] = 0;  // Null term
        printf("Data: %s\n", resp.data);
        // printf("Station Number: %d%d\n", resp.data[0], resp.data[1]);

        memset(cmd.data, 0, 16 * sizeof(char));
        printf("Input: ");
    }

    puts("");

    free(cmd.data);
    free(resp.data);

    // Close //
    libpolhemus_close(handle_idx);
    PASS("close");

    // Exit //
    libpolhemus_exit();
    PASS("exit");
    */

    return 0;
}
