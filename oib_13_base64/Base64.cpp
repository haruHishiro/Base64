#include "Base64.h"

Base64::Base64() {
	char hardcodeAbc[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"    // 26
		"abcdefghijklmnopqrstuvwxyz"   // 26
		"0123456789"                  // 10
		"-_";                        // 2

	for (char i = 0; i < 64; i++) {
		Base64::hardcode_abc[i] = hardcodeAbc[i];
	}
}

Base64::Base64(char mode) {
	char hardcodeAbc[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"    // 26
		"abcdefghijklmnopqrstuvwxyz"   // 26
		"0123456789"                  // 10
		"-_";                        // 2

	for (char i = 0; i < 64; i++) {
		Base64::hardcode_abc[i] = hardcodeAbc[i];
	}

	if (mode) {
		Base64::hardcode_abc[62] = '+';
		Base64::hardcode_abc[62] = '/';
	}
}

void Base64::decode(char* string, unsigned char str_len, char* ret_arr) {
	unsigned blocks = str_len >> 2;

	unsigned char _4_bytes[4];
	unsigned offset;
	char f = 0;

	for (offset = 0; offset < blocks; offset++) {
		char _offset = offset << 2;
		_4_bytes[0] = Base64::getIndex(string[0 + _offset]);
		_4_bytes[1] = Base64::getIndex(string[1 + _offset]);
		if (string[2 + _offset] == '=' || string[3 + _offset] == '=') {
			//printf("here\n");
			f = 1;
			break;
		}
		_4_bytes[2] = Base64::getIndex(string[2 + _offset]);
		_4_bytes[3] = Base64::getIndex(string[3 + _offset]);

		char arr[3];
		Base64::getChars(_4_bytes, arr);

		_offset -= offset;
		ret_arr[0 + _offset] = arr[0];
		ret_arr[1 + _offset] = arr[1];
		ret_arr[2 + _offset] = arr[2];
	}

	if (f) {
		char f2 = 0;
		char _offset = offset << 2;
		if (string[2 + _offset] != '=') {
			_4_bytes[2] = Base64::getIndex(string[2 + _offset]);
			f2 = 1;
		}
		_4_bytes[3] = 0;

		char arr[3];
		Base64::getChars(_4_bytes, arr);

		_offset -= offset;
		ret_arr[0 + _offset] = arr[0];
		if (f2) {
			ret_arr[1 + _offset] = arr[1];
		}
		ret_arr[2 + _offset] = 0;
	}
}

void Base64::encode(char* string, unsigned char str_len, char* ret_arr) {
	char _3_bytes[3];

	unsigned char full_blocks = str_len / 3;
	unsigned char paddings = 0;
	if (str_len % 3) {
		paddings = 3 - (str_len % 3);
	}

	char offset;
	for (offset = 0; offset < full_blocks; offset++) {
		char _offset = offset * 3;
		_3_bytes[0] = string[0 + _offset];
		_3_bytes[1] = string[1 + _offset];
		_3_bytes[2] = string[2 + _offset];

		unsigned __int32 i32 = Base64::get_Int32(_3_bytes);

		_offset += offset;
		char index = i32 >> 24;
		ret_arr[0 + _offset] = Base64::hardcode_abc[index];
		index = (i32 >> 16) & 0xff;
		ret_arr[1 + _offset] = Base64::hardcode_abc[index];
		index = (i32 >> 8) & 0xff;
		ret_arr[2 + _offset] = Base64::hardcode_abc[index];
		ret_arr[3 + _offset] = Base64::hardcode_abc[i32 & 0xff];
	}

	if (paddings) {
		char _offset = offset * 3;
		_3_bytes[0] = string[0 + _offset];
		_3_bytes[1] = 0;
		_3_bytes[2] = 0;
		if (paddings == 1) {
			_3_bytes[1] = string[1 + _offset];
		}

		unsigned __int32 i32 = Base64::get_Int32(_3_bytes);

		_offset += offset;

		char index = i32 >> 24;
		ret_arr[0 + _offset] = Base64::hardcode_abc[index];
		index = (i32 >> 16) & 0xff;
		ret_arr[1 + _offset] = Base64::hardcode_abc[index];
		index = (i32 >> 8) & 0xff;
		ret_arr[2 + _offset] = Base64::hardcode_abc[index];

		if (paddings == 2) {
			ret_arr[2 + _offset] = '=';
		}
		ret_arr[3 + _offset] = '=';
	}
}

char Base64::getIndex(char chr) {
	if (chr >= 'A' && chr <= 'Z') return chr - 'A';
	else if (chr >= 'a' && chr <= 'z') return chr - 'a' + 26;
	else if (chr >= '0' && chr <= '9') return chr - '0' + 52;
	else if (chr == '-' || chr == '+') return 62;
	else if (chr == '_' || chr == '/') return 63;
	else return -1;
}

unsigned __int32 Base64::get_Int32(char* bytes) {
	unsigned __int32 i32;
	unsigned __int8 i8;
	i8 = bytes[0] >> 2;
	i32 = i8 << 24;

	i8 = bytes[0] & 0x3;
	i8 <<= 4;
	i8 += (bytes[1] & 0xf0) >> 4;
	i32 += i8 << 16;

	i8 = bytes[1] & 0xf;
	i8 <<= 2;
	i8 += (bytes[2] & 0xc0) >> 6;
	i32 += i8 << 8;

	i8 = bytes[2] & 0x3f;
	i32 += i8;

	return i32;
}

void Base64::getChars(unsigned char* chrs, char* chars) {
	char chr1, chr2, chr3;

	chr1 = (chrs[0] << 2) + (chrs[1] >> 4);
	chr2 = (chrs[1] << 4) + (chrs[2] >> 2);
	chr3 = (chrs[2] << 6) + (chrs[3]);

	chars[0] = chr1;
	chars[1] = chr2;
	chars[2] = chr3;
}
