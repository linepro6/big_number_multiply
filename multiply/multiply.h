#pragma once
#include "demical.h"

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

// 大整数加法（带移位操作）
Demical* demicalAdd(Demical* A, size_t aAppendZeroCount, Demical* B, size_t bAppendZeroCount)
{
	size_t lenA = A->digits + aAppendZeroCount;
	size_t lenB = B->digits + bAppendZeroCount;

	size_t minLen = lenA;
	if (lenB < minLen)
	{
		minLen = lenB;
	}
	size_t maxLen = lenA;
	Demical* maxTarget = A;
	size_t maxTargetAppendZero = aAppendZeroCount;
	if (lenB > maxLen)
	{
		maxLen = lenB;
		maxTarget = B;
		maxTargetAppendZero = bAppendZeroCount;
	}

	Demical* result = demicalInit(maxLen + 1, false);

	int addSign = 0;
	size_t i = 0;
	while (i < minLen)
	{
		int tempA = 0;
		if (i >= aAppendZeroCount)
		{
			tempA = A->content[i - aAppendZeroCount];
		}

		int tempB = 0;
		if (i >= bAppendZeroCount)
		{
			tempB = B->content[i - bAppendZeroCount];
		}

		int temp = tempA + tempB + addSign;
		if (temp >= 10)
		{
			addSign = 1;
			temp -= 10;
		}
		else
		{
			addSign = 0;
		}
		result->content[i] = temp;
		++i;
	}
	while (i < maxLen)
	{
		int temp = addSign;
		if (i >= maxTargetAppendZero)
		{
			temp += maxTarget->content[i - maxTargetAppendZero];
		}


		if (temp >= 10)
		{
			addSign = 1;
			temp -= 10;
		}
		else
		{
			addSign = 0;
		}
		result->content[i] = temp;
		++i;
	}
	result->content[i] = addSign;
	demicalFixDigits(result);
	return result;
}

// 大整数减法
Demical* demicalSub(Demical* A, Demical* B)
{
	size_t lenA = A->digits;
	size_t lenB = B->digits;

	size_t minLen = lenA;
	if (lenB < minLen)
	{
		minLen = lenB;
	}
	size_t maxLen = lenA;
	Demical* maxTarget = A;
	if (lenB > maxLen)
	{
		maxLen = lenB;
		maxTarget = B;
	}

	Demical* result = demicalInit(maxLen, false);

	int subSign = 0;
	size_t i = 0;
	while (i < minLen)
	{
		int temp = A->content[i] - B->content[i] - subSign;
		if (temp < 0)
		{
			subSign = 1;
			temp += 10;
		}
		else
		{
			subSign = 0;
		}
		result->content[i] = temp;
		++i;
	}
	while (i < maxLen)
	{
		int temp = maxTarget->content[i] - subSign;

		if (temp < 0)
		{
			subSign = 1;
			temp += 10;
		}
		else
		{
			subSign = 0;
		}
		result->content[i] = temp;
		++i;
	}
	demicalFixDigits(result);
	return result;
}

// 普通大整数乘法
Demical* demicalMultiplyStandard(Demical* A, Demical* B)
{
	if (A->digits <= 0 || B->digits <= 0)
	{
		return demicalInit(0, false);
	}
	Demical* result = demicalInit(A->digits + B->digits, true);
	for (size_t i = 0; i < A->digits; ++i)
	{
		int a = A->content[i];
		int signAdd = 0;
		for (size_t j = 0; j < B->digits; ++j)
		{
			int b = B->content[j];
			int temp = a * b + signAdd + result->content[i + j];
			result->content[i + j] = temp % 10;
			signAdd = temp / 10;
		}
		if (signAdd != 0) {
			result->content[i + B->digits] += signAdd;
		}
	}
	demicalFixDigits(result);
	return result;
}

// 大整数临时变量初始化
Demical* demicalInitTemp(byte* contentStart, size_t digits)
{
	Demical* dec = malloc(sizeof(Demical));
	dec->content = contentStart;
	dec->digits = digits;
	return dec;
}

// 改进的分治大整数乘法
Demical* demicalMultiplyDividedImproved(Demical* A, Demical* B)
{
	if (A->digits <= 128 || B->digits <= 128)
	{
		return demicalMultiplyStandard(A, B);
	}

	size_t targetDivideDigits = MIN(A->digits, B->digits) / 2;

	Demical* a1 = demicalInitTemp(A->content + targetDivideDigits, A->digits - targetDivideDigits);
	Demical* a2 = demicalInitTemp(A->content, targetDivideDigits);
	Demical* b1 = demicalInitTemp(B->content + targetDivideDigits, B->digits - targetDivideDigits);
	Demical* b2 = demicalInitTemp(B->content, targetDivideDigits);

	Demical* c0 = demicalMultiplyDividedImproved(a2, b2);
	Demical* c2 = demicalMultiplyDividedImproved(a1, b1);

	Demical* t1 = demicalAdd(a1, 0, a2, 0);
	Demical* t2 = demicalAdd(b1, 0, b2, 0);

	Demical* t3 = demicalMultiplyDividedImproved(t1, t2);
	Demical* t4 = demicalAdd(c0, 0, c2, 0);
	Demical* c1 = demicalSub(t3, t4);
	Demical* t5 = demicalAdd(c2, targetDivideDigits * 2, c1, targetDivideDigits);
	Demical* result = demicalAdd(t5, 0, c0, 0);

	free(a1);
	free(a2);
	free(b1);
	free(b2);

	demicalFree(c0);
	demicalFree(c1);
	demicalFree(c2);
	demicalFree(t1);
	demicalFree(t2);
	demicalFree(t3);
	demicalFree(t4);
	demicalFree(t5);

	demicalFixDigits(result);
	return result;
}

// 普通的分治大整数乘法
Demical* demicalMultiplyDividedStandard(Demical* A, Demical* B)
{
	if (A->digits <= 128 || B->digits <= 128)
	{
		return demicalMultiplyStandard(A, B);
	}

	size_t targetDivideDigits = MIN(A->digits, B->digits) / 2;

	Demical* a1 = demicalInitTemp(A->content + targetDivideDigits, A->digits - targetDivideDigits);
	Demical* a2 = demicalInitTemp(A->content, targetDivideDigits);
	Demical* b1 = demicalInitTemp(B->content + targetDivideDigits, B->digits - targetDivideDigits);
	Demical* b2 = demicalInitTemp(B->content, targetDivideDigits);

	Demical* a2b2 = demicalMultiplyDividedStandard(a2, b2);
	Demical* a1b2 = demicalMultiplyDividedStandard(a1, b2);
	Demical* a2b1 = demicalMultiplyDividedStandard(a2, b1);
	Demical* a1b1 = demicalMultiplyDividedStandard(a1, b1);

	Demical* t1 = demicalAdd(a1b2, 0, a2b1, 0);
	Demical* t2 = demicalAdd(a1b1, targetDivideDigits * 2, t1, targetDivideDigits);
	Demical* result = demicalAdd(t2, 0, a2b2, 0);

	free(a1);
	free(a2);
	free(b1);
	free(b2);
	demicalFree(a2b2);
	demicalFree(a1b2);
	demicalFree(a2b1);
	demicalFree(a1b1);
	demicalFree(t1);
	demicalFree(t2);
	return result;
}