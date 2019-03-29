#include "drawHist.h"

#include<stdlib.h>
#include<string.h>
#include<time.h>

/// @file drawHist.c

/**
 * Internal helper function which finds the maximum value in an array of integers. 
 * @return The maximum value in the array.
 */
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

/**
 * Internal helper function to get the width of the terminal.
 * @return The width of the current terminal.
 */
int getTerminalWidth()
{
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	return w.ws_col;
}

/**
 * Helper function to create a new string from the given string padded with the given padding character 
 * to the specified length. 
 * @param str The string to be padded
 * @param padding The character to pad the string with.
 * @param len The length to pad the string to
 * @return a new string which is padded with the character.
 */
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

/**
 * Returns a new instance of a Histogram struct. Performs all the necessary first time setup.
 * @return A newly instantiated Histogram struct.
 */
Histogram *getHistogram()
{
	Histogram *hist = malloc(sizeof(Histogram));
	hist->numEntries = 0;
	hist->labelLen = 1;
	hist->entryLimit = 50;
	hist->values = malloc(sizeof(int) * hist->entryLimit);
	hist->labels = malloc(sizeof(char *)*hist->entryLimit);
	hist->paddingChar = ' ';
	return hist;
}

/**
 * Adds an entry into a Histogram. 
 * @param h The Histogram to add the given entry to.
 * @param label The label for the entry
 * @param value The value for the entry.
 */
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
	// This bit of code allocates space and a half for the values in the Histogram and 
	// moves the old entries to the new location and tries inserting the value again.
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

/**
 * Draws a Histogram struct to the specified stream formatted to the given width.
 * @param stream The stream to print the Histogram to. Can be a file.
 * @param swidth The width of the stream to be printed to. 
 */
void drawHistogramToStream(Histogram *h, FILE *stream, int swidth)
{
	int i = 0;
	int max = findMax(h->values, h->numEntries);
	int width = swidth - (h->labelLen + 2);
	int dw = max / width;
	for(; i < h->numEntries; i++)
	{
		fprintf(stream, "\n");
		char *str = padString(h->labels[i], h->paddingChar, h->labelLen);
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

/**
 *
 */
void setPaddingCharacter(Histogram *h, char padding)
{
	h->paddingChar = padding;
}

/**
 * Convenience function to simply print out a Histogram to the terminal.
 * @param h The Histogram to be displayed.
 */
void drawHistogram(Histogram *h)
{
	drawHistogramToStream(h, stdout, getTerminalWidth());
}

/** @deprecated
 * Takes in an array of integers and prints them as a Histogram with the 
 * index as the label.
 * @param arr The array of values to be displayed.
 * @param len The number of entries in the array.
 */
void drawHistogramFromArray(int *arr, int len)
{
	Histogram *h = getHistogram();
	for(int i = 0; i < len; i++)
	{
		int slen = (int) (log10(arr[i]) + 1);
		char *string = malloc(sizeof(char) * (slen + 1));
		sprintf(string, "%d", i);
		addEntry(h, string, arr[i]);
		free(string);
	}
	setPaddingCharacter(h, '0');
	drawHistogram(h);
	destroyHistogram(h);
}

/**
 * Takes in a Histogram struct and frees all of it's assets.
 * @param h The Histogram struct to be destroyed
 */
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
	int arr[200];
	srand(time(0));
	for(int i = 0; i < 200; i++)
	{
		arr[i] = rand()%3000;
	}
	drawHistogramFromArray(arr, 200);
}
