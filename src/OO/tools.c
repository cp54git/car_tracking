#include "./tr2field.h"

int* MY_MALLOC_INT(int n, const char*info)
{
	int *mas;
	int i;
	mas=(int*)malloc(n*sizeof(int));
	if(mas==NULL) 
	{
		printf("%s:malloc error\n", info); 
		exit(8);
	}
	for(i=0;i<n;i++) mas[i]=0;
	return mas;
}

double* MY_MALLOC_DOUBLE(int n, const char*info)
{
	double *mas;
	int i;
	mas=(double*)malloc(n*sizeof(double));
	if(mas==NULL) 
	{
		printf("%s:malloc error\n", info); 
		exit(8);
	}
	for(i=0;i<n;i++) mas[i]=0;
	return mas;
}

char* MY_MALLOC_CHAR(int n, const char*info)
{
	char *mas;
	int i;
	mas=(char*)malloc(n*sizeof(char));
	if(mas==NULL) 
	{
		printf("%s:malloc error\n", info); 
		exit(8);
	}
	for(i=0;i<n;i++) mas[i]=0;
	return mas;
}

void SET_COLOR (int x, int y, int k, FIELD * field, struct CLR Clr) 
{
	char *r=Clr.r;
	char *g=Clr.g;
	char *b=Clr.b;
	if (x < 0 || x >= field->Width || y < 0 || y >= field->Height)
		return;
	else
	{
		field->field[y*field->Width*3+x*3+0]=b[k];
		field->field[y*field->Width*3+x*3+1]=r[k];
		field->field[y*field->Width*3+x*3+2]=g[k];		
	}
	return;
}	

void LINE (int x0, int y0, int x1, int y1, int k, FIELD * field, struct CLR Clr) 
{
    int steep = 0, tmp;
		if (abs(x0-x1)<abs(y0-y1)) 
		{
			tmp = x0;
			x0 = y0;
			y0 = tmp;
			tmp = x1;
			x1 = y1;
			y1 = tmp;
			steep = 1;
		}
		if (x0>x1) 
		{
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
		}
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = abs(dy)<<1;
    int error2 = 0;
    int y = y0;
		for (int x=x0; x<=x1; x++) 
		{
			if (steep) 
			{
				SET_COLOR(y, x, k, field, Clr);
			} 
			else 
			{
				SET_COLOR(x, y, k, field, Clr);
			}
			error2 += derror2;

			if (error2 > dx) 
			{
				y += (y1>y0?1:-1);
				error2 -= dx<<1;
			}
		}
	return;	
}

void point (int x, int y, struct FIELD Field){
	char *field=Field.field;
	int width=Field.Width;
	int k=10;
	
	field[(x+1)*width*3+y*3+0]=-k+field[(x+1)*width*3+y*3+0]; 
	field[(x+1)*width*3+y*3+1]=-k+field[(x+1)*width*3+y*3+1];
	field[(x+1)*width*3+y*3+2]=-k+field[(x+1)*width*3+y*3+2];
	
	field[(x-1)*width*3+y*3+0]=-k+field[(x-1)*width*3+y*3+0]; 
	field[(x-1)*width*3+y*3+1]=-k+field[(x-1)*width*3+y*3+1];
	field[(x-1)*width*3+y*3+2]=-k+field[(x-1)*width*3+y*3+2];
	
	field[x*width*3+(y+1)*3+0]=-k+field[x*width*3+(y+1)*3+0]; 
	field[x*width*3+(y+1)*3+1]=-k+field[x*width*3+(y+1)*3+1];
	field[x*width*3+(y+1)*3+2]=-k+field[x*width*3+(y+1)*3+2];
	
	field[x*width*3+(y-1)*3+0]=-k+field[x*width*3+(y-1)*3+0]; 
	field[x*width*3+(y-1)*3+1]=-k+field[x*width*3+(y-1)*3+1];
	field[x*width*3+(y-1)*3+2]=-k+field[x*width*3+(y-1)*3+2];

	field[(x+1)*width*3+(y+1)*3+0]=-k+field[(x+1)*width*3+(y+1)*3+0]; 
	field[(x+1)*width*3+(y+1)*3+1]=-k+field[(x+1)*width*3+(y+1)*3+1];
	field[(x+1)*width*3+(y+1)*3+2]=-k+field[(x+1)*width*3+(y+1)*3+2];
	
	field[(x+1)*width*3+(y-1)*3+0]=-k+field[(x+1)*width*3+(y-1)*3+0]; 
	field[(x+1)*width*3+(y-1)*3+1]=-k+field[(x+1)*width*3+(y-1)*3+1];
	field[(x+1)*width*3+(y-1)*3+2]=-k+field[(x+1)*width*3+(y-1)*3+2];
	
	field[(x-1)*width*3+(y+1)*3+0]=-k+field[(x-1)*width*3+(y+1)*3+0]; 
	field[(x-1)*width*3+(y+1)*3+1]=-k+field[(x-1)*width*3+(y+1)*3+1];
	field[(x-1)*width*3+(y+1)*3+2]=-k+field[(x-1)*width*3+(y+1)*3+2];
	
	field[(x-1)*width*3+(y-1)*3+0]=-k+field[(x-1)*width*3+(y-1)*3+0]; 
	field[(x-1)*width*3+(y-1)*3+1]=-k+field[(x-1)*width*3+(y-1)*3+1];
	field[(x-1)*width*3+(y-1)*3+2]=-k+field[(x-1)*width*3+(y-1)*3+2];
}

struct CLR READCLR(char *fname){
	struct CLR Clr;
	int r,g,b;
	char * mr;
	char * mg;
	char * mb;
	int k, i;
	FILE *in;
	k=1276;
	in=fopen(fname,"r");
	mr=MY_MALLOC_CHAR(k,__FUNCTION__);
	mg=MY_MALLOC_CHAR(k,__FUNCTION__);
	mb=MY_MALLOC_CHAR(k,__FUNCTION__);
	for (i=0;i<k;i++)
	{
		if (fscanf(in,"%d%d%d",&r,&g,&b)!=3) 
		{
			printf("%s:file %s error\n", __FUNCTION__, fname); 
			break;
		}
		mr[i]=(char) r;
		mg[i]=(char) g;
		mb[i]=(char) b;
	}
	Clr.r=mr;
	Clr.g=mg;
	Clr.b=mb;
	return Clr;
}