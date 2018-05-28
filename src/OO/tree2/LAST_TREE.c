#include<stdio.h>
#include<stdlib.h>

struct TREE 
{ 
	struct TREE_NODE *mem; 
	int Nmax; 
	int Ncur; 
}; 

struct TREE_B
{
	struct TREE *mem; 
	int Nmax; 
	int Ncur; 
};

struct TREE_NODE 
{ 
	int val; 
	char* str;
	int x,y;
	struct TREE *next;  
}; 

int my_strlen(char *a);
char * my_strdup(char *a);
int my_strcmp(char *a, char *b);
void save(struct TREE_B* tree, char* fname);
void download(struct TREE_B* tree, char* fname);

int main() 
{ 
	int i,j,z;
	int year,month,day,x1,y1;
	char s[10];
	struct TREE_B *tree;
	struct TREE years;
	FILE*base;
	FILE*coor;
	char fname1[]="inn.txt";
	char fname2[]="inn2.txt";
	char fname3[]="outt.txt";
	base=fopen(fname1,"r");
	tree = (struct TREE_B *)malloc(sizeof(struct TREE_B));
	tree->mem = (struct TREE *)malloc(2000*sizeof(struct TREE));
	tree->Ncur=1;
	years.Ncur=0;
	fscanf(base,"%d", &years.Nmax);
	years.mem=(struct TREE_NODE *)malloc(years.Nmax*sizeof(struct TREE_NODE));
	for(i=0;i<years.Nmax;i++)//years
	{
		fscanf(base,"%d",&(years.mem[i].val));
		years.mem[i].next=(struct TREE *)malloc(1*sizeof(struct TREE));
		years.Ncur++;
	}
	tree->mem[0]=years;
	printf("%d\n", tree->Ncur);
	for(i=0;i<years.Ncur;i++)//months
	{
		tree->mem[i+1].mem=(struct TREE_NODE *)malloc(13*sizeof(struct TREE_NODE));
		for(j=1;j<=12;j++)
		{
			tree->mem[i+1].mem[j].val=j;
		}
		years.mem[i].next=&(tree->mem[tree->Ncur]);
		tree->Ncur++;
	}
	int k=0;
	printf("%d\n", tree->Ncur);
	for(i=0;i<years.Ncur*12;i++)//days
	{
		tree->mem[years.Ncur+i+1].mem=(struct TREE_NODE *)malloc(32*sizeof(struct TREE_NODE));
		for(j=1;j<=31;j++)
		{
			tree->mem[years.Ncur+i+1].mem[j].val=j;
		}
		if (i%12==0) k++;
		tree->mem[k].mem[i%12+1].next=&(tree->mem[years.Ncur+i+1]);
		tree->Ncur++;
	}
	coor=fopen(fname2,"r");
	k=years.Ncur;
	printf("%d\n", tree->Ncur);
	for(i=0;i<years.Ncur*12*31;i++)//times
	{
		tree->mem[years.Ncur+years.Ncur*12+i+1].mem=(struct TREE_NODE *)malloc(10*sizeof(struct TREE_NODE));
		tree->mem[years.Ncur+years.Ncur*12+i+1].Ncur=0;
		if (i%31==0) k++;
		tree->mem[k].mem[i%31+1].next=&(tree->mem[years.Ncur+years.Ncur*12+i+1]);
		tree->Ncur++;
		//printf("%d %d %d\n", k, i, tree->mem[0].mem[3].val);
	}
//	tree->mem[years.Ncur+years.Ncur*12+1].val=123;
	printf("%d\n", tree->Ncur);
	while(fscanf(coor,"%d%d%d%s%d%d",&year,&month,&day,s,&x1,&y1)==6)
	{
		for(i=0;i<years.Ncur;i++)
		{
			if(tree->mem[0].mem[i].val==year)
			{
				for(j=1;j<=12;j++)
				{
					if(tree->mem[0].mem[i].next->mem[j].val==month)
					{
						for(k=1;k<=31;k++)
						{
							if(tree->mem[0].mem[i].next->mem[j].next->mem[k].val==day)
							{
								tree->mem[0].mem[i].next->mem[j].next->mem[k].next->mem[tree->mem[0].mem[i].next->mem[j].next->mem[k].next->Ncur].str=my_strdup(s);
								tree->mem[0].mem[i].next->mem[j].next->mem[k].next->mem[tree->mem[0].mem[i].next->mem[j].next->mem[k].next->Ncur].x=x1;
								tree->mem[0].mem[i].next->mem[j].next->mem[k].next->mem[tree->mem[0].mem[i].next->mem[j].next->mem[k].next->Ncur].y=y1;
								tree->mem[0].mem[i].next->mem[j].next->mem[k].next->Ncur++;
							}
						}
					}
				}
			}
		}
	}
	
	//save(tree,fname3);
	//printf("1\n");
	download(tree,fname3);
	//printf("1\n");
	while(scanf("%d%d%d%s",&year,&month,&day,s)==4)
	{
		for(i=0;i<years.Ncur;i++)
		{
			if(tree->mem[0].mem[i].val==year)
			{
				for(j=1;j<=12;j++)
				{
					if(tree->mem[0].mem[i].next->mem[j].val==month)
					{
						for(k=1;k<=31;k++)
						{
							if(tree->mem[0].mem[i].next->mem[j].next->mem[k].val==day)
							{
								for(z=0;z<tree->mem[0].mem[i].next->mem[j].next->mem[k].next->Ncur;z++)
								{
									if(my_strcmp(tree->mem[0].mem[i].next->mem[j].next->mem[k].next->mem[z].str,s)==0)
									{
										printf("%d ",tree->mem[0].mem[i].next->mem[j].next->mem[k].next->mem[z].x);
										printf("%d \n",tree->mem[0].mem[i].next->mem[j].next->mem[k].next->mem[z].y);
									}
								}
							}	
						}
					}
				}
			}
		}
	}
	//printf("%d ",tree->mem[0].mem[0].next->mem[1].next->mem[3].next->mem[0].x);
	//printf("%d \n",tree->mem[0].mem[0].next->mem[1].next->mem[3].next->mem[0].y);
	//tree->mem[5].mem[2].val=123;
	//printf("%d\n", tree->mem[0].mem[3].val);
	//printf("%d\n", tree->mem[0].mem[0].next->mem[1].next->mem[2].val);
	printf("%d\n", tree->Ncur);
	return 0; 
}

