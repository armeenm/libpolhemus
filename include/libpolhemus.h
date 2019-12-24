#pragma once

#include <libusb-1.0/libusb.h>

#ifdef __cplusplus
extern "C" {
#endif

enum DevType { PATRIOT, PATRIOT_HS };
struct libpolhemus_device_handle;

#ifndef __cplusplus
typedef enum DevType DevType;
typedef struct libpolhemus_device_handle libpolhemus_device_handle;
#endif

int libpolhemus_init();
int libpolhemus_open(DevType dev_type, libpolhemus_device_handle** dev_handle);

int libpolhemus_snd(libpolhemus_device_handle* dev_handle, void* buf,
                     int maxlen, int* transferred, unsigned int timeout);
int libpolhemus_rcv(libpolhemus_device_handle* dev_handle, void* buf,
                      int maxlen, int* transferred, unsigned int timeout);
int libpolhemus_sndrcv(
        libpolhemus_device_handle* handle,
        void* snd_buf,
        void* rcv_buf,
        int max_snd,
        int max_rcv,
        int* sent,
        int* rcvd,
        unsigned int snd_timeout,
        unsigned int rcv_timeout
);

void libpolhemus_close(libpolhemus_device_handle* dev_handle);
void libpolhemus_exit();

#ifdef __cplusplus
}
#endif
