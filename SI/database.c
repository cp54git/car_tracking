#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int searchName(FILE *in, char *prompt);//find name prompt in file in
char **mapPrompt(char *in);//split in in arrays of char* by '_'
void clearBuff(char *in);
void indexData();//create index-files 
void switchDates(int k);//change time k seconds forward
void mapFileSystem();//create directory tree
void formStatFile();//create stat-file

void indexData(){
	char *prompt0, *prompt1, *cmd;
	FILE *tmp1;
	FILE *indexIMEI;
	FILE *indexDate;
	FILE *indexTime;
	prompt0 = malloc(60*sizeof(char));
	prompt1 = malloc(60*sizeof(char));
	cmd = malloc(60*sizeof(char));
	system("ls ./data/ > tmp");
	tmp1 = fopen("tmp", "r");
	indexIMEI = fopen("indexIMEI.txt", "w");
	indexDate = fopen("indexDate.txt", "w");
	indexTime = fopen("indexTime.txt", "w");
	while(fscanf(tmp1, "%s", prompt0) == 1){
		fprintf(indexIMEI, "./data/%s\n", prompt0);
	}
	clearBuff(prompt0);
	clearBuff(prompt1);
	clearBuff(cmd);
	fclose(indexIMEI);
	indexIMEI = fopen("indexIMEI.txt", "r");
	while(fscanf(indexIMEI, "%s", prompt0) == 1){
		sprintf(cmd, "ls %s > tmp", prompt0);
		system(cmd);
		clearBuff(cmd);
		tmp1 = fopen("tmp", "r");
		while(fscanf(tmp1, "%s", prompt1) == 1){
			fprintf(indexDate, "%s/%s\n", prompt0, prompt1);
		}
		fclose(tmp1);
		system("rm ./tmp");
	}
	clearBuff(prompt0);
	clearBuff(prompt1);
	fclose(indexDate);
	indexDate = fopen("indexDate.txt", "r");
	fclose(indexIMEI);
	while(fscanf(indexDate, "%s", prompt0) == 1){
		sprintf(cmd, "ls %s > tmp", prompt0);
		system(cmd);
		clearBuff(cmd);
		tmp1 = fopen("tmp", "r");
		while(fscanf(tmp1, "%s", prompt1) == 1){
			fprintf(indexTime, "%s/%s\n", prompt0, prompt1);
		}
		fclose(tmp1);
		system("rm ./tmp");
	}
	fclose(indexTime);
	free(prompt0);
	free(prompt1);
	free(cmd);	
}

char **mapPrompt(char *in){
	char **res;
	char *tmp = (char*)malloc(20*sizeof(char));
	int i = 0, j = 0;
	res = (char**)malloc(5*sizeof(char*));
	res[0] = (char*)malloc(16*sizeof(char));
	res[1] = (char*)malloc(16*sizeof(char));
	res[2] = (char*)malloc(16*sizeof(char));
	res[3] = (char*)malloc(16*sizeof(char));
	res[4] = (char*)malloc(16*sizeof(char));
	while(in[i] != '\0'){
		if(in[i] == '_'){
			clearBuff(tmp);
			strncpy(tmp, in, i);
			tmp[i] = '\0';
			strcpy(res[j], tmp);
			j++;
			in = &(in[i+1]);
			i = 0;
		}
		i++;
	}
	clearBuff(tmp);
	strncpy(tmp, in, i);
	tmp[i] = '\0';
	strcpy(res[j], tmp);
	return res;
}

int searchName(FILE *in, char *prompt){
	char *tmp;
	tmp = malloc(64*sizeof(char));
	while(fscanf(in, "%s", tmp) == 1){
		if(strcmp(tmp, prompt) == 0) return 1;
	}
	free(tmp);
	return 0;
}

