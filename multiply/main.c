#include "demical.h"
#include "timecalc.h"
#include "multiply.h"
#include <math.h>

int main(void) 
{
	printf("位数      普通      分治普通  分治改进  结果校验\n");
	// 10 位到 10000 位执行 100 次取平均时间
	for (size_t digits = 1; digits <= 4; ++digits)
	{
		Demical* dec1 = demicalRand((size_t)pow(10, (double)digits));
		Demical* dec2 = demicalRand((size_t)pow(10, (double)digits));

		uint64_t total3 = 0, total4 = 0, total5 = 0;
		Demical* dec3 = NULL, * dec4 = NULL, * dec5 = NULL;
		bool equal = false;

		for (size_t i = 0; i < 100; ++i)
		{
			uint64_t ts0 = millis();
			dec3 = demicalMultiplyStandard(dec1, dec2);
			uint64_t ts1 = millis();
			dec4 = demicalMultiplyDividedStandard(dec1, dec2);
			uint64_t ts2 = millis();
			dec5 = demicalMultiplyDividedImproved(dec1, dec2);
			uint64_t ts3 = millis();

			total3 += ts1 - ts0;
			total4 += ts2 - ts1;
			total5 += ts3 - ts2;

			equal = demicalEquals(dec3, dec4) && demicalEquals(dec4, dec5);

			demicalFree(dec3);
			demicalFree(dec4);
			demicalFree(dec5);
		}

		printf("1e%-8llu%-10.2f%-10.2f%-10.2f", digits, total3 / 100.0, total4 / 100.0, total5 / 100.0);

		if (equal)
		{
			puts("通过");
		}
		else
		{
			puts("不通过");
		}

		demicalFree(dec1);
		demicalFree(dec2);
	}

	for (size_t digits = 5; digits <= 6; ++digits)
	{
		Demical* dec1 = demicalRand((size_t)pow(10, (double)digits));
		Demical* dec2 = demicalRand((size_t)pow(10, (double)digits));

		uint64_t ts0 = millis();
		Demical* dec3 = demicalMultiplyStandard(dec1, dec2);
		uint64_t ts1 = millis();
		Demical* dec4 = demicalMultiplyDividedStandard(dec1, dec2);
		uint64_t ts2 = millis();
		Demical* dec5 = demicalMultiplyDividedImproved(dec1, dec2);
		uint64_t ts3 = millis();

		printf("1e%-8llu%-10llu%-10llu%-10llu", digits, ts1 - ts0, ts2 - ts1, ts3 - ts2);
		
		if (demicalEquals(dec3, dec4) && demicalEquals(dec4, dec5))
		{
			puts("通过");
		}
		else
		{
			puts("不通过");
		}

		demicalFree(dec1);
		demicalFree(dec2);
		demicalFree(dec3);
		demicalFree(dec4);
		demicalFree(dec5);
	}

	// 1 千万位普通算法执行时间已超过 24 小时故不执行
	size_t digits = 7;
	Demical* dec1 = demicalRand((size_t)pow(10, (double)digits));
	Demical* dec2 = demicalRand((size_t)pow(10, (double)digits));
	uint64_t ts0 = millis();
	Demical* dec3 = demicalMultiplyDividedImproved(dec1, dec2);
	uint64_t ts1 = millis();

	char str[] = "＞24h";
	printf("1e%-8llu%-10s%-10s%-10llu", digits, str, str, ts1 - ts0);

	demicalFree(dec1);
	demicalFree(dec2);
	demicalFree(dec3);

	return 0;
}