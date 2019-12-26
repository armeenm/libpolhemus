#pragma once

#include <libusb-1.0/libusb.h>
#include <stdbool.h>

#define DEVS PATRIOT, PATRIOT_HS

#ifdef __cplusplus
extern "C" {
enum class DevType { DEVS };
#else
typedef enum DevType { DEVS } DevType;
#endif

typedef struct Buffer {
    unsigned char* data;
    int len;
} Buffer;

int libpolhemus_init();

int libpolhemus_open(DevType dev_type, uint8_t* handle_idx);

bool libpolhemus_valid(uint8_t handle_idx);

unsigned int libpolhemus_get_timeout(uint8_t handle_idx);
void libpolhemus_set_timeout(uint8_t handle_idx, unsigned int timeout);

int libpolhemus_send_raw(uint8_t handl_idx, Buffer buf);
int libpolhemus_recv_raw(uint8_t handle_idx, Buffer buf);

int libpolhemus_check_connection_att(uint8_t handle_idx, uint8_t attempts);
int libpolhemus_check_connection(uint8_t handle_idx);

int libpolhemus_send_cmd(uint8_t handle_idx, Buffer cmd, Buffer resp);

void libpolhemus_close(uint8_t handle_idx);

void libpolhemus_exit();

#ifdef __cplusplus
}
#endif
