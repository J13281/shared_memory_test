#include <stdio.h>
#include <windows.h>

#include "shared.h"

int main()
{
    puts("hello read_and_write");

    const int kSharedSize = 0x80;

    // open_or_create
    HANDLE hmapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, "read_and_write");
    if (hmapping == NULL)
    {
        hmapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, kSharedSize, "read_and_write");
    }

    // 実際にファイルの内容と対応するメモリ領域を取得する
    hello_message *pshared = MapViewOfFile(hmapping, FILE_MAP_ALL_ACCESS, 0, 0, kSharedSize);
    
    unsigned char buf[0x10];

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        printf("message: "); scanf("%s", buf);

        if (buf[0] == 'r')
        {
            printf("%d:%s\n",
                pshared->len, pshared->text);
        }
        else if (buf[0] == 'w')
        {
            pshared->len++;
            strcpy(pshared->text, buf);
        }
        else 
        {
            break;
        }
    }

    puts("exit app.");

    // 解放もしっかり
    UnmapViewOfFile(pshared);
    CloseHandle(hmapping);

    return 0;
}
