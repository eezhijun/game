/**
 * @file error.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _ERROR_H_
#define _ERROR_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t U_RESULT;

#ifdef U_SUCCESS
#undef U_SUCCESS
#endif
#define U_SUCESS 0x0000

#ifdef U_FAILURE
#undef U_FAILURE
#endif
#define U_FAILURE 0xFFFF

#define ERR_ID 0x0000

#ifdef __cplusplus
};
#endif

#endif
