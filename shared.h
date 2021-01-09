typedef struct _hello_message
{
    unsigned char len;
    unsigned char text[0x0F];
} hello_message;
