#include "demical.h"
#include "timecalc.h"
#include "multiply.h"
#include <math.h>

int main(void) 
{
	size_t digits = 5;
	Demical* dec1 = demicalRand((size_t)pow(10, (double)digits));
	Demical* dec2 = demicalRand((size_t)pow(10, (double)digits));
	// Demical* dec1 = demicalFromCString("0");
	// Demical* dec2 = demicalFromCString("11561651");
	uint64_t ts0 = millis();
	Demical* dec3 = demicalMultiplyStandard(dec1, dec2);
	uint64_t ts1 = millis();
	Demical* dec4 = demicalMultiplyDividedStandard(dec1, dec2);
	uint64_t ts2 = millis();
	Demical* dec5 = demicalMultiplyDividedImproved(dec1, dec2);
	uint64_t ts3 = millis();
	demicalPrint(dec3);
	putchar('\n');
	demicalPrint(dec4);
	putchar('\n');
	demicalPrint(dec5);
	putchar('\n');
	printf("%lld\t%lld\t%lld\n", ts1 - ts0, ts2 - ts1, ts3 - ts2);

	demicalFree(dec1);
	demicalFree(dec2);
	demicalFree(dec3);
	demicalFree(dec4);
	demicalFree(dec5);

	return 0;
}