/* Do NOT #include this file directly
 * #include once to add macros and #include again to remove
 */

#ifndef LIBPOLHEMUS_MACRO_H_
#define LIBPOLHEMUS_MACRO_H_

#ifndef POLHEMUS_PREFIX
#define POLHEMUS_PREFIX libpolhemus_
#endif

#ifndef POLHEMUS_ENUM_PREFIX
#define POLHEMUS_ENUM_PREFIX
#endif

#define II_PLHM_ADD_PREFIX_(PREFIX, NAME) PREFIX##NAME
#define I_PLHM_ADD_PREFIX_(PREFIX, NAME) II_PLHM_ADD_PREFIX_(PREFIX, NAME)

#define PLHM_ADD_PREFIX_(NAME) I_PLHM_ADD_PREFIX_(POLHEMUS_PREFIX, NAME)
#define PLHM_ADD_ENUM_PREFIX_(NAME) \
    I_PLHM_ADD_PREFIX_(POLHEMUS_ENUM_PREFIX, NAME)

#define PLHM_BUFFER_ PLHM_ADD_PREFIX_(buffer)
#define PLHM_DEV_TYPE_ PLHM_ADD_PREFIX_(device_type)
#define PLHM_DEV_HANDLE_ PLHM_ADD_PREFIX_(device_handle)

#else

#undef II_PLHM_ADD_PREFIX_
#undef I_PLHM_ADD_PREFIX_
#undef PLHM_ADD_PREFIX_
#undef PLHM_ADD_ENUM_PREFIX_
#undef PLHM_BUFFER_
#undef PLHM_DEV_TYPE_
#undef PLHM_DEV_HANDLE_
#undef LIBPOLHEMUS_MACRO_H_

#endif  // LIBPOLHEMUS_MACRO_H_
