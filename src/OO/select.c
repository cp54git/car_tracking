#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void SELECT(char* str);
void MAKE_ASK(char* s);

int main()
{
	char str[100];
	char file1[]="ask.txt";
	FILE*in;
	in=fopen(file1,"r");
	while(fscanf(in,"%s",str)==1)
	{
		if(strstr(str,"SELECT")!=NULL)
			SELECT(str);
	}
	fclose(in);
	return 0;
}

void SELECT(char* str)
{
	int IMEI;
	int date;
	int time;
	int i=0,j;
	char adress[20];
	char* begin;
	adress[0]='/';
	if(strstr(str,"IMEI")!=NULL)
	{
		begin=strstr(str,"IMEI");
		while(&(str[i])!=begin)
		{
			i++;
		}
		j=i+5;
		while(str[j]!='\0')
		{
			adress[j-i-4]=str[j];
			j++;
		}
		MAKE_ASK(adress);
		//printf("%s\n",adress);
	}
	else if(strstr(str,"date")!=NULL)
	{
		begin=strstr(str,"date");
		while(&(str[i])!=begin)
		{
			i++;
		}
		j=i+5;
		while(str[j]!='\0')
		{
			adress[j-i-4]=str[j];
			j++;
		}
		adress[j-i-4]='\0';
		MAKE_ASK(adress);
	}
	else if(strstr(str,"time")!=NULL)
	{
		begin=strstr(str,"time");
		while(&(str[i])!=begin)
		{
			i++;
		}
		j=i+5;
		while(str[j]!='\0')
		{
			adress[j-i-4]=str[j];
			j++;
		}
		adress[j-i-4]='.';
		adress[j-i-4+1]='t';
		adress[j-i-4+2]='x';
		adress[j-i-4+3]='t';
		adress[j-i-4+4]='\0';
		printf("%s\n",adress);
		MAKE_ASK(adress);
	}
}

void MAKE_ASK(char* s)
{
	char file2[]="indexTime.txt";
	char file3[]="answer.txt";
	char path[1024];
	FILE*out;
	FILE*index;
	out=fopen(file3,"w");
	index=fopen(file2,"r");
	while(fscanf(index,"%s",path)==1)
	{
		if(strstr(path,s)!=NULL)  printf("%s\n",path);
		//printf("1\n");
	}
	fclose(out);
	fclose(index);
}