void switchDates(int k){
	struct tm *timer = malloc(sizeof(struct tm));
    int i;
	char* date;
	char* times;
	char* tmp = malloc(sizeof(char)*5);
	char* filename = malloc(sizeof(char)*32);
	char* nfilename = malloc(sizeof(char)*32);
	char* string = malloc(sizeof(char)*128);
	char* dayFinStr = malloc(sizeof(char)*2);
	char* monFinStr = malloc(sizeof(char)*2);
	char* yearFinStr = malloc(sizeof(char)*2);
	char* hourFinStr = malloc(sizeof(char)*2);
	char* minFinStr = malloc(sizeof(char)*2);
	char* secFinStr = malloc(sizeof(char)*2);
	char** prompt;
	FILE* tmpf, *names;
	system("ls ./data > tmp");
	names = fopen("tmp", "r");
	printf("creating index files...");
	while(fscanf(names, "%s", filename) == 1){
		if(strstr(filename, ".txt") == NULL || strcmp(filename, "indexIMEI.txt") == 0 || strcmp(filename, "indexDate.txt") == 0 || strcmp(filename, "indexTime.txt") == 0) continue;
		date = calloc(0, sizeof(char)*16);
		times = calloc(0, sizeof(char)*16);
		clearBuff(nfilename);
		sprintf(nfilename, "./data/%s", filename);
		tmpf = fopen(nfilename, "r");
		fscanf(tmpf, "%s", string);
		prompt = mapPrompt(string);
		//prompt[0] - IMEI, prompt[1] - date, prompt[2] - time, prompt[3] - latitude, prompt[4] - longitude
		strcat(date, prompt[1]);
		strcat(times, prompt[2]);
		strncpy(tmp, date, 2);
		date = &date[3];
		timer -> tm_mday = atoi(tmp);
		strncpy(tmp, date, 2);
		date = &date[3];
		timer -> tm_mon = atoi(tmp) - 1;
		strncpy(tmp, date, 4);
		timer -> tm_year = atoi(tmp) - 1900;
		for(i = 0; i < 5; i++){
			tmp[i] = 0;
		}
		strncpy(tmp, times, 2);
		times = &times[3];
		timer -> tm_hour = atoi(tmp);
		strncpy(tmp, times, 2);
		times = &times[3];
		timer -> tm_min = atoi(tmp);
		strncpy(tmp, times, 2);
		timer -> tm_sec = atoi(tmp);
		time_t tim_fin = mktime(timer);
		tim_fin += k;
		struct tm *wri = malloc(sizeof(struct tm));
		wri = localtime(&tim_fin);
		if(wri -> tm_mday < 10){
			sprintf(dayFinStr, "0%d", wri -> tm_mday);
		}
		else{
			sprintf(dayFinStr, "%d", wri -> tm_mday);
		}
		if(wri -> tm_mon < 10){
			sprintf(monFinStr, "0%d", wri -> tm_mon + 1);
		}
		else{
			sprintf(monFinStr, "%d", wri -> tm_mon + 1);
		}
		sprintf(yearFinStr, "%d", wri -> tm_year + 1900);
		if(wri -> tm_hour < 10){
			sprintf(hourFinStr, "0%d", wri -> tm_hour);
		}
		else{
			sprintf(hourFinStr, "%d", wri -> tm_hour);
		}
		if(wri -> tm_min < 10){
			sprintf(minFinStr, "0%d", wri -> tm_min);
		}
		else{
			sprintf(minFinStr, "%d", wri -> tm_min);
		}
		if(wri -> tm_sec < 10){
			sprintf(secFinStr, "0%d", wri -> tm_sec);
		}
		else{
			sprintf(secFinStr, "%d", wri -> tm_sec);
		}
		fclose(tmpf);
		tmpf = fopen(nfilename, "w");
		fprintf(tmpf, "%s_%s.%s.%s_%s:%s:%s_%s_%s\n", prompt[0], dayFinStr, monFinStr, yearFinStr, hourFinStr, minFinStr, secFinStr, prompt[3], prompt[4]);
		fclose(tmpf);
		free(prompt[0]);
		free(prompt[1]);
		free(prompt[2]);
		free(prompt[3]);
		free(prompt[4]);
		free(prompt);
	}
	free(nfilename);
	free(timer);
	free(tmp);
	free(filename);
	free(string);
	free(dayFinStr);
	free(monFinStr);
	free(yearFinStr);
	free(hourFinStr);
	free(minFinStr);
	free(secFinStr);
	fclose(names);
	system("rm tmp");
}

void clearBuff(char *in){
	int i;
	for(i = 0; i < strlen(in); i++){
		in[i] = 0;
	}
}

