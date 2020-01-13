/* Do NOT #include this file directly
 * It is meant to be #included by the C or C++ API headers
 */

#include "libpolhemus_macro.h"

typedef enum {
    PLHM_ADD_ENUM_PREFIX_(PATRIOT),
    PLHM_ADD_ENUM_PREFIX_(PATRIOT_HS)
} PLHM_DEV_TYPE_;

typedef struct {
    unsigned char* data;
    int len;
} PLHM_BUFFER_;

#include "libpolhemus_macro.h"
