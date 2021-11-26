#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>

inline int charToInt(char ch)
{
	return ch - '0';
}

char* initCharArray(size_t size)
{
	char* result = malloc(size);
	memset(result, -1, size);
	return result;
}

size_t charArrayLen(char* arr)
{
	size_t len = 0;
	while (arr[len] != -1) ++len;
	while (arr[len - 1] == 0) --len;
	return len;
}

char* transResultToCharArray(char* result)
{
	size_t len = charArrayLen(result);
	if (len == 0)
	{
		strcpy(result, "0");
		return result;
	}
	bool divide = (len % 2 == 0);
	for (size_t i = 0; i < len / 2; ++i) {
		size_t j = len - i - 1;
		char temp = result[i] + '0';
		result[i] = result[j] + '0';
		result[j] = temp;
	}
	if (!divide) {
		result[len / 2] += '0';
	}
	result[len] = 0;
	return result;
}

char* multiplyStandard(char* A, char* B)
{
	if (strcmp(A, "0") == 0 || strcmp(B, "0") == 0)
	{
		char* result = malloc(2);
		strcpy(result, "0");
		return result;
	}
	size_t lenA = strlen(A);
	size_t lenB = strlen(B);
	char* result = initCharArray(lenA + lenB + 1);
	for (size_t i = 0; i < lenA; ++i)
	{
		int a = charToInt(A[lenA - i - 1]);
		int signAdd = 0;
		for (size_t j = 0; j < lenB; ++j)
		{
			if (result[i + j] == -1) result[i + j] = 0;
			int b = charToInt(B[lenB - j - 1]);
			int temp = a * b + signAdd + result[i + j];
			result[i + j] = temp % 10;
			signAdd = temp / 10;
		}
		if (signAdd != 0) {
			if (result[i + lenB] == -1) result[i + lenB] = 0;
			result[i + lenB] += signAdd;
		}
	}
	return transResultToCharArray(result);
}

char* charAppendZero(char* arr, size_t zeroCount)
{
	size_t len = strlen(arr);
	for (size_t i = 0; i < zeroCount; ++i)
	{
		arr[len + i] = '0';
	}
	arr[len + zeroCount] = '\0';
	return arr;
}

