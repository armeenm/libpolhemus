#pragma once

#include "libpolhemus_common.h"
#include "libpolhemus_macro.h"

typedef struct PLHM_DEV_HANDLE_ PLHM_DEV_HANDLE_;

int PLHM_ADD_PREFIX_(init)();

int PLHM_ADD_PREFIX_(open)(PLHM_DEV_TYPE_ dev_type, PLHM_DEV_HANDLE_* handle);

unsigned int PLHM_ADD_PREFIX_(get_timeout)(PLHM_DEV_HANDLE_* handle);

void PLHM_ADD_PREFIX_(set_timeout)(PLHM_DEV_HANDLE_* handle,
                                   unsigned int timeout);

int PLHM_ADD_PREFIX_(send_raw)(PLHM_DEV_HANDLE_* handle,
                               const PLHM_BUFFER_* buf);
int PLHM_ADD_PREFIX_(recv_raw)(PLHM_DEV_HANDLE_* handle, PLHM_BUFFER_* buf);

int PLHM_ADD_PREFIX_(check_connection_att)(PLHM_DEV_HANDLE_* handle,
                                           unsigned int attempts);
int PLHM_ADD_PREFIX_(check_connection)(PLHM_DEV_HANDLE_* handle);

int PLHM_ADD_PREFIX_(send_cmd)(PLHM_DEV_HANDLE_* handle,
                               const PLHM_BUFFER_* cmd, PLHM_BUFFER_* resp);

void PLHM_ADD_PREFIX_(close)(libpolhemus_device_handle* handle);

void PLHM_ADD_PREFIX_(exit)();

#include "libpolhemus_macro.h"
