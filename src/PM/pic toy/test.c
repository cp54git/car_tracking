#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct FIELD {
	char * field;
	int Width, Height;
} FIELD;

typedef struct Color {
	char red, green, blue;
} Color;

char * malloc_char (int n) {
	char * mas = (char *) malloc (n * sizeof(char));
	int i;
		if(mas == NULL) {
			printf ("char malloc error\n"); 
			exit(8);
		}
		for(i=0;i<n;i++) mas[i]=0;
	return mas;
}

int bmp_to_data(char * fdata, char * fbmp) {
	int i, j, n, Height, Width;
	char * mas;
	FILE * in=fopen(fbmp,"r");
	FILE * out=fopen(fdata,"w");
	
		if (in == NULL || out == NULL) {
			printf("%s: file error\n", __FUNCTION__);
			exit(8);
		}
		
	unsigned char bm[54];       //Массив для хранения заголовка
	unsigned int biWidth;           
	unsigned int biHeight;   
			fread((void *)bm, 1, 54, in);
		//printf ("\n%s\n", bm);	
			memcpy((void *)(&biWidth), (void*) (bm + 18), 4);
			memcpy((void *)(&biHeight), (void*) (bm + 22), 4);
		Width = biWidth;
		Height = biHeight;
		n = Height * Width * 3;
			//printf("bp11\n%d\n", n);
		mas = malloc_char (n);
			fread((void *) mas, 1, n, in);
			fprintf(out, "%d %d\n", Width, Height);
			for(i = Height-1; i >= 0; i--) {
				for(j = 0; j < Width; j++)
					fprintf(out, "%4d %4d %4d  ", mas[i*3*Width + j*3+0], mas[i*3*Width + j*3+1], mas[i*3*Width + j*3+2]);
				fprintf(out, "\n");
			}
		fclose (in);
		fclose (out);
	return 0;
}

FIELD * read_field (char * fdata) {
	FIELD * field = (FIELD *) malloc (sizeof (FIELD));
	int i, j, c1, c2, c3;
	int Width, Height, N;
	char * mas;
	FILE * in = fopen (fdata, "r");
	
			if (in == NULL) {
				printf("%s: file error\n", __FUNCTION__);
				exit(8);
			}
			if (fscanf (in,"%d %d", &Width, &Height) != 2)  {
				printf("%s: file error\n", __FUNCTION__);
				exit(8);
			}
			
		N = Width * Height * 3;
		field->Width = Width;
		field->Height = Height;
		field->field = malloc_char (N);
		mas = field->field;
	
		for(i = Height-1; i >= 0; i--)
			for(j=0; j<Width; j++) {
				fscanf(in, "%d%d%d", &c1, &c2, &c3);
				mas[i*3*Width + j*3+0] = c1;
				mas[i*3*Width + j*3+1] = c2;
				mas[i*3*Width + j*3+2] = c3;
			}
		
		field->field = mas;
	return field;
}

void data_to_bmp (char * fbmp1, char * fbmp2, FIELD * Field) {
	char * mas = Field->field;    //Массив для новой кратинки
	unsigned char bm[54];       //Массив для хранения заголовка
	int Height = Field->Height, Width = Field->Width;		
	FILE * in = fopen (fbmp1, "rb"); 
	FILE * out = fopen (fbmp2, "wb");
			
			if (Width % 4) {
				printf("Width некратна 4\n"); 
				exit(1);
			}
			
			if (out == NULL || in == NULL) {
				printf("Err_fopen\n"); 
				exit(3);
			}

		printf("%d * %d\n", Width, Height);
		fread((void*) bm, 1, 54, in);
		fwrite((void*) bm, 1, 54, out);
		fwrite((void*) mas, 1, Height*Width*3, out);
		fclose(out);
	return;
}

Color * Get_color (int x, int y, FIELD * field) {
	Color * color = (Color *) malloc (sizeof (Color));
	int pos = 3 * (x + y * field->Width);
		color->blue = field->field[pos];
		color->red = field->field[pos+1];
		color->green = field->field[pos+2];
	//printf ("got color: %d %d %d\n", color->blue, color->red, color->green);	
	return color;
}

void Set_color (int x, int y, Color * color, FIELD * field) {
	if (x < 0 || x >= field->Width || y < 0 || y >= field->Height)
		return;
	int pos = 3 * (x + y * field->Width);
		field->field[pos] = color->blue;
		field->field[pos+1] = color->red;
		field->field[pos+2] = color->green;
	return;
}	

