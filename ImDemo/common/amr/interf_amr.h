//
// Created by yangwenfang on 2019/3/19.
//

#ifndef ALIOS_AMR_ENCODE_INTERF_AMR_H
#define ALIOS_AMR_ENCODE_INTERF_AMR_H

#include <stdbool.h>
#include "errorcode.h"
#include <functional>

using namespace std;

typedef std::function<void (const int16_t * buf, int length, bool* is_end, bool* cancel)> readcbfunctype;    
typedef std::function<void (const int16_t * buf, int length, bool is_end, bool* cancel)> writecbfunctype;   

int sdkpcm2amr(const char *amr_file, readcbfunctype read_cb);   /* use speech mode */

int sdkamr2pcm(const char *amr_file, writecbfunctype write_cb);   /* use speech mode */

#endif //ALIOS_AMR_ENCODE_INTERF_AMR_H
