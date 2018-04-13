#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*int Gen_coor (long long t) {
	t = pow (t, 0.75);
	time_t c = time (NULL);
		c %= 41241;
		t %= c;
	return t/332313.123;
}*/

int main () {
	int i, j, n;
	double x, y;
	char dir[] = "./data/", txt[] = ".txt";
	char * full_dir = (char *) calloc (128, sizeof(char));
	char * name = (char *) calloc (64, sizeof(char));
	FILE * out;
	time_t cur_t = time (NULL), e_t = time (NULL);
		strcat (full_dir, dir);
		scanf ("%d", &n);
		e_t += n;
		while (cur_t < e_t) {
			time (&cur_t);
			x = 5.0;
			y = 4.0;
			name = ctime (&cur_t);
			for (j=0; j<64; j++)
				if (name[j] == ' ' || name[j] == '\n')
					name[j] = '_';
			strcat (full_dir, name);
			strcat (full_dir, txt);	
			printf ("%s\n", full_dir);
			out = fopen (full_dir, "w");
			fprintf (out, "%s %lf %lf", name, x, y);
			fclose (out);
			for (j = strlen (dir); j < 128; j++)
				full_dir[j] = 0;
		}
	return 0;
}