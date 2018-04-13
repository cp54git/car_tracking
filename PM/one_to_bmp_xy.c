#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct track_point {
	double x, y;
	int pic_x, pic_y;
	char * date;
	char * time;
	struct track_point * next;
} track_point;

typedef struct list {
	track_point * head;
	track_point * tail;
	int n;
} list;

typedef struct comma {
	double x, y;
} comma;

typedef struct coor {								// структура 4 крайних точек карты
	comma t_r, t_l, b_r, b_l;
} coor;

typedef struct Setka {								// структура переработанных 4 крайних точек карты
	double A1, B1, C1, A2, B2, C2, lenght1, lenght2;
	int Width, Height;
} Setka;

list * List_init () {
	list * p = (list *) malloc (sizeof(list));
		p->head = NULL;
		p->tail = NULL;
		p->n = 0;
	return p;
}

void push_point (list * p, char * date, char * time, double x, double y) {
	track_point * point = (track_point *) malloc (sizeof(track_point));
		point->date = date;
		point->time = time;
		point->x = x;
		point->y = y;
		point->pic_x = -1;
		point->pic_y = -1;
		point->next = NULL;
		if (p->n == 0) {
			p->head = point;
			p->tail = point;
			p->n = 1;
		}
		else {
			p->tail->next = point;
			p->tail = point;
			p->n++;
		}
	return;
}	

int print_point (list * p) {
		if (p->n == 0) {
			printf ("\nпустой лист\n");
			return -1;
		}
	int i;
	track_point * cur = p->head;
		for (i=1; i <= p->n; i++) {
			printf ("%d: %s %s %lf %lf %d %d\n", i, cur->date, cur->time, cur->x, cur->y, cur->pic_x, cur->pic_y);
			cur = cur->next;
		}
	return i;
}

int coorfile_to_setkafile (FILE ** cor, FILE ** ABC) {
	double A1, B1, C1, A2, B2, C2, lenght1, lenght2, x, y;
	int i, j;
	*cor = fopen ("coor.txt", "r");
		if (cor == NULL) {
				printf ("\nout of map coordinates\n");
			return -1;
		}
	coor * coord = (coor *) malloc (sizeof(coor));
	
		fscanf (*cor, "%d%d", &i, &j);
		fscanf (*cor, "%lf%lf", &x, &y);
			coord->t_l.x = x;
			coord->t_l.y = y;
		fscanf (*cor, "%lf%lf", &x, &y);
			coord->t_r.x = x;
			coord->t_r.y = y;
		fscanf (*cor, "%lf%lf", &x, &y);
			coord->b_l.x = x;
			coord->b_l.y = y;
		fscanf (*cor, "%lf%lf", &x, &y);
			coord->b_r.x = x;
			coord->b_r.y = y;
			
	printf ("	t_l: %lf %lf\n	t_r: %lf %lf\n	b_l: %lf %lf\n	b_r: %lf %lf\n", coord->t_l.x,	coord->t_l.y, coord->t_r.x, coord->t_r.y, coord->b_l.x, coord->b_l.y, coord->b_r.x, coord->b_r.y);		
		lenght1 = sqrt ( pow (coord->t_l.x - coord->b_l.x, 2) + pow (coord->t_l.y - coord->b_l.y, 2) );
		lenght2 = sqrt ( pow (coord->b_l.x - coord->b_r.x, 2) + pow (coord->b_l.y - coord->b_r.y, 2) );
			A1 = coord->b_l.y - coord->b_r.y;
			B1 = coord->b_r.x - coord->b_l.x;
			C1 = coord->b_l.x * coord->b_r.y - coord->b_r.x * coord->b_l.y;
			A2 = coord->t_l.y - coord->b_l.y;
			B2 = coord->b_l.x - coord->t_l.x;
			C2 = coord->t_l.x * coord->b_l.y - coord->b_l.x * coord->t_l.y;
			
		*ABC = fopen ("ABC.txt", "w");
		fprintf (*ABC, "%d %d\n%lf %lf %lf %lf %lf %lf %lf %lf", i, j, A1, B1, C1, A2, B2, C2, lenght1, lenght2);
			fclose (*cor);
			free (coord);
			fclose (*ABC);
		*ABC = fopen ("ABC.txt", "r");
	return 0;	
}

Setka * setka_init (FILE * ABC) {
	double x, y, A1, B1, C1, A2, B2, C2, lenght1, lenght2;
	int i, j;
		fscanf (ABC, "%d%d%lf%lf%lf%lf%lf%lf%lf%lf", &i, &j, &A1, &B1, &C1, &A2, &B2, &C2, &lenght1, &lenght2);
	Setka * setka = (Setka *) malloc (sizeof(Setka));
		setka->Width = i;
		setka->Height = j;
		setka->A1 = A1;
		setka->B1 = B1;
		setka->C1 = C1;
		setka->A2 = A2;
		setka->B2 = B2;
		setka->C2 = C2;
		setka->lenght1 = lenght1;
		setka->lenght2 = lenght2;
	return setka;	
}

void point_to_bmp_coor (track_point * point, Setka * setka) {
	double dx = fabs (setka->A2 * point->x + setka->B2 * point->y + setka->C2) / sqrt (setka->A2*setka->A2 + setka->B2*setka->B2);
	double dy = fabs (setka->A1 * point->x + setka->B1 * point->y + setka->C1) / sqrt (setka->A1*setka->A1 + setka->B1*setka->B1);
		point->pic_x = setka->Width * (dx / setka->lenght1);
		point->pic_y = setka->Height * (dy / setka->lenght2);
	return;
}

int main () {
	char c;
	int count_points, h, m, s, i, j;
	char date[32];
	char time[32];
	double x, y, A1, B1, C1, A2, B2, C2, lenght1, lenght2;
	
	FILE * in = fopen ("wad.txt", "r");				// файл с координатами траектории
	FILE * out = fopen ("bmp_xy.txt", "w");			// файл с обработанными точками траектории
		if (in == NULL || out == NULL) {
				printf ("\n	no such files\n");
			return -1;
		}
		
		fscanf (in, "%d", &count_points);
		printf ("%d\n", count_points);
	
	list * points = List_init ();
	char string [count_points][64];
		for (i=0; i<count_points; i++) {
			fscanf (in, "%s", string[i]);
			for (j=0; j<63; j++)
				if (string[i][j] == '_')
					string[i][j] = ' ';
			sscanf (string[i], "%s%s%lf%lf", date, time, &x, &y);
			push_point (points, date, time, x, y);
		}
		fclose (in);
		print_point (points);

	FILE * 	ABC = fopen ("ABC.txt", "r");		// файл с переработанными 4 точками на карте
	if (ABC == NULL) {
		FILE * cor = fopen ("coor.txt", "r");	// файл с координатами 4 крайних точек на карте и разрешением картинки
		if (coorfile_to_setkafile (&cor, &ABC) == -1)
			return -2;
	}
		
	Setka * setka = setka_init (ABC);
	
	track_point * cur = points->head;
		for (i=0; i<count_points; i++) {
			point_to_bmp_coor (cur, setka);
			cur = cur->next;
		}
		print_point (points);
		
		cur = points->head;
		fprintf (out, "%d\n", count_points);
		for (i=0; i<count_points; i++) {
			sscanf (cur->time, "%d:%d:%d", &h, &m, &s);
			//printf ("%d %d %d\n", h, m, s);
			s += h*3600 + m*60;
			fprintf (out, "%d %d %d\n", s, cur->pic_x, cur->pic_y);
			cur = cur->next;
		}
		fclose (out);
	return 0;
}	