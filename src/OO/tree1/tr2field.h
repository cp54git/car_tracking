#include<stdio.h>
#include<stdlib.h>
#include  <string.h>
#include  <math.h>

typedef struct FIELD
{
	char* field;
	int Width;
	int Height;
}FIELD;


struct TR
{
	int* tr;
	int *time;
	int N;
	int psize;
	char r,g,b;
};


struct CARTR
{
	int N;
	int day,month;
	int time;
	int x,y;
	char* fullname;
	//char b,r,g;
};



typedef struct COLOR 
{
	char red, green, blue;
} COLOR;


struct CLR
{
	char * r;
	char * g;
	char * b;
};
struct CLR Clr;


int BMP2DATA(char *fdata, char *fbmp);
struct FIELD READFIELD(char* fdata);
//int TR2FIELD(struct FIELD Field,struct TR Tr);
int TR2FIELD(struct FIELD Field,struct TR Tr, struct CLR Clr);
struct TR READTR(char *fname);
void CAR_TR(int *x, int *y, struct TR Tr, struct FIELD Field, struct FIELD Car );
void DATA2BMP(char *fbmp1, char *fbmp2, struct FIELD Field);//immamap.bmp,immamap2.bmp,struct FIELD
struct CARTR CARFIND(char* f, char* tr);
int* MY_MALLOC_INT(int n, const char*info);
char* MY_MALLOC_CHAR(int n, const char*info);
double* MY_MALLOC_DOUBLE(int n, const char*info);
void SET_COLOR (int x, int y, int k, FIELD * field, struct CLR Clr);
void LINE (int x0, int y0, int x1, int y1, int k, FIELD * field, struct CLR Clr);
void point (int x, int y, struct FIELD Field);
struct CLR READCLR(char *fname);
