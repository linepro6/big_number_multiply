#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef unsigned char byte;
// ʮ���ƴ������ṹ��
typedef struct
{
	byte* content;    // �ֽ����飬ÿһλռһ���ֽڣ�С�˴洢
	size_t digits;    // ������λ��
} Demical;

// ��������ӡ���
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

// Ϊ������Լ�����λ�� 0
void demicalFixDigits(Demical* dec)
{
	if (dec->digits <= 0) return;
	byte* actualPtr = dec->content + dec->digits - 1;
	while (*actualPtr == 0) --actualPtr;
	dec->digits = actualPtr - dec->content + 1;
}

// �������ڴ����
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

// �������ڴ��ͷ�
void demicalFree(Demical* dec)
{
	if (dec->content) {
		free(dec->content);
	}
	free(dec);
}

// ���ַ����г�ʼ��һ��������
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

// �������ָ��λ���Ĵ�����
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

// ��������ȱȽ�
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