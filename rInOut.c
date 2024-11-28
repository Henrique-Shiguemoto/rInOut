#include "rInOut.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <windows.h>

#define _rio_min(a, b) ((a) > (b) ? (b) : (a))

rio_file rio_open_file(const char* filename, RIO_FILE_OPEN_MODE mode){
	DWORD opening_mode = 0;

	if(mode & RIO_READ_MODE)  { opening_mode |= GENERIC_READ;  }
	if(mode & RIO_WRITE_MODE) { opening_mode |= GENERIC_WRITE; }

	HANDLE file_handle = CreateFileA(filename, opening_mode, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	rio_file result_file = (rio_file){0};
	CopyMemory(result_file.name, filename, _rio_min(lstrlenA(filename), MAX_FILENAME_SIZE));
	result_file.file_size = GetFileSize(file_handle, NULL);
	result_file.opening_mode = mode;
	result_file.contents = VirtualAlloc(NULL, result_file.file_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(result_file.contents == NULL) { return (rio_file){0}; }

	DWORD number_of_bytes_read = 0;
	ReadFile(file_handle, result_file.contents, result_file.file_size, &number_of_bytes_read, NULL);

	CloseHandle(file_handle);

	if(number_of_bytes_read != result_file.file_size) { return (rio_file){0}; }
	
	return result_file;
}

int rio_write_file(rio_file* file, const void* contents, int count){
	return 0;
}

int rio_read_file(rio_file* file, void* dest, int count, int offset){
	return 0;
}

int rio_file_exists(const char* filename){
	return 0;
}

void rio_close_file(rio_file* file){
	VirtualFree(file->contents, file->file_size, MEM_DECOMMIT);
	file->name[0] = 0;
	file->file_size = 0;
	file->opening_mode = RIO_INVALID_MODE;
}

#elif defined(__linux__) || defined(__gnu_linux__)

#include <stdio.h>

rio_file rio_open_file(const char* filename, RIO_FILE_OPEN_MODE mode){
	return {0};
}

int rio_write_file(rio_file* file, const void* contents, int count){
	return 0;
}

int rio_read_file(rio_file* file, void* dest, int count, int offset){
	return 0;
}

int rio_file_exists(const char* filename){
	return 0;
}

void rio_close_file(rio_file* file){
	
}

#endif
