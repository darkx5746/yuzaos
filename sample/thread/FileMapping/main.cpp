#include <windef.h>
#include <minwindef.h>
#include <minwinconst.h>
#include <winapi.h>

int main(int argc, char* argv[])
{
	HANDLE hFile = CreateFile("MMFTest.dat",
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);

   HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE,  0,  100,  NULL);

    CloseFile(hFile);

	return 0;
}