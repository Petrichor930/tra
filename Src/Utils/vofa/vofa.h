#ifndef VOFA_H
#define VOFA_H

#ifdef USE_VOFA
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

void Vofa_FireWater(const char *format, ...);
void Vofa_JustFloat(float *_data, uint8_t _num);

#ifdef __cplusplus
}
#endif
#endif
#endif
