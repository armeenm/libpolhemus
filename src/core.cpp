#include <libusb-1.0/libusb.h>
#include <stdio.h>

extern "C" {

static libusb_device_handle* handle = nullptr;

int libpolhemus_init() {
    int r = libusb_init(nullptr);
    return r;
}

int libpolhemus_open(int vid, int pid) {
    handle = libusb_open_device_with_vid_pid(nullptr, vid, pid);
    if (!handle) return -1;

    return 0;
}

void libpolhemus_exit() { libusb_exit(nullptr); }
}
