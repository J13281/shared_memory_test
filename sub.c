#include <stdio.h>
#include <windows.h>
#include "shared.h"

int main()
{
    puts("hello sub");

    // 指定された名前をもつマッピングオブジェクトを引っ張ってくる
    HANDLE hmapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "my_shared");

    // ファイル全体をマッピングする
    void *pshared = MapViewOfFile(hmapping, FILE_MAP_READ, 0, 0, 0);

    if (pshared == NULL)
    {
        puts("not open");
        return 0;
    }
    while (getchar() != 'q') {
        printf("len=%d, text=%s\n",
            ((hello_message *)pshared)->len,
            ((hello_message *)pshared)->text);
    }

    UnmapViewOfFile(pshared);
    CloseHandle(hmapping);
}
