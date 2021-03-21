//
// Created by yangwenfang on 2019/3/19.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "interf_amr.h"
#include "interf_dec.h"
#include "interf_enc.h"

using namespace std;

const int sizes[] = {12, 13, 15, 17, 19, 20, 26, 31, 5, 6, 5, 5, 0, 0, 0, 0};

enum Mode findMode(const char *str) {
    struct {
        enum Mode mode;
        int rate;
    } modes[] = {
            {MR475, 4750},
            {MR515, 5150},
            {MR59,  5900},
            {MR67,  6700},
            {MR74,  7400},
            {MR795, 7950},
            {MR102, 10200},
            {MR122, 12200}
    };
    int rate = atoi(str);
    int closest = -1;
    int closestdiff = 0;
    unsigned int i;
    for (i = 0; i < sizeof(modes) / sizeof(modes[0]); i++) {
        if (modes[i].rate == rate)
            return modes[i].mode;
        if (closest < 0 || closestdiff > abs(modes[i].rate - rate)) {
            closest = i;
            closestdiff = abs(modes[i].rate - rate);
        }
    }
    fprintf(stderr, "Using bitrate %d\n", modes[closest].rate);
    return modes[closest].mode;
}

int sdkpcm2amr(const char *amr_file/*å¾…å†™å…¥çš„æ–‡ä»¶*/, readcbfunctype read_cb)  /* use speech mode */
{
    enum Mode mode = MR122;
    int dtx = 0;
    FILE *out;
    void *amr;
    int sampleRate = 8000, channels = 1, bitsPerSample = 16;
    int inputSize;
    bool end = false;
    bool cancelFlag = false;
    unsigned char *inputBuf;
    inputSize = channels * 2 * 160;
    inputBuf = (unsigned char *) malloc(inputSize);
    amr = IM_Encoder_Interface_init(dtx);
    out = fopen(amr_file, "wb");
    if (!out) {
        free(inputBuf);
        perror(amr_file);
        return 1;
    }
    fwrite("#!AMR\n", 1, 6, out);
    while (1 && !end) {
        short buf[160];
        unsigned char outbuf[500];
        int read,i, n;
        read_cb((int16_t *)inputBuf, inputSize, &end, &cancelFlag);
        if (cancelFlag)
        {
            break;  
        }
        read = inputSize;
        read /= channels;
        read /= 2;
        if (read < 160) {
            break;
        }
        for (i = 0; i < 160; i++) {
            const unsigned char *in = &inputBuf[2 * channels * i];
            buf[i] = in[0] | (in[1] << 8);
        }
        n = IM_Encoder_Interface_Encode(amr, mode, buf, outbuf, 0);
        fwrite(outbuf, 1, n, out);
    }
    free(inputBuf);
    fclose(out);

    /* Èç¹û·µ»ØÎªTRUE£¬ËµÃ÷µ÷ÓÃ·½Ö÷¶¯Í£Ö¹£¬É¾³ýÒÑ¾­´´½¨µÄÎÄ¼þ */
    if (cancelFlag)
    {
        if (remove(amr_file) != 0)
        {
            printf("pcm2amr remove: %s error", amr_file);        
        } 
    }
    
    IM_Encoder_Interface_exit(amr);

    return 0;
}

int sdkamr2pcm(const char *amr_file, writecbfunctype write_cb) {
    int8_t header[6];
    int n;
    bool cancelFlag = false;
    
    FILE *in = fopen(amr_file, "rb");
    if (!in) {
        return -1;
    }
    n = fread(header, 1, 6, in);
    if (n != 6 || memcmp(header, "#!AMR\n", 6)) {
        return -1;
    }
    void *amr;
    amr = IM_Decoder_Interface_init();
    while (1) {
        unsigned char buffer[500], littleendian[320], *ptr;
        int size, i;
        int16_t outbuffer[160];
        /* Read the mode byte */
        n = fread(buffer, 1, 1, in);
        if (n <= 0) {
            break;
        }
        size = sizes[(buffer[0] >> 3) & 0x0f];
        n = fread(buffer + 1, 1, size, in);
        if (n != size) {
            break;
        }
        IM_Decoder_Interface_Decode(amr, buffer, outbuffer, 0);
        ptr = littleendian;
        for (i = 0; i < 160; i++) {
            *ptr++ = (outbuffer[i] >> 0) & 0xff;
            *ptr++ = (outbuffer[i] >> 8) & 0xff;
        }
        write_cb((int16_t *)littleendian, 320, false, &cancelFlag);

        if (cancelFlag)
        {
            break;
        }
    }
    
    fclose(in);
    IM_Decoder_Interface_exit(amr);

    if (!cancelFlag)
    {
        write_cb(0, 0, true, &cancelFlag);
    }
    
    return 0;
}
