#include "rInOut.h"
#include <windows.h>
#include <stdio.h>

int main(void){
	rio_file* handle = rio_open_file("test.txt", RIO_READ_MODE | RIO_WRITE_MODE);
	
	const char* sample_content = "Hello World!\n";

	if(!rio_write_file(handle, sample_content, 13)){
		return 1;
	}
	
	char dest[1024] = {0};
	if(!rio_read_file(handle, dest, 13, 0)){
		return 1;
	}
	printf("Size: %d\n", lstrlenA(dest));
	printf("Dest: %s\n", dest);

	rio_save_and_close_file(handle);
	return 0;
}
