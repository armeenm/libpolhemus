#pragma once

#include <libusb-1.0/libusb.h>

#define DEVS PATRIOT, PATRIOT_HS

#ifdef __cplusplus
extern "C" {
enum class DevType { DEVS };
#else
typedef enum DevType { DEVS } DevType;
#endif

struct Buffer {
    unsigned char* data;
    int len;
};

int libpolhemus_init();
void libpolhemus_exit();

int libpolhemus_open(DevType dev_type, uint8_t* handle_idx);

int libpolhemus_snd(uint8_t handle_idx, void* buf, int maxlen, int* transferred,
                    unsigned int timeout);
int libpolhemus_rcv(uint8_t handle_idx, void* buf, int maxlen, int* transferred,
                    unsigned int timeout);
int libpolhemus_sndrcv(uint8_t handle_idx, void* snd_buf, void* rcv_buf,
                       int max_snd, int max_rcv, int* sent, int* rcvd,
                       unsigned int snd_timeout, unsigned int rcv_timeout);

void libpolhemus_close(uint8_t handle_idx);

#ifdef __cplusplus
}
#endif
