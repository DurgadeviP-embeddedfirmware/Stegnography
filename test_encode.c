#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "decode.h"

int main(int argc ,EncodeInfo encode, char *argv[])
{
    if(check_operation_type(argv) == e_encode)
    {
        printf("Selected encoding\n");

       
        if(read_and_validate_encode_args( argv , &encode) == e_success)
        {
            printf("read and validate encode is success\n");

            printf("<----------------Started encoding---------------->\n");

            if(do_encoding(&encode) == e_success)
            {
                    printf("Encodeing is successfull\n");
            }
            else
            {
                printf("Failed to perform encoding operation\n");
                return -2;
            }

        }
        else
        {
            printf("read and validate encode is failure\n");
            return -1;
        }

    }
    else if(check_operation_type(argv) ==  e_decode)
    {
        printf("Selected decoding\n");
        DecodeInfo decInfo;
        if(read_and_validate_decode_args( argv ,&encode, &decInfo) == e_success)
        {
            printf("read and validate decode is success\n");

            printf("<----------------Started decoding---------------->\n");

            if(do_decoding(&decInfo) == e_success)
            {
                    printf("Decoding is successfull\n");
            }
            else
            {
                printf("Failed to perform Decoding operation\n");
                return -2;
            }
        }
    }
    else 
    {
        printf("Invalid Input\n");
        
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1] , "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)    
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}