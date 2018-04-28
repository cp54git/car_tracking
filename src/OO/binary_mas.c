#include<stdio.h> 
#include<stdlib.h> 
struct TREE 
{ 
	struct TREE_NODE *mem; 
	int Nmax; 
	int Ncur; 
}; 

struct TREE_NODE 
{ 
	int val; 
	int left; 
	int right; 
}; 

void TREE_prn(struct TREE *root, int cur); 
void add(struct TREE* tree, int val, int cur); 
int search(struct TREE* tree, int val, int cur); 
void save(struct TREE* tree, char* fname);
void download(struct TREE* tree, char* fname);


int main() 
{ 
	int Nmax = 10; 
	int x, found, i; 
	struct TREE *root; 
	char fname1[]="elem.txt"; 
	FILE*elem; 
	char fname2[]="out.txt";
	//printf("1\n"); 
	elem=fopen(fname1,"r"); 
	//char buf[50]; 
	root = (struct TREE *)malloc(sizeof(struct TREE)); 
	root->Nmax = Nmax; 
	root->mem = (struct TREE_NODE*)malloc(Nmax*sizeof(struct TREE_NODE)); 
	root->Ncur=0; 
	while(fscanf(elem, "%d",&x)==1) 
	{  
		add(root,x,0); 
	} 
	found=search(root,7,0); 
	printf("%d\n",found); 
	save(root, fname2);
	download(root, fname2);
	printf("\n");
	TREE_prn(root, 0);
	return 0; 
} 

void TREE_prn(struct TREE *root, int cur)//tree print
{ 
	if (cur!=-1) printf("<%d>\n", root->mem[cur].val); 
	if (cur!=-1) TREE_prn(root,root->mem[cur].left); 
	if (cur!=-1) TREE_prn(root,root->mem[cur].right); 
	return; 
} 

void add(struct TREE* tree, int val, int cur) //val, left, right adding
{ 
	int new=tree->Ncur; 
	tree->mem[new].left=-1; 
	tree->mem[new].right=-1; 
	tree->mem[new].val=val; 
	tree->Ncur++; 
	if(tree->Ncur!=1) 
	{ 
		while(1) 
		{ 
			if (tree->mem[cur].val<val)//right 
			{ 
				if(tree->mem[cur].right==-1) {tree->mem[cur].right=new; return;} 
				else {cur=tree->mem[cur].right;} 
			} 
			else 
			{ 
				if(tree->mem[cur].left==-1) {tree->mem[cur].left=new; return;} 
				else {cur=tree->mem[cur].left;} 
			} 
		} 
	} 
} 

int search(struct TREE* tree, int val, int cur) 
{ 
	while(val!=tree->mem[cur].val) 
	{ 
		if (tree->mem[cur].val<val)//right 
		{ 
			if(tree->mem[cur].right==-1) {printf("NO ELEMENT\n");return 0;} 
			else cur=tree->mem[cur].right; 
		} 
		else 
		{ 
			if(tree->mem[cur].left==-1) {printf("NO ELEMENT\n");return 0;} 
			else cur=tree->mem[cur].left; 
		} 
	} 
	return cur+1; 
}

void save(struct TREE* tree, char* fname)
{
	FILE* out;
	out=fopen(fname, "wb");
	fwrite((void*)&(tree->Ncur), sizeof(int), 1, out);
	fwrite((void*)&(tree->mem[0]), sizeof(struct TREE_NODE), tree->Ncur, out);
	fclose(out);
}

void download(struct TREE* tree, char* fname)
{
	int i;
	FILE* out;
	out=fopen(fname, "rb");	
	fread((void*)&(tree->Ncur), sizeof(int), 1, out);
	tree->mem = (struct TREE_NODE*)malloc(tree->Ncur*sizeof(struct TREE_NODE));
	fread((void*)&(tree->mem[0]), sizeof(struct TREE_NODE), tree->Ncur, out);
	printf("%d\n", tree->Ncur);
	for(i=0;i<tree->Ncur;i++)
	{
		printf("%d_", tree->mem[i].val);
		printf("%d_", tree->mem[i].left);
		printf("%d\n", tree->mem[i].right);
	}
	fclose(out);
}