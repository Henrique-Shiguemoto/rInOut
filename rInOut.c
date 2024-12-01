#include "rInOut.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <windows.h>

#define _rio_min(a, b) ((a) > (b) ? (b) : (a))

typedef struct rio_file {
	char name[MAX_FILENAME_SIZE];
	HANDLE handle;
	unsigned int file_size; // in bytes
	int opening_mode;
	void* contents;
} rio_file;

rio_file* rio_open_file(const char* filename, RIO_FILE_OPEN_MODE mode){
	DWORD opening_mode = 0;

	if(mode & RIO_READ_MODE)  { 
		opening_mode |= GENERIC_READ;  
	}
	if(mode & RIO_WRITE_MODE) { 
		opening_mode |= GENERIC_WRITE; 
	}

	rio_file* result_file = (rio_file*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(rio_file));

	result_file->handle = CreateFileA(filename, opening_mode, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	int min_size = _rio_min(lstrlenA(filename), MAX_FILENAME_SIZE);
	CopyMemory(result_file->name, filename, min_size);
	result_file->file_size = GetFileSize(result_file->handle, NULL);
	result_file->opening_mode = mode;
	if(result_file->file_size == 0){
		// means we just created the file, didn't open an existing one
		result_file->contents = VirtualAlloc(0, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		return result_file;
	}else{
		result_file->contents = VirtualAlloc(0, result_file->file_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	}
	
	if(result_file->contents == NULL) { 
		return NULL; 
	}

	DWORD number_of_bytes_read = 0;
	ReadFile(result_file->handle, result_file->contents, result_file->file_size, &number_of_bytes_read, NULL);

	if(number_of_bytes_read != result_file->file_size) { 
		return NULL; 
	}
	
	return result_file;
}

int rio_write_file(rio_file* file, const void* contents, int count){
	SYSTEM_INFO sys;
	GetSystemInfo(&sys);
	int number_of_pages_now = (int)(file->file_size / sys.dwPageSize) + 1;
	int number_of_pages_needed = (int)(file->file_size + count / sys.dwPageSize) + 1;
	if(number_of_pages_needed > number_of_pages_now){
		// If we need more pages, we'll allocated more memory
		BOOL result_free = VirtualFree(file->contents, 0, MEM_RELEASE);
		file->contents = VirtualAlloc(0, file->file_size + count, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if(!result_free | (file->contents == NULL)){
			return 0;
		}
	}

	// appending contents to the end of file->contents
	CopyMemory((char*)(file->contents) + file->file_size, contents, count);

	file->file_size += count;

	return 1;
}

int rio_read_file(rio_file* file, void* dest, int count, int offset){
	return 0;
}

int rio_file_exists(const char* filename){
	return (lstrlenA(filename) < 256) && (GetFileAttributesA(filename) != INVALID_FILE_ATTRIBUTES);
}

void rio_close_file(rio_file* file){
	CloseHandle(file->handle);
	VirtualFree(file->contents, 0, MEM_RELEASE);
	file->contents = NULL;
	file->name[0] = 0;
	file->file_size = 0;
	file->opening_mode = RIO_INVALID_MODE;
}

int rio_save_changes(rio_file* file){
	DWORD bytes_written = 0;
	BOOL result_write = WriteFile(file->handle, file->contents, file->file_size, &bytes_written, NULL);
	if(result_write && bytes_written == file->file_size){
		return 1;
	}
	return 0;
}

int rio_save_and_close_file(rio_file* file){
	if(rio_save_changes(file)){
		rio_close_file(file);
		return 1;
	}
	return 0;
}

#elif defined(__linux__) || defined(__gnu_linux__)

#include <stdio.h>

typedef struct rio_file {
	char name[MAX_FILENAME_SIZE];
	unsigned int file_size; // in bytes
	int opening_mode;
	void* contents;
} rio_file;

rio_file* rio_open_file(const char* filename, RIO_FILE_OPEN_MODE mode){
	return NULL;
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

int rio_save_changes(rio_file* file){
	return 0;
}

void rio_close_file(rio_file* file){
	
}

int rio_save_and_close_file(rio_file* file){
	return 0;
}

#endif
