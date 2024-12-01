#include "rInOut.h"

#define check_if_zero_then_exit(x) if((x)) { return 1; }

int main(void){
	// Opening a file, if the file doesn't exist, it creates an empty one
	rio_file* handle0 = rio_open_file("test0.txt", RIO_READ_MODE | RIO_WRITE_MODE);
	check_if_zero_then_exit(!handle0)
	
	// Writing to memory, not being persisted
	const char* sample_content0 = "Hello World - This string will not be persisted!\n";
	check_if_zero_then_exit(!rio_write_file(handle0, sample_content0, 49))
	
	// Reading from memory, not from disk
	char dest0[1024] = {0};
	check_if_zero_then_exit(!rio_read_file(handle0, dest0, 49, 0))
	
	rio_close_file(handle0); // just closes handle and zeros out everything

	// -------------------------------------------------------------------------------

	rio_file* handle1 = rio_open_file("test1.txt", RIO_READ_MODE | RIO_WRITE_MODE);
	check_if_zero_then_exit(!handle1)
	
	const char* sample_content1 = "Hello World - This string will be persisted!\n";
	check_if_zero_then_exit(!rio_write_file(handle1, sample_content1, 45))
	
	char dest1[1024] = {0};
	check_if_zero_then_exit(!rio_read_file(handle1, dest1, 45, 0))
	
	// since we're calling rio_save_changes(), changes will be persisted
	// If you want to save changes but not close the file handle, use rio_save_changes()
	rio_save_and_close_file(handle1);

	return 0;
}
