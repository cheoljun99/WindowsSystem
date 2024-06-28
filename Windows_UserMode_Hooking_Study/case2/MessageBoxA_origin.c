#include <stdio.h>
#include <windows.h>


int main(int argc, char* argv[])
{
   
    printf("Press the any key...");
    getchar();

    MessageBoxA(NULL, "Not Hooked...!", "Trampoline Hook", 0);// MessageBoxA ½ÇÇà

    return 0;
}