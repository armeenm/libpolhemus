#pragma once

#include <libusb-1.0/libusb.h>

#ifdef __cplusplus
extern "C" {
#endif

enum DevType { PATRIOT, PATRIOT_HS };

#ifndef __cplusplus
typedef enum DevType DevType;
typedef struct DevInfo DevInfo;
typedef struct libpolhemus_device_handle libpolhemus_device_handle;
#endif

struct DevInfo {
    DevType dev_type;
    const char* name;
    uint16_t vid, pid;
    unsigned char write_ep, read_ep;
};

struct libpolhemus_device_handle {
    libusb_device_handle* libusb_handle;
    DevInfo info;
};

int libpolhemus_init();
libpolhemus_device_handle* libpolhemus_open(DevType dev_type);
int libpolhemus_read(libpolhemus_device_handle* dev_handle, void* buf,
                     int maxlen, int* transferred);
int libpolhemus_write(libpolhemus_device_handle* dev_handle, void* buf,
                      int maxlen, int* transferred);
void libpolhemus_close(libpolhemus_device_handle* dev_handle);
void libpolhemus_exit();

#ifdef __cplusplus
}
#endif
