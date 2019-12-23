#include <stdio.h>

#include "libpolhemus.h"

#define PASS(sec) (puts("PASS: " sec))

#define FAIL(sec, err) (printf("FAIL: %s: %d\n", (sec), (err)))
#define FAIL_D(sec) FAIL(sec, DEF_ERR)

#define FATAL(sec, err)                          \
    do {                                         \
        printf("FATAL: %s: %d\n", (sec), (err)); \
        return -1;                               \
    } while (0)

#define CHECKF(str)        \
    do {                   \
        if (r)             \
            FATAL(str, r); \
        else               \
            PASS(str);     \
    } while (0)

int main(int argc, char** argv) {
    int r = 0;

    r = libpolhemus_init();
    CHECKF("init");

    int vid = 0x0f44, pid = 0xef12;  // Patriot's VID:PID
    r = libpolhemus_open(vid, pid);
    CHECKF("open");

    libpolhemus_exit();
    PASS("exit");

    return 0;
}