void mapFileSystem(){
	FILE* names;//names - file with names in directory
	FILE* data;//data - current file with a prompt
	FILE* tmp0; //tmp file for veryfing existance
	FILE* tmp1;
	char* filename, *prompt, **tmp, *cmd, *nfilename;//filename - string for opening the current file, prompt - data from <filename>
	system("ls -t ./data > tmp");
	names = fopen("tmp", "r");
	prompt = malloc(128*sizeof(char));
	cmd = malloc(64*sizeof(char));
	//printf("created tmp:\n");
	printf("########################################\n");
	while(fscanf(names, "%s", prompt) == 1){
		printf("%s\n", prompt);
	}
	printf("########################################\n");
	fclose(names);
	names = fopen("tmp", "r");
	filename = malloc(32*sizeof(char));
	nfilename = malloc(32*sizeof(char));
	while(fscanf(names, "%s", filename) == 1){
		if(strstr(filename, ".txt") == NULL) continue;
		if(strcmp(filename, "indexIMEI.txt") == 0){
			system("rm ./indexIMEI.txt");
			continue;
		}
		if(strcmp(filename, "indexDate.txt") == 0){
			system("rm ./indexDate.txt");
			continue;
		}
		if(strcmp(filename, "indexTime.txt") == 0){
			system("rm ./indexTime.txt");
			continue;
		}
		printf("parsing: %s\n", filename);
		clearBuff(nfilename);
		sprintf(nfilename, "./data/%s", filename);
		//printf("%s\n", nfilename);
		data = fopen(nfilename, "r");
		fscanf(data, "%s", prompt);
		tmp = mapPrompt(prompt);
		//tmp now has all the info([0] - IMEI, [1] - date, [2] - time, [3] - latitude, [4] - longtitude)
		//now searching for IMEI directory 
		system("ls ./data > tmp0");
		tmp0 = fopen("tmp0", "r");
		//printf("created tmp0\n");
		if(!searchName(tmp0, tmp[0])){
			clearBuff(cmd);
			printf("could not find IMEI directory, creating ");
			sprintf(cmd, "mkdir ./data/%s", tmp[0]);
			printf("./data/%s\n", tmp[0]);
			system(cmd);
		}
		else{
			printf("IMEI directory found!\n");
		}
		clearBuff(cmd);
		sprintf(cmd, "ls ./data/%s > ./data/tmp1", tmp[0]);
		system(cmd);
		clearBuff(cmd);
		tmp1 = fopen("./data/tmp1", "r");
		//printf("tmp1 formed\n");
		if(!searchName(tmp1, tmp[1])){
			clearBuff(cmd);
			printf("could not find date directory, creating ");
			sprintf(cmd, "mkdir ./data/%s/%s", tmp[0], tmp[1]);
			printf("./data/%s/%s\n", tmp[0], tmp[1]);
			system(cmd);
		}
		else{
			printf("date directory found\n");
		}
		clearBuff(cmd);
		//printf("moving file\n");
		clearBuff(cmd);
		sprintf(cmd, "mv ./data/%s ./data/%s/%s/%s.txt", filename, tmp[0], tmp[1], tmp[2]);
		printf("moving %s from ./data/%s to ./data/%s/%s/%s.txt\n", filename, filename, tmp[0], tmp[1], tmp[2]);
		system(cmd);
		printf("########################################\n");
		free(tmp[0]);
		free(tmp[1]);
		free(tmp[2]);
		free(tmp[3]);
		free(tmp[4]);
		free(tmp);
		system("rm tmp0");
		system("rm ./data/tmp1");
	}
	fclose(names);
	free(filename);
	free(nfilename);
	free(prompt);
	system("rm tmp");
}

void formStatFile(){
	FILE *timeIndex;
	char *prompt = malloc(sizeof(char)*64);
	char *res = malloc(sizeof(char)*64);
	timeIndex = fopen("indexTime.txt", "r");
	while(fscanf(timeIndex, "%s", prompt) == 1){
		sprintf(res, "cat %s >> res.txt", prompt);
		system(res);
	}
	fclose(timeIndex);
	free(prompt);
	free(res);
}

int main(){
	switchDates(7*60*60);
	mapFileSystem();
	indexData();
	//formStatFile();
    indexData();
}
