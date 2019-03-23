#ifndef DRAWHIST_H
#define DRAWHIST_H

#include<stdio.h>
#include<sys/ioctl.h>
#include<math.h>
#include<string.h>

struct _Hist
{
	int numEntries;
	char **labels;
	int *values;
	int labelLen;
	int entryLimit;
};

typedef struct _Hist Histogram;

int findMax(int *arr, int len);
int getTerminalWidth();
void padWithZeroes(char *str, int num, int numDigits);
char *padString(char *str, char padding, int len);
void drawHistogramFromArray(int *arr, int len);

Histogram *getHistogram();
void addEntry(Histogram *h, char *label, int value);
void drawHistogram(Histogram *h);
void destroyHistogram(Histogram *h);


#endif
