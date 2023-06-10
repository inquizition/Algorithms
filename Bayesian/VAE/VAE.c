#include "stdint.h"
#include "stdio.h"

#define SIZE 5000;
//void VAE()

int main()
{
	FILE *file;
	int img[SIZE];
	
	file = fopen("data/img.bin", "rb");
	fread(img, sizeof(int), SIZE, file);
	fclose(file);
	
	int i;
	for(i = 0; i < SIZE; i++)
	{
	   printf("d%\n", img[i]);
	}
	
	return 1;
}
