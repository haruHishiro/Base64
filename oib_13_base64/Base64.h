#pragma once
class Base64 {
public:
	Base64();
	Base64(char mode);
	void decode(char* string, unsigned char str_len, char* ret_arr);
	void encode(char* string, unsigned char str_len, char* ret_arr);
private:
	char getIndex(char chr);
	unsigned __int32 get_Int32(char* bytes);
	void getChars(unsigned char* chrs, char* chars);
	char abc[64];
};

