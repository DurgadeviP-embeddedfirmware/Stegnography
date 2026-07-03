#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>
#include "common.h"

Status open_decode_files(DecodeInfo *decInfo)
{
        // Stego Image file
    decInfo->D_fptr_stego_file = fopen(decInfo->D_stego_file_name, "r");
    if(decInfo->D_fptr_stego_file == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->D_stego_file_name);
        return e_failure;
    }    
    decInfo->D_fptr_decode_file = fopen(decInfo->D_decode_file_name, "w");
    // Do Error handling
    if (decInfo->D_fptr_decode_file == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->D_decode_file_name);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_decode_args(char *argv[],EncodeInfo *encInfo, DecodeInfo *decInfo)
{
    if(strstr(argv[2] ,".bmp" ) != NULL)
    {
        decInfo->D_stego_file_name = argv[2];
    }
    else
    {
        printf("Invalid stego file name (.bmp)\n");
        return e_failure;
    }
    if(argv[3] != NULL)
    {
        if(strstr(argv[3] , ".txt") != NULL)
        {
            decInfo->D_decode_file_name = argv[3];
        }
    }
    else
    {
        decInfo->D_decode_file_name = "decode.txt";
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo) == e_success)
    {
        printf("Open files is success\n");
    }
    else
    {
        printf("Open files is failure\n");
        return e_failure;
    }
    if(decode_magic_string(decInfo) == e_success)
    {
        printf("Decode magic string is successfull\n");
    }
    else
    {
        printf("Failed to decode magic string\n");
        return e_failure;
    }
    if(decode_secret_file_extn_size(decInfo) == e_success)
    {
        printf("Decode file extension size is successfull\n");
    }
    else
    {
        printf("Failed to decode file extension size\n");
        return e_failure;
    }
    if(decode_secret_file_extn(decInfo) == e_success)
    {
        printf("Decode file extension is successfull\n");
    }
    else
    {
        printf("Failed to decode file extension\n");
        return e_failure;
    }
    if(decode_secret_file_size(decInfo) == e_success)
    {
        printf("Decode file size is successfull\n");
    }
    else
    {
        printf("Failed to decode file size\n");
        return e_failure;
    }
    if(decode_secret_file_data(decInfo) == e_success)
    {
        printf("Decoded File Data successfully\n");
    }
    else
    {
        printf("Failed to decode file data \n");
        return e_failure;
    }
    return e_success;
}

Status decode_secret_file_extn_size( DecodeInfo *decInfo)
{
    char str[32];
    int val = 0;
        fread(str , 32, 1, decInfo->D_fptr_stego_file);
        for(int i = 0; i < 32; i++)
        {
           val = ( ((str[i] & 1) << i) | val) ;
        }
        decInfo->D_secret_extn_size = val;
        //printf("\t val is %d\n", decInfo->D_secret_extn_size);
    return e_success;    
}

Status decode_secret_file_extn( DecodeInfo *decInfo)
{
    char str1[5];
    char str2[8];
    int i ;
    for( i = 0; i < decInfo->D_secret_extn_size; i++)
    {
        fread(str2 , 8, 1, decInfo->D_fptr_stego_file);
        decode_lsb_to_byte(str2);
        str1[i] = str2[0];
    }
    str1[i] = '\0';
    decInfo->D_secret_extn = str1;
    //printf("\t%s\n", decInfo->D_secret_extn);
    if(strcmp(decInfo->D_secret_extn , ".txt") != 0)
    {
        printf("Enter .txt file \nFor Decoding : ./a.out -d stego.bmp [decode.txt]\n");
        return e_failure;
    }

    return e_success;
}

Status decode_secret_file_size( DecodeInfo *decInfo)
{
    char str[32];
    int val = 0;
        fread(str , 32, 1, decInfo->D_fptr_stego_file);
        for(int i = 0; i < 32; i++)
        {
           val = ( ((str[i] & 1) << i) | val) ;
        }
        decInfo->D_file_size = val;
        //printf("\t data size is %d\n", decInfo->D_file_size);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char str1[25];
    char str2[8];
    int i ;
    for( i = 0; i < decInfo->D_file_size; i++)
    {
        fread(str2 , 8, 1, decInfo->D_fptr_stego_file);
        decode_lsb_to_byte(str2);
        str1[i] = str2[0];
    }
    str1[i] = '\0';
    decInfo->D_file_data = str1;
    //printf("\t%s\n",decInfo->D_file_data);
    fwrite(str1, 25, 1, decInfo->D_fptr_decode_file);
    /*if(strcmp(decInfo->D_file_data , MAGIC_STRING) != 0)
    {
        return e_failure;
    }*/

    return e_success;
}

Status decode_magic_string( DecodeInfo *decInfo)
{
    fseek(decInfo->D_fptr_stego_file , 54, SEEK_SET);
    char str1[3];
    char str2[8];
    int i ;
    for( i = 0; i < 2; i++)
    {
        fread(str2 , 8, 1, decInfo->D_fptr_stego_file);
        decode_lsb_to_byte(str2);
        str1[i] = str2[0];
    }
    str1[i] = '\0';
    decInfo->D_magic_string = str1;
    //printf("\t%s\n",decInfo->D_magic_string);
    if(strcmp(decInfo->D_magic_string , MAGIC_STRING) != 0)
    {
        return e_failure;
    }

    return e_success;
}

Status decode_lsb_to_byte(char* str)
{
    char ch = 0;
    for(int i = 0; i < 8; i++)
    {
       ch = (((str[i] & 1) << i) | ch );
       //printf("\tstr[i] = %x and ch = %x\n", str[i], ch);
    }
    str[0] = ch;
    //printf("\tfinal value : %c\n", str[0]);
    return e_success;
}