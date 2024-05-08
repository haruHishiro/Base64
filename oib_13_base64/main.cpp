#include <stdio.h>
#include <malloc.h>
#include "Base64.h"


int main() {
	Base64* b64 = new Base64();

	char ab[] = "1234t";
	char str_len = 5;
	char* ret_arr;
	char* rt_arr;

	unsigned char size = str_len / 3;

	if (str_len % 3) {
		size++;
	}

	size = size << 2;
	size++;
	
	ret_arr = (char*)malloc(size * sizeof(char));
	
	b64->encode(ab, str_len, ret_arr);
	ret_arr[size - 1] = 0;
	printf("%s\n", ret_arr);

	rt_arr = (char*)malloc((str_len + 1) * sizeof(char));
	b64->decode(ret_arr, size - 1, rt_arr);
	rt_arr[str_len] = 0;
	printf("%s", rt_arr);

	delete b64;
	free(rt_arr);
	free(ret_arr);
	return 0;
}
