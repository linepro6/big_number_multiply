#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef unsigned char byte;
// 十进制大整数结构体
typedef struct
{
	byte* content;    // 字节数组，每一位占一个字节，小端存储
	size_t digits;    // 大整数位数
} Demical;

// 大整数打印输出
void demicalPrint(Demical* dec)
{
	byte* actualPtr = dec->content + dec->digits - 1;
	while (*actualPtr == 0) --actualPtr;
	if (actualPtr < dec->content)
	{
		putchar('0');
		return;
	}
	for (byte* i = actualPtr; i >= dec->content; --i)
	{
		putchar((*i) + '0');
	}
}

// 为大整数约简掉高位的 0
void demicalFixDigits(Demical* dec)
{
	if (dec->digits <= 0) return;
	byte* actualPtr = dec->content + dec->digits - 1;
	while (*actualPtr == 0) --actualPtr;
	dec->digits = actualPtr - dec->content + 1;
}

// 大整数内存分配
Demical* demicalInit(size_t digits, bool zeroInit)
{
	Demical* dec = malloc(sizeof(Demical));
	dec->digits = digits;
	if (digits == 0)
	{
		dec->content = NULL;
		return dec;
	}
	dec->content = malloc(digits);
	if (zeroInit)
	{
		memset(dec->content, 0, digits);
	}
	return dec;
}

// 大整数内存释放
void demicalFree(Demical* dec)
{
	if (dec->content) {
		free(dec->content);
	}
	free(dec);
}

// 从字符串中初始化一个大整数
Demical* demicalFromCString(char* str)
{
	Demical* dec = malloc(sizeof(Demical));
	size_t len = strlen(str);
	dec->digits = 0;
	char* ch = str + len - 1;
	if (ch < str)
	{
		dec->content = NULL;
		return dec;
	}
	dec->content = malloc(len);
	while (ch >= str)
	{
		dec->content[dec->digits] = *ch - '0';
		++dec->digits;
		--ch;
	}
	return dec;
}

// 随机生成指定位数的大整数
Demical* demicalRand(size_t digits)
{
	Demical* dec = demicalInit(digits, false);
	if (digits <= 0)
	{
		return dec;
	}
	byte* i = dec->content;
	for (; (size_t)(i - dec->content) < (size_t)(digits - 1); ++i)
	{
		*i = rand() % 10;
	}
	*i = rand() % 9 + 1;
	return dec;
}

// 大整数相等比较
bool demicalEquals(Demical* dec1, Demical* dec2)
{
	demicalFixDigits(dec1);
	demicalFixDigits(dec2);
	if (dec1->digits != dec2->digits)
	{
		return false;
	}
	return (memcmp(dec1->content, dec2->content, dec1->digits) == 0);
}