#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "libpolhemus.h"

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

    r = libpolhemus_init();
    CHECKF("init");

    handle = libpolhemus_open(PATRIOT);
    if (handle)
        PASS("open");
    else
        FAIL("open", -1);

    r = libpolhemus_write(handle, "\r", 1, &transferred);
    if (r < 0)
        FAIL("write", r);
    else
        PASSM("write", transferred);

    buf = malloc(1024 * sizeof(uint8_t));
    r = libpolhemus_read(handle, buf, 1024, &transferred);
    if (r < 0)
        FAIL("read", r);
    else
        PASSM("read", transferred);

    libpolhemus_close(handle);
    PASS("close");

    libpolhemus_exit();
    PASS("exit");

    return 0;
}
