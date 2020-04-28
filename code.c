#include <stdio.h>
#include <stdlib.h>
#include "code.h"

static void chartohex(char in, char* outh, char* outl)
{
	*outl = in & 0x0f;
	if(*outl > 9)
	{
		*outl += 55;
	}
	else{
		*outl += 48;
	}
	*outh = in >> 4;
	if(*outh > 9)
	{
		*outh += 55;
	}
	else{
		*outh += 48;
	}
}

static void updateaddress(char* a, char* b, char* c)
{
	if(*c == 99)
	{
		*c = 48;
		*b += 1;
	}
	else{ 
		*c += 4;
	}
	if(*c > 57)
	{
		*c = 99;
	}
	if(*b == 58)
	{
		*b = 97;
	}
	if(*b == 103)
	{
		*a += 1;
		*b = 48;
	}
}

int createout(char* infile, char* codefile)
{
	FILE* fptr = fopen(infile, "r");
	if(!fptr)
	{
		return 0;
	}
	
	fseek(fptr, 0, SEEK_END);
	int size = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);
	
	char end = size % 2;
	if(end)
	{
		if((((size / 2) * 12) + 18) > 512)
		{	
			fclose(fptr);
			return 0;
		}
	}
	else if((((size / 2) * 12) + 14) > 512)
	{	
		fclose(fptr);
		return 0;
	}
	
	FILE* fptrc = fopen(codefile, "w");
	if(!fptrc)
	{
		return 0;
	}
	
	size -= end;
	char a = 48;
	char b = 48;
	char c = 48;
	char r1p = -1;
	char r0 = 0;
	char r1 = 0;
	char r0h;
	char r0l;
	char r1h;
	char r1l;
	
	for(int i = 0; i < size; i += 2){
		if(r1p == '\n')
		{
			r0 = 13;
		}
		else{
			fread(&r0, sizeof(char), 1, fptr);
		}
		if(r0 == '\n')
		{
			r1 = 13;
		}
		else{
			fread(&r1, sizeof(char), 1, fptr);
		}
		if(r0 == r1p)
		{
			chartohex(r1, &r1h, &r1l);
			fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'h8011, 16'h61%c%c);\n", a, b, c, r1h, r1l);
		}
		else if(r0 == r1)
		{
			chartohex(r1, &r0h, &r0l);
			fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'h60%c%c, 16'h8110);\n", a, b, c, r0h, r0l);
		}
		else{
			chartohex(r0, &r0h, &r0l);
			chartohex(r1, &r1h, &r1l);
			fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'h60%c%c, 16'h61%c%c);\n", a, b, c, r0h, r0l, r1h, r1l);
		}
		updateaddress(&a, &b, &c);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'hc000, 16'hff00);\n", a, b, c);
		updateaddress(&a, &b, &c);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'hc100, 16'hff00);\n", a, b, c);
		updateaddress(&a, &b, &c);
		if(a >= 50)
		{
			return 0;
		}
		r1p = r1;
	}
	if(end)
	{
		fread(&r0, sizeof(char), 1, fptr);
		chartohex(r1, &r0h, &r0l);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'h60%c%c, 16'h610d);\n", a, b, c, r0h, r0l);
		updateaddress(&a, &b, &c);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'hc000, 16'hff00);\n", a, b, c);
		updateaddress(&a, &b, &c);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'hc100, 16'hff00);\n", a, b, c);
		updateaddress(&a, &b, &c);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'h600a, 16'hc000);\n", a, b, c);
		updateaddress(&a, &b, &c);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'hff00, 16'hdf00);\n", a, b, c);
	}
	else{
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'h601d, 16'h610a);\n", a, b, c);
		updateaddress(&a, &b, &c);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'hc000, 16'hff00);\n", a, b, c);
		updateaddress(&a, &b, &c);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'hc100, 16'hff00);\n", a, b, c);
		updateaddress(&a, &b, &c);
		fprintf(fptrc, "storage[16'h0%c%c%c>>2] = two16(16'hdf00, 16'h0000);\n", a, b, c);
	}
	fclose(fptr);
	fclose(fptrc);
	return 1;
}