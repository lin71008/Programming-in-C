#include <stdio.h>
#include "hw0103.h"

void swap(const char** pA, const char** pB);

int main()
{
	swap(&pStr01, &pStr02);
	print_answers();
	return 0;
}

void swap(const char** pA, const char** pB)
{
	const char *pTemp = *pA;
	*pA = *pB;
	*pB = pTemp;
}
