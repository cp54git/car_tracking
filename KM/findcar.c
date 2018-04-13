#include "./tr2field.h" 	

int main(){
	int x;
	int y;
	int i;
	struct TR Tr;
	struct FIELD Car;
	struct FIELD Field;
	struct CARTR CarTr;
	struct COLOR Color; 
	char fname1[]="Data.bmp";
	char fname2[]="imamap.bmp";
	char fname3[]="imamap2.bmp";
	char text1[]="Data.txt";
	char text2[]="imamap.txt";
	//char coor[]="coordinates.txt";
	char f[]="carfind.txt";
	char tr[]="traektoria.txt";
	char fcolor[]="clr.txt";
	BMP2DATA(text1, fname1);
	BMP2DATA(text2, fname2);
	CarTr=CARFIND(f, tr);
	//printf("1\n");
	Tr=READTR(CarTr.fullname);
	Clr=READCLR(fcolor); //"color.txt"
	Car=READFIELD(text1);
	Field=READFIELD(text2);
	//TR2FIELD(Field, Tr);
	TR2FIELD(Field,Tr,Clr);
	//x=Tr.tr[(Tr.N-1)*2+0];
	//y=Tr.tr[(Tr.N-1)*2+1];
	x=CarTr.x;
	y=CarTr.y;
	CAR_TR(&x, &y, Tr, Field, Car);
	//for (i=0;i<Tr.N-1;i++)
	//{
	//	LINE(Tr.tr[i*2+0],Tr.tr[i*2+1],Tr.tr[i*2+2],Tr.tr[i*2+3],&Color,&Field);
	//}
	//printf("1\n");
	DATA2BMP(fname2,fname3,Field);
	
	return 0;
}

int BMP2DATA(char *fdata, char *fbmp)//"imamap.txt","imamap.bmp"
{
	int i,j;
	FILE *in;
	FILE *out;
	int N,Height,Width;
	char *mas;
	in=fopen(fbmp,"r");
	out=fopen(fdata,"w");
	unsigned char bm[54];       //Массив для хранения заголовка
	unsigned int   biWidth;           
	unsigned int   biHeight;   
	fread((void *) bm,1,54,in);
	memcpy((void *)(&biWidth), (void*) (bm +18), 4);
	memcpy((void *)(&biHeight), (void*) (bm +22), 4);
	Width=biWidth;
	Height=biHeight;
	N=Height*Width*3;
	mas=MY_MALLOC_CHAR(N,__FUNCTION__);
	fread((void *) mas,1,N,in);
	//out=stdout;
	fprintf(out, "%d %d\n",Width,Height);
	for(i=Height-1;i>=0;i--)
	{
		for(j=0;j<Width;j++)
		{
			fprintf(out, "%4d %4d %4d  ",mas[i*3*Width+j*3+0],mas[i*3*Width+j*3+1],mas[i*3*Width+j*3+2]);
		}
		fprintf(out, "\n");
	}
	fclose(in);
	fclose(out);
	return 0;
}

struct FIELD READFIELD(char* fdata) //"imamap.txt", "car.txt"
{
	struct FIELD field;
	int N,Width,Height,i,j,c1,c2,c3;
	char *mas;
	FILE *in;
	in=fopen(fdata,"r");
	if(in==NULL)
	{
		printf("%s: file error\n", __FUNCTION__);
		exit(8);
	}
	if(fscanf(in,"%d %d", &Width, &Height)!=2)
	{
		printf("%s: file erro\n", __FUNCTION__);
		exit(8);
	}
	//printf("1\n");
	N=Width*Height*3;
	field.Width=Width;
	field.Height=Height;
	field.field=MY_MALLOC_CHAR(N,__FUNCTION__);
	mas=field.field;
	for(i=Height-1;i>=0;i--)
	{
		for(j=0;j<Width;j++)
		{
			fscanf(in, "%d%d%d",&c1,&c2,&c3);
			mas[i*3*Width+j*3+0]=c1;
			mas[i*3*Width+j*3+1]=c2;
			mas[i*3*Width+j*3+2]=c3;
		}
	}
	field.field=mas;
	fclose(in);
	return field;
}

struct TR READTR(char *fname)//"coordinates.txt"
{
	struct TR Tr;
	int *mas;
	int *time;
	int i,tmp;
	FILE *in;
	//printf("1\n");
	in=fopen(fname,"r");
	//printf("2\n");
	fscanf(in, "%d", &Tr.N);
	//printf("1\n");
	/*fscanf(in, "%d", &tmp);
	Tr.b=(unsigned char)tmp;	
	fscanf(in, "%d", &tmp);
	Tr.r=(unsigned char)tmp;
	fscanf(in, "%d", &tmp);
	Tr.g=(unsigned char)tmp;*/
	//fscanf(in, "%d", &Tr.psize);
	mas=MY_MALLOC_INT(2*Tr.N,__FUNCTION__);
	time=MY_MALLOC_INT(Tr.N,__FUNCTION__);
	//printf("1\n");
	for (i=0;i<Tr.N;i++)
	{
		if (fscanf(in,"%d%d%d",&time[i], &mas[i*2+0],&mas[i*2+1])!=3) 
		{
			printf("%s:file %s error\n", __FUNCTION__, fname); 
			break;
		}
	}
	Tr.N=i;
	Tr.tr=mas;
	Tr.time=time;
	fclose(in);
	return Tr;
}

