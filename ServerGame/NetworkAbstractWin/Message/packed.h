//
// Created by wurmel_a on 25/09/17.
//

#ifndef _PACKED_H_
#define _PACKED_H_

#ifdef _MSC_VER
# define PACKED_STRUCT
#pragma pack(push, 1)
#elif defined(__GNUC__)
#define PACKED_STRUCT __attribute__((packed))
#endif

#endif //_PACKED_H_
