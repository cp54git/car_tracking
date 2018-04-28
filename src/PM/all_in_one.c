#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void s_bubblesort (char ** names, int count_files) {
	int len_a, len_b, l, i, j, k;
	char * tmp_str = (char *) calloc (64, sizeof(char));
		for (i=0; i<count_files; i++) {
			k = count_files - i - 1;
			for (j=0; j<k; j++)
				if (strcmp (names[j], names[j+1]) > 0) {
					len_a = strlen (names[j]);
					len_b = strlen (names[j+1]);
					for (l=0; l<len_a; l++)
						tmp_str[l] = names[j][l];
					for (l=0; l<len_b; l++)
						names[j][l] = names[j+1][l];
					names[j][l] = 0;
					for (l=0; l<len_a; l++)
						names[j+1][l] = tmp_str[l];
					names[j+1][l] = 0;
				}
		}
	return;
}	

int main () {
	int count_files=0, i, j;
	char c;
	char dat[] = "ls -t ./data > tmp";
		system (dat);
	FILE * tmp = fopen ("tmp", "r");
		if (tmp == NULL) {
			printf("отсутствует файл tmp");
			return -1;
		}
	
		while (1) {
			c = fgetc (tmp);
			if (c == '\n')
				count_files++;
			if (feof(tmp)) {
				fseek (tmp, 0, SEEK_SET);
				break;
			}
		}
		
		//printf ("count: %d\n", count_files);
		
	char ** names = (char **) malloc (count_files * sizeof(char *));
		for (i=0; i<count_files; i++)
			names[i] = (char *) calloc (64, sizeof(char));
		for (i=0; i<count_files; i++)
			fscanf (tmp, "%s", names[i]);
		fclose (tmp);
		s_bubblesort (names, count_files);
		/*for (i=0; i<count_files; i++)
			printf ("%s\n", names[i]);*/
		
	FILE * wad = fopen ("wad.txt", "r");
	char directory[64] = "./data/";
	char * full_directory = (char *) calloc (128, sizeof(char));
	
		if (wad != NULL) {
			int n;
				fscanf (wad, "%d", &n);
				printf ("n = %d\n", n);
				fseek (wad, SEEK_CUR, 1);
			char ** str = (char **) malloc (sizeof(char *) * n);
				for (i=0; i<n; i++) {
					str[i] = (char *) calloc (128, sizeof(char));
					fgets (str[i], 128, wad);
					//printf ("содержимое %d: %s\n", i, str[i]);
				}
				fclose (wad);
				remove(wad);
				wad = fopen ("wad.txt", "w");
				fprintf (wad, "%d\n", count_files + n);
				for (i=0; i<n; i++) {
					fputs (str[i], wad);
					free (str[i]);
				}
				free (str);
		}
		else {
			printf ("0\n");
			wad = fopen ("wad.txt", "w");
			fprintf (wad, "%d\n", count_files);
		}
		
		strcat (full_directory, directory);
		
		for (i=0; i<count_files; i++) {
			strcat (full_directory, names[i]);
			printf ("%s\n", full_directory);
			tmp = fopen (full_directory, "r");
			if (tmp == NULL) {
				printf ("Ошибка при открытии файла папки data\n");
				return -1;
			}
			while (1) {
				c = fgetc (tmp);
				if (feof(tmp))
					break;
				fputc (c, wad);
			}
			fprintf (wad, "\n");
			remove (full_directory);
			fclose (tmp);
			for (j=strlen(directory); j<128; j++)
				full_directory[j] = 0;
		}
		fclose (wad);
	return 0;
}