int TR2FIELD(struct FIELD Field,struct TR Tr, struct CLR Clr)
{
	int N=Tr.N;
	int i,nw,nh,lw,lh;
	int *tr=Tr.tr;
	char *field=Field.field;
	int width=Field.Width;
	int height=Field.Height;
	char *r=Clr.r;
	char *g=Clr.g;
	char *b=Clr.b;
	int k=0;
	int time=0;
	for(i=0;i<N-1;i++)
	{
		nw=tr[i*2];
		nh=tr[i*2+1];
		lw=tr[(i+1)*2];
		lh=tr[(i+1)*2+1];
		
		//printf("%d %d\n", nw,nh);
		//printf("%d %d\n", width,height);
		if ((nw>=width)||(nh>=height)) continue;
		else
		{	
			if((nw==lw)&&(nh==lh))
			{ 
			time++;
			//printf("%d\n", time);
			}
			else
			{
				field[nh*width*3+nw*3+0]=b[k];
				field[nh*width*3+nw*3+1]=r[k];
				field[nh*width*3+nw*3+2]=g[k];
				LINE(nw,nh,lw,lh,k,&Field,Clr);
				k=k+((int)1276/N);
				time=0;				
			}
			if(time>=2)
			{
				//printf("mmm\n");
				point(nh, nw, Field);
			}
		}
	}
	return 0;
}
/*int TR2FIELD(struct FIELD Field,struct TR Tr)
{
	int N=Tr.N;
	int i,nw,nh;
	int *tr=Tr.tr;
	char *field=Field.field;
	int width=Field.Width;
	int height=Field.Height;
	char b=Tr.b;
	char g=Tr.g;
	char r=Tr.r;
	for(i=0;i<N;i++)
	{
		nw=tr[i*2];
		nh=tr[i*2+1];
		if ((nw>=width)||(nh>=height)) continue;
		else
		{
			field[nh*width*3+nw*3+0]=b*0;
			field[nh*width*3+nw*3+1]=r*0;
			field[nh*width*3+nw*3+2]=g*0;
		}
	}
	return 0;
}*/

void CAR_TR(int *x, int *y, struct TR Tr, struct FIELD Field, struct FIELD Car)
{
	int i,j;
	for(i=0;i<Car.Height;i++)
	{
		for(j=0;j<Car.Width;j++)
		{
			Field.field[((*y)+i)*Field.Width*3+((*x)+j)*3+0]=Car.field[i*Car.Width*3+j*3+0];
			Field.field[((*y)+i)*Field.Width*3+((*x)+j)*3+1]=Car.field[i*Car.Width*3+j*3+1];
			Field.field[((*y)+i)*Field.Width*3+((*x)+j)*3+2]=Car.field[i*Car.Width*3+j*3+2];
		}
	}
}
void DATA2BMP(char *fbmp1, char *fbmp2, struct FIELD Field)//immamap.bmp,immamap2.bmp,struct FIELD
{
 char *mas=Field.field;    //Массив для новой кратинки
 unsigned char bm[54];       //Массив для хранения заголовка
 FILE *in;
 FILE *out;
 int Height,Width;
 Width=Field.Width;
 Height=Field.Height;
 if(Width%4!=0)
 {
	printf("Width\4!=0\n"); 
	exit(1);
 }			
in=fopen(fbmp1,"rb"); 
out=fopen(fbmp2,"wb");
 if(out==NULL)
 {
	 printf("immamap2.bmp:  Err_fopen\n"); 
	 exit(3);
 }
printf("%d %d\n", Height, Width);
fread((void*) bm,1,54,in);
fwrite((void*) bm,1,54,out);
fwrite((void*) mas,1,Height*Width*3,out);
fclose(in);
fclose(out);
return;
}

struct CARTR CARFIND(char* f, char* tr)
{
	struct CARTR CarTr;
	FILE*find,*traek;
	int x,y,num;
	char fullname[1024];
	int t=0;
	find=fopen(f,"r");
	printf("input a car number:\n");
	fscanf(find,"%d",&CarTr.N);
	printf("input day and month:\n");
	fscanf(find,"%d",&CarTr.day);
	fscanf(find,"%d",&CarTr.month);
	printf("input time:\n");
	fscanf(find,"%d",&CarTr.time);	
	//переход в нужную директорию
	sprintf(fullname,"./cars/%d/%d/%d/%s",CarTr.N,CarTr.month,CarTr.day,tr);
	traek=fopen(fullname,"r");
	if(traek==NULL) printf("%s:file \n", __FUNCTION__);
	//printf("%s\n",fullname);
	CarTr.fullname=strdup(fullname);
	fscanf(traek,"%d",&num);
	fscanf(traek,"%d",&t);
	while((CarTr.time-t)>0)
	{
		fscanf(traek,"%d%d",&x,&y);	
		fscanf(traek,"%d",&t);
	}
	fscanf(traek,"%d%d",&x,&y);	
	CarTr.x=x;
	CarTr.y=y;
	return CarTr;
}