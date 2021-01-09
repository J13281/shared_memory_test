#include <stdio.h>
#include <windows.h>

typedef struct {
    unsigned char len;
    char buffer[0x39];
} my_struct;

int main_loop(my_struct* pshared)
{
    struct {
        unsigned char mode;
        char text[0x39];
    } buf;

    memset(&buf, 0, sizeof(buf));
    printf("->"); scanf("%s", &buf);

    if (buf.mode == 'r')
    {
        printf("%d::%s\n", pshared->len, pshared->buffer);
        return 1;
    }
    else if (buf.mode == 'w')
    {
        pshared->len++;
        strcpy(pshared->buffer, buf.text);
        return 1;
    }
    else
    {
        // exit
        return 0;
    }
}

int main()
{
    puts("hello read_and_write");

    const int kSharedSize = sizeof(my_struct);
    const char* key = "read_and_write";

    // 自分が最初なら作る
    HANDLE hmapping = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, key);
    if (hmapping == NULL)
    {
        hmapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, kSharedSize, key);
    }

    // 実際にファイルの内容と対応するメモリ領域を取得する
    my_struct *pshared = MapViewOfFile(hmapping, FILE_MAP_ALL_ACCESS, 0, 0, kSharedSize);

    // main loop
    while (main_loop(pshared));

    puts("exit app.");

    // 解放もしっかり
    UnmapViewOfFile(pshared);
    CloseHandle(hmapping);

    return 0;
}
