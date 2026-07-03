#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include "encode.h"
#include<stdio.h>
typedef struct _DecodeInfo
{
    char * D_decode_file_name;
    FILE * D_fptr_decode_file;
    char * D_stego_file_name;
    FILE * D_fptr_stego_file;
    /* Meta Data info*/
    char * D_magic_string;
    int D_secret_extn_size;
    char * D_secret_extn;
    int D_file_size;
    char * D_file_data;
} DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[],EncodeInfo * encInfo, DecodeInfo *decInfo);


/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string( DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn( DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size( DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode lsb bits to bytes*/
Status decode_lsb_to_byte(char* str);

#endif