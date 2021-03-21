/** @file base_typedefs.h

  * @brief SDK?ù±?ààDí??ò?

  * @date 2019/1/31

  */

#ifndef CMCC_IM_BASE_TYPEDEFS_H_

#define CMCC_IM_BASE_TYPEDEFS_H_

#if !defined(_MSC_VER)
#include <stdint.h>
#else
// Define C99 equivalent types, since pre-2010 MSVC doesn't provide stdint.h.
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef __int64             int64_t;
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned __int64    uint64_t;
#endif



#endif //CMCC_IM_BASE_TYPEDEFS_H_