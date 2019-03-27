#include "drawHist.h"

// This is just for rand and srand, you won't need this.
#include<stdlib.h>
#include<string.h>
#include<time.h>


int findMax(int *arr, int len)
{
	int m = -1;
	for(int i = 0; i < len; i++)
	{
		if(arr[i] > m)
		{
			m = arr[i];
		}
	}
	return m;
}

int getTerminalWidth()
{
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	return w.ws_col;
}

void padWithZeroes(char *str, int num, int numDigits)
{
	int i = 0;
	if(num == 0)
	{
		for(int i = 0; i < numDigits; i++)
		{
			str[i] = '0';
		}
		str[numDigits] = 0;
	}
	else
	{
		while(num*pow(10, i) < pow(10, numDigits-1))
		{
			str[i] = '0';
			i++;
		}
		sprintf(str + i, "%d", num);
	}
}

void drawHistogramFromArray(int *arr, int len)
{
	int max = findMax(arr, len);
	int numDigits = floor(log10(len) + 1);
	int width = getTerminalWidth() - 5;
	int dw = max / width;
	width = getTerminalWidth() - (numDigits + 2);
	dw = max / width;
	//printf("%d, %d, %d\n", max, width, dw);
	for(int i = 0; i < len; i++)
	{
		char string[numDigits+2];
		padWithZeroes(string, i, numDigits);
		printf("\n");
		printf("%s|", string);
		for(int j = 0; j < width - numDigits+2; j++)
		{
			if(arr[i] > dw * j)
			{
				printf("#");
			}
			else
			{
				break;
			}
		}
	}
	printf("\n");
}

Histogram *getHistogram()
{
	Histogram *hist = malloc(sizeof(Histogram));
	hist->numEntries = 0;
	hist->labelLen = 1;
	hist->entryLimit = 50;
	hist->values = malloc(sizeof(int) * hist->entryLimit);
	hist->labels = malloc(sizeof(char *)*hist->entryLimit);
	return hist;
}

void addEntry(Histogram *h, char *label, int value)
{
	if(h->numEntries < h->entryLimit)
	{
		h->values[h->numEntries] = value;
		if(strlen(label) > h->labelLen)
		{
			h->labelLen = strlen(label);
		}
		h->labels[h->numEntries] = malloc(strlen(label) + 1);
		strcpy(h->labels[h->numEntries], label);
		h->numEntries++;
	}
	else
	{
		int *tvals = malloc(sizeof(int)*(h->numEntries + (h->numEntries / 2)));
		char **tlabels = malloc(sizeof(char *)*(h->numEntries + (h->numEntries / 2)));
		memcpy(tvals, h->values, h->numEntries * sizeof(int));
		memcpy(tlabels, h->labels, h->numEntries * sizeof(char *));
		free(h->values);
		free(h->labels);
		h->labels = tlabels;
		h->values = tvals;
		h->entryLimit = h->entryLimit + (h->entryLimit / 2);
		addEntry(h, label, value);
	}
}

//len is the length to pad the string to.
char *padString(char *str, char padding, int len)
{
	int needed = len - strlen(str);
	char *ret = (char *) malloc(len + 1);
	int i = 0;
	while(needed > 0)
	{
		ret[i] = padding;
		i++;
		needed--;
	}
	memcpy(ret+i, str, strlen(str)+1);
	return ret;
}

void drawHistogramToStream(Histogram *h, FILE *stream, int swidth)
{
	int i = 0;
	int max = findMax(h->values, h->numEntries);
	int width = swidth - (h->labelLen + 2);
	int dw = max / width;
	for(; i < h->numEntries; i++)
	{
		fprintf(stream, "\n");
		char *str = padString(h->labels[i], ' ', h->labelLen);
		fprintf(stream, "%s|", str);
		free(str);
		for(int j = 0; j < width; j++)
		{
			if(h->values[i] > dw * j)
			{
				fprintf(stream, "#");
			}
			else
			{
				break;
			}
		}
	}
	fprintf(stream, "\n");
}


void drawHistogram(Histogram *h)
{
	drawHistogramToStream(h, stdout, getTerminalWidth());
}

void destroyHistogram(Histogram *h)
{
	for(int i = 0; i < h->numEntries; i++)
	{
		free(h->labels[i]);
	}
	free(h->labels);
	free(h->values);
	free(h);
}

int main()
{
	Histogram *h = getHistogram();
	srand(time(0));
	for(int i = 0; i < 200; i++)
	{
		char string[5];
		sprintf(string, "%d", i);
		addEntry(h, string, rand()%3000);
	}
	FILE *out = fopen("test.hist", "w");
	drawHistogramToStream(h, out, 200);
	fclose(out);
	destroyHistogram(h);
}