char* add(char* A, size_t actLenA, size_t aAppendZeroCount, char* B, size_t actLenB, size_t bAppendZeroCount)
{
	size_t lenA = actLenA + aAppendZeroCount;
	size_t lenB = actLenB + bAppendZeroCount;

	size_t minLen = lenA;
	if (lenB < minLen)
	{
		minLen = lenB;
	}
	size_t maxLen = lenA;
	char* maxTarget = A;
	size_t maxActLen = actLenA;
	if (lenB > maxLen)
	{
		maxLen = lenB;
		maxTarget = B;
		maxActLen = actLenB;
	}

	// size_t initSize = maxLen + 1;

	char* result = malloc(maxLen + 2);
	memset(result, '0', maxLen + 1);
	result[maxLen + 1] = '\0';

	int addSign = 0;
	size_t i = 0;
	while (i < minLen)
	{
		int tempA = 0;
		if (lenA - 1 - i <= actLenA - 1)
		{
			tempA = charToInt(A[lenA - 1 - i]);
		}

		int tempB = 0;
		if (lenB - 1 - i <= actLenB - 1)
		{
			tempB = charToInt(B[lenB - 1 - i]);
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
		result[maxLen - i] = temp + '0';
		++i;
	}
	while (i < maxLen)
	{
		int temp = addSign;
		if (maxLen - 1 - i <= maxActLen - 1)
		{
			temp += charToInt(maxTarget[maxLen - 1 - i]);
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
		result[maxLen - i] = temp + '0';
		++i;
	}
	if (addSign != 0)
	{
		result[maxLen - i] = addSign + '0';
	}
	return result;
}

char* sub(char* A, char* B)
{
	size_t lenA = strlen(A);
	size_t lenB = strlen(B);

	size_t minLen = lenA;
	if (lenB < minLen)
	{
		minLen = lenB;
	}
	size_t maxLen = lenA;
	char* maxTarget = A;
	if (lenB > maxLen)
	{
		maxLen = lenB;
		maxTarget = B;
	}

	char* result = malloc(maxLen + 1);
	memset(result, '0', maxLen);
	result[maxLen] = '\0';

	int subSign = 0;
	size_t i = 0;
	while (i < minLen)
	{
		int tempA = charToInt(A[lenA - 1 - i]);
		int tempB = charToInt(B[lenB - 1 - i]);

		int temp = tempA - tempB - subSign;
		if (temp < 0)
		{
			subSign = 1;
			temp += 10;
		}
		else
		{
			subSign = 0;
		}
		result[maxLen - 1 - i] = temp + '0';
		++i;
	}
	while (i < maxLen)
	{
		int temp = charToInt(maxTarget[maxLen - 1 - i]) - subSign;

		if (temp < 0)
		{
			subSign = 1;
			temp += 10;
		}
		else
		{
			subSign = 0;
		}
		result[maxLen - 1 - i] = temp + '0';
		++i;
	}
	return result;
}

char* multiplyDivideStandard(char* A, size_t lowA, size_t highA, char* B, size_t lowB, size_t highB)
{
	// printf("(%zd, %zd), (%zd, %zd)\n", lowA, highA, lowB, highB);
	size_t lenA = highA - lowA + 1;
	size_t lenB = highB - lowB + 1;

	if (lenA <= 2 && lenB <= 2)
	{
		char* result = malloc(9);
		int tempA = charToInt(A[highA]);
		if (lenA == 2) tempA += 10 * charToInt(A[lowA]);
		int tempB = charToInt(B[highB]);
		if (lenB == 2) tempB += 10 * charToInt(B[lowB]);
		int temp = tempA * tempB;
		sprintf(result, "%d", temp);
		// printf("%s\n", result);
		return result;
	}

	size_t halfA = (lowA + highA) / 2;
	size_t halfB = (lowB + highB) / 2;
	
	char* a2b2 = multiplyDivideStandard(A, halfA + 1, highA, B, halfB + 1, highB);
	char* a1b2 = multiplyDivideStandard(A, lowA, halfA, B, halfB + 1, highB);
	char* a2b1 = multiplyDivideStandard(A, halfA + 1, highA, B, lowB, halfB);
	char* a1b1 = multiplyDivideStandard(A, lowA, halfA, B, lowB, halfB);

	// printf("%s\t%s\t%s\t%s\t", a2b2, a1b2, a2b1, a1b1);

	/* a1b1^n + (a1b2+a2b1)^(n/2) + a2b2 */

	char* t1 = add(a1b2, strlen(a1b2), 0, a2b1, strlen(a2b1), 0);
	char* t2 = add(a1b1, strlen(a1b1), lenA, t1, strlen(t1), lenA / 2);
	char* result = add(t2, strlen(t2), 0, a2b2, strlen(a2b2), 0);

	// printf("%s, %s, %s\n", t1, t2, result);

	free(t1);
	free(t2);
	free(a2b2);
	free(a1b2);
	free(a2b1);
	free(a1b1);

	return result;
}

char* multiplyDivideImproved(char* A, size_t lowA, size_t highA, char* B, size_t lowB, size_t highB)
{
	// printf("(%zd, %zd), (%zd, %zd)\n", lowA, highA, lowB, highB);
	size_t lenA = highA - lowA + 1;
	size_t lenB = highB - lowB + 1;

	if (lenA <= 2 && lenB <= 2)
	{
		char* result = malloc(9);
		int tempA = charToInt(A[highA]);
		if (lenA == 2) tempA += 10 * charToInt(A[lowA]);
		int tempB = charToInt(B[highB]);
		if (lenB == 2) tempB += 10 * charToInt(B[lowB]);
		int temp = tempA * tempB;
		sprintf(result, "%d", temp);
		// printf("%s\n", result);
		return result;
	}

	size_t halfA = (lowA + highA) / 2;
	size_t halfB = (lowB + highB) / 2;

	char* c0 = multiplyDivideImproved(A, halfA + 1, highA, B, halfB + 1, highB);
	char* c2 = multiplyDivideImproved(A, lowA, halfA, B, lowB, halfB);

	size_t lenc0 = strlen(c0);
	size_t lenc2 = strlen(c2);

	char* t1 = add(A + halfA + 1, lenA / 2, 0, A + lowA, lenA / 2, 0);
	char* t2 = add(B + halfB + 1, lenB / 2, 0, B + lowB, lenB / 2, 0);

	size_t t1ActIndex = 0;
	while (t1[t1ActIndex] == '0') ++t1ActIndex;
	size_t t2ActIndex = 0;
	while (t2[t2ActIndex] == '0') ++t2ActIndex;

	size_t lent1 = strlen(t1) - t1ActIndex;
	size_t lent2 = strlen(t2) - t2ActIndex;

	char* t3 = NULL;
	char* tt1 = t1;
	char* tt2 = t2;

	if (lent1 == 0 || lent2 == 0)
	{
		t3 = malloc(2);
		t3[0] = '0';
		t3[1] = '\0';
	}
	else
	{
		size_t initSize = 2;
		size_t maxLen = lent1;
		if (lent2 > maxLen)
		{
			maxLen = lent2;
		}
		while (initSize < maxLen)
		{
			initSize *= 2;
		}
		if (lent1 != initSize)
		{
			tt1 = malloc(initSize);
			memset(tt1, '0', initSize - lent1);
			memcpy(tt1 + initSize - lent1, t1 + t1ActIndex, lent1);
			t1ActIndex = 0;
			lent1 = initSize;
		}
		if (lent2 != initSize)
		{
			tt2 = malloc(initSize);
			memset(tt2, '0', initSize - lent2);
			memcpy(tt2 + initSize - lent2, t2 + t2ActIndex, lent2);
			t2ActIndex = 0;
			lent2 = initSize;
		}
		t3 = multiplyDivideImproved(tt1, t1ActIndex, t1ActIndex + lent1 - 1, tt2, t2ActIndex, t2ActIndex + lent2 - 1);
	}

	char* t4 = add(c2, lenc2, 0, c0, lenc0, 0);
	char* c1 = sub(t3, t4);
	char* t5 = add(c2, lenc2, lenA, c1, strlen(c1), lenA / 2);
	char* result = add(t5, strlen(t5), 0, c0, lenc0, 0);

	// printf("%s\n", result);

	if (tt1 != t1) free(tt1);
	if (tt2 != t2) free(tt2);

	free(t1);
	free(t2);
	free(t3);
	free(t4);
	free(t5);
	free(c1);
	free(c2);
	free(c0);

	return result;
}

char* multiplyDivideMain(char* A, char* B, char* (*func)(char*, size_t, size_t, char*, size_t, size_t))
{
	if (strcmp(A, "0") == 0 || strcmp(B, "0") == 0)
	{
		char* result = malloc(2);
		strcpy(result, "0");
		return result;
	}
	size_t initSize = 1;
	size_t lenA = strlen(A);
	size_t lenB = strlen(B);

	size_t maxLen = lenA;
	if (lenB > maxLen)
	{
		maxLen = lenB;
	}
	while (initSize < maxLen)
	{
		initSize *= 2;
	}
	char* tempA = malloc(initSize);
	char* tempB = malloc(initSize);
	memset(tempA, '0', initSize - lenA);
	memcpy(tempA + initSize - lenA, A, lenA);
	memset(tempB, '0', initSize - lenB);
	memcpy(tempB + initSize - lenB, B, lenB);
	char* result = func(tempA, 0, initSize - 1, tempB, 0, initSize - 1);
	free(tempA);
	free(tempB);
	
	size_t resLen = strlen(result);
	size_t resActIndex = 0;
	while (result[resActIndex] == '0') ++resActIndex;
	if (result[resActIndex] == '\0')
	{
		char* actResult = malloc(2);
		actResult[0] = '0';
		actResult[1] = '\0';
		free(result);
		return actResult;
	}
	else
	{
		char* actResult = malloc(resLen - resActIndex + 1);
		strcpy(actResult, result + resActIndex);
		free(result);
		return actResult;
	}
}

char* generateBigInteger(size_t count)
{
	char* result = malloc(count + 1);
	result[0] = '1' + rand() % 9;
	for (size_t i = 1; i < count; ++i)
	{
		result[i] = '0' + rand() % 10;
	}
	result[count] = '\0';
	return result;
}

/// Convert seconds to milliseconds
#define SEC_TO_MS(sec) ((sec)*1000)
/// Convert seconds to microseconds
#define SEC_TO_US(sec) ((sec)*1000000)
/// Convert seconds to nanoseconds
#define SEC_TO_NS(sec) ((sec)*1000000000)

/// Convert nanoseconds to seconds
#define NS_TO_SEC(ns)   ((ns)/1000000000)
/// Convert nanoseconds to milliseconds
#define NS_TO_MS(ns)    ((ns)/1000000)
/// Convert nanoseconds to microseconds
#define NS_TO_US(ns)    ((ns)/1000)

/// Get a time stamp in milliseconds.
uint64_t millis()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	uint64_t ms = SEC_TO_MS((uint64_t)ts.tv_sec) + NS_TO_MS((uint64_t)ts.tv_nsec);
	return ms;
}

/// Get a time stamp in microseconds.
uint64_t micros()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	uint64_t us = SEC_TO_US((uint64_t)ts.tv_sec) + NS_TO_US((uint64_t)ts.tv_nsec);
	return us;
}

/// Get a time stamp in nanoseconds.
uint64_t nanos()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	uint64_t ns = SEC_TO_NS((uint64_t)ts.tv_sec) + (uint64_t)ts.tv_nsec;
	return ns;
}


int main(void) 
{
	size_t digits = 5;
	char* A = generateBigInteger(pow(10, digits));
	char* B = generateBigInteger(pow(10, digits));
	uint64_t ts0 = millis();
	char* result1 = multiplyStandard(A, B);
	printf("%s\n", result1);
	uint64_t ts1 = millis();
	char* result2 = multiplyDivideMain(A, B, multiplyDivideStandard);
	printf("%s\n", result2);
	uint64_t ts2 = millis();
	char* result3 = multiplyDivideMain(A, B, multiplyDivideImproved);
	printf("%s\n", result3);
	uint64_t ts3 = millis();

	printf("%ld, %lld, %lld, %lld\n", digits, ts1 - ts0, ts2 - ts1, ts3 - ts2);

	free(result1);
	free(result2);
	free(result3);
	free(A);
	free(B);
	return 0;
}