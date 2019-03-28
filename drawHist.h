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
char *padString(char *str, char padding, int len);
Histogram *getHistogram();
void addEntry(Histogram *h, char *label, int value);
void drawHistogram(Histogram *h);
void drawHistogramToStream(Histogram *h, FILE *stream, int swidth);
void destroyHistogram(Histogram *h);


#endif
