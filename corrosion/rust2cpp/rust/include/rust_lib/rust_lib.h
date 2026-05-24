#ifndef RUST_LIB_H_INCLUDED
#define RUST_LIB_H_INCLUDED

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Print a greeting to given name.
 */
void rust_function(const char *name);

/**
 * Add given parameters together, and return the result.
 */
int32_t add(int32_t left, int32_t right);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  /* RUST_LIB_H_INCLUDED */