int my_strlen(char *a)
{
	int len=0;
	while (a[len]!='\0') len++;
	return len;
}

char * my_strdup(char *a)
{
	char *b;
	int i;
	int n=my_strlen(a);
	b=malloc((n+1)*sizeof(char));
	for(i=0;i<n;i++) b[i]=a[i];
	return b;
}

int my_strcmp(char *a, char *b)
{
	int i=0;
	while((a[i]!='\0')&&(a[i]==b[i])) i++;
	return a[i]-b[i];
}

void save(struct TREE_B* tree, char* fname)
{
	FILE* out;
	out=fopen(fname, "wb");
	fwrite((void*)&(tree->Ncur), sizeof(int), 1, out);
	fwrite((void*)&(tree->mem[0]), sizeof(struct TREE), tree->Ncur, out);
	fclose(out);
}

void download(struct TREE_B* tree, char* fname)
{
	int i;
	FILE* out;
	out=fopen(fname, "rb");	
	fread((void*)&(tree->Ncur), sizeof(int), 1, out);
	tree->mem = (struct TREE*)malloc(tree->Ncur*sizeof(struct TREE));
	fread((void*)&(tree->mem[0]), sizeof(struct TREE_NODE), tree->Ncur, out);
	//printf("1\n");
	fclose(out);
}