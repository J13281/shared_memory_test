#include <stdio.h>
#include <windows.h>

#include "shared.h"

int main()
{
    puts("hello main");

    const int kSharedSize = 128;

    // ファイルとメモリの対応関係を管理するオブジェクトを作成する
    HANDLE hmapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, kSharedSize, "my_shared");

    // 実際にファイルの内容と対応するメモリ領域を取得する
    void *pshared = MapViewOfFile(hmapping, FILE_MAP_ALL_ACCESS, 0, 0, kSharedSize);

    *(hello_message *)pshared = (hello_message) {
        .len = 5,
        .text = "wa-i",
    };

    puts("change?"); (void)getchar();
    *(hello_message *)pshared = (hello_message) {
        .len = 5,
        .text = "changed",
    };
    puts("end?"); (void)getchar();

    // 解放もしっかり
    UnmapViewOfFile(pshared);
    CloseHandle(hmapping);

    return 0;
}