void Circle (int cent_x, int cent_y, double r, double widness, Color * color, FIELD * field) {
	int i, j, cur_x, cur_y, pos;
	int R = r + widness;
	double f_x, f_y;
	
		for (i = -R; i <= R; i++) {
			cur_x = cent_x + i;
			if (cur_x < field->Width && cur_x >= 0) {
				for (j = -R; j <= R; j++) {
					cur_y = cent_y + j;
					if (cur_y < field->Height && cur_y >= 0) {
						f_x = cur_x - cent_x;
						f_y = cur_y - cent_y;
						if (fabs (sqrt (f_x*f_x + f_y*f_y) - r) < widness) {
							Set_color (cur_x, cur_y, color, field);
							/*pos = cur_x + cur_y * field->Width;
							pos *= 3;
							field->field[pos] = color->blue;
							field->field[pos+1] = color->red;
							field->field[pos+2] = color->green;*/
						}
					}
				}
			}
		}
		
	return;
}

void Fill (int x, int y, Color * cur_col, Color * need_col, FIELD * field) {
	Color * col = Get_color (x, y, field);
	if (cur_col->green == col->green && cur_col->blue == col->blue && cur_col->red == col->red && (x >= 0 && x < field->Width) && (y >= 0 && y < field->Height)) {
			Set_color (x, y, need_col, field);
		/*int pos = 3 * (x + y * field->Width);
			field->field[pos] = need_col->blue;
			field->field[pos+1] = need_col->red;
			field->field[pos+2] = need_col->green;*/
			Fill (x - 1, y, cur_col, need_col, field);
			Fill (x, y - 1, cur_col, need_col, field);
			Fill (x + 1, y, cur_col, need_col, field);
			Fill (x, y + 1, cur_col, need_col, field);
		return;
	}
	else
		return;
}

void Line (int x0, int y0, int x1, int y1, Color * color, FIELD * field) {
    int steep = 0, tmp;
		if (abs(x0-x1)<abs(y0-y1)) {
			tmp = x0;
			x0 = y0;
			y0 = tmp;
			tmp = x1;
			x1 = y1;
			y1 = tmp;
			steep = 1;
		}
		if (x0>x1) {
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
		for (int x=x0; x<=x1; x++) {
			if (steep) {
				Set_color(y, x, color, field);
			} 
			else {
				Set_color(x, y, color, field);
			}
			error2 += derror2;

			if (error2 > dx) {
				y += (y1>y0?1:-1);
				error2 -= dx<<1;
			}
		}
	return;	
}

void Square (int x0, int y0, int x1, int y1, Color * color, FIELD * field) {
	int i, j;
	int stepx = (x0 < x1 ? 1 : -1), stepy = (y0 < y1 ? 1 : -1);
		for (i = x0; i != x1; i += stepx)
			for (j = y0; j != y1; j += stepy)
				Set_color (i, j, color, field);
	return;
}	

int main () {
	char bmp_name[] = "test.bmp";
	char data_bmp_name[] = "test.txt";
	char new_bmp_name[] = "test_1.bmp";
	
		bmp_to_data (data_bmp_name, bmp_name);
	FIELD * field = read_field (data_bmp_name); 
	
	FILE * in = fopen ("pic.txt", "r");
		if (in == NULL) {
			printf ("\nout of circles\n");
			return -3;
		}
		
	int x, y, i, blue, green, red, count_command, com, x1, y1;
	double radius, widness;
	char command[16];
	Color * color = (Color *) malloc (sizeof(Color));
	
		fscanf (in, "%d", &count_command);
		for (i=0; i<count_command; i++) {
			fscanf (in, "%s", command);
			com = command[0];
			switch (com) {
				case 99: {
					fscanf (in, "%d%d%d%d%d%lf%lf", &x, &y, &blue, &red, &green, &radius, &widness);
					color->green = green;
					color->red = red;
					color->blue = blue;
					Circle (x, y, radius, widness, color, field);
					printf ("circle\n");
					break;
				}
				case 102: {
					fscanf (in, "%d%d%d%d%d", &x, &y, &blue, &red, &green);
					color->green = green;
					color->red = red;
					color->blue = blue;
					Fill (x, y, Get_color (x, y, field), color, field);
					printf ("fill\n");
					break;
				}
				case 108: {
					fscanf (in, "%d%d%d%d%d%d%d", &x, &y, &x1, &y1, &blue, &red, &green);
					color->blue = blue;
					color->red = red;
					color->green = green;
					Line (x, y, x1, y1, color, field);
					printf ("line\n");
					break;
				}
				case 115: {
					fscanf (in, "%d%d%d%d%d%d%d", &x, &y, &x1, &y1, &blue, &red, &green);
					color->blue = blue;
					color->red = red;
					color->green = green;
					Square (x, y, x1, y1, color, field);
					printf ("square\n");
					break;
				}
			}
		}
		fclose (in);
		
		data_to_bmp (bmp_name, new_bmp_name, field);
	return 0;
}