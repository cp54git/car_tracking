#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int car_stack();


int main(){
	// код пользователя
	car_stack();
	return 0;
}

int car_stack(){ // для одной даты
	int POG; printf("\nPOG:\n");scanf("%d",&POG); // это "область опасной зоны" , т.е. критическое расстояние между машинами
	FILE* jam = fopen("IMEIstack.txt","r");
	FILE* result = fopen("IMEIresult.txt","w");
	int count; 
	fscanf(jam,"%d",&count); 
	char IMEI[count][15];
	char time0[count][8];
	char time[count][8];
	char xy0[count][19];
	char xy[count][19];
	
	for(int i=0;i<count;i++){
		char *s0;
		s0 = (char*)malloc(55*sizeof(char));
		char *s;
		s = (char*)malloc(55*sizeof(char));		
		fscanf(jam,"%s%s",s0,s); 
		
		for(int j=0;j<15;j++){
			IMEI[i][j]=s[j];
		}
		
		for(int j=27;j<35;j++){
			time0[i][j-27]=s0[j];
			time[i][j-27]=s[j];
		}
		
		for(int j=36;j<55;j++){
			xy0[i][j-36]=s0[j];
			xy[i][j-36]=s[j];
		}
	}
	
	// 0,000001 градус = 0,11111 м => 1м ~ 0,00001 гр
	for(int i=0; i<count;i++){
		for(int j=count-1;j>i ;j--){
			//  берем средний <xy> = (xy+xy0)/2, потом <xy1>=<xy2>+-POG
			
			int xi = (xy[i][8]-'0')+(xy[i][7]-'0')*10+(xy[i][6]-'0')*100+(xy[i][5]-'0')*1000+(xy[i][4]-'0')*10000+(xy[i][3]-'0')*100000+(xy[i][1]-'0')*1000000+(xy[i][0]-'0')*10000000;
			int x0i = (xy0[i][8]-'0')+(xy0[i][7]-'0')*10+(xy0[i][6]-'0')*100+(xy0[i][5]-'0')*1000+(xy0[i][4]-'0')*10000+(xy0[i][3]-'0')*100000+(xy0[i][1]-'0')*1000000+(xy0[i][0]-'0')*10000000;
			int yi = (xy[i][18]-'0')+(xy[i][17]-'0')*10+(xy[i][16]-'0')*100+(xy[i][15]-'0')*1000+(xy[i][14]-'0')*10000+(xy[i][13]-'0')*100000+(xy[i][11]-'0')*1000000+(xy[i][10]-'0')*10000000;
			int y0i = (xy0[i][18]-'0')+(xy0[i][17]-'0')*10+(xy0[i][16]-'0')*100+(xy0[i][15]-'0')*1000+(xy0[i][14]-'0')*10000+(xy0[i][13]-'0')*100000+(xy0[i][11]-'0')*1000000+(xy0[i][10]-'0')*10000000;
			int x_i = (xi+x0i)/2;
			int y_i = (yi+y0i)/2;
			
			int xj = (xy[j][8]-'0')+(xy[j][7]-'0')*10+(xy[j][6]-'0')*100+(xy[j][5]-'0')*1000+(xy[j][4]-'0')*10000+(xy[j][3]-'0')*100000+(xy[j][1]-'0')*1000000+(xy[j][0]-'0')*10000000;
			int x0j = (xy0[j][8]-'0')+(xy0[j][7]-'0')*10+(xy0[j][6]-'0')*100+(xy0[j][5]-'0')*1000+(xy0[j][4]-'0')*10000+(xy0[j][3]-'0')*100000+(xy0[j][1]-'0')*1000000+(xy0[j][0]-'0')*10000000;
			int yj = (xy[j][18]-'0')+(xy[j][17]-'0')*10+(xy[j][16]-'0')*100+(xy[j][15]-'0')*1000+(xy[j][14]-'0')*10000+(xy[j][13]-'0')*100000+(xy[j][11]-'0')*1000000+(xy[j][10]-'0')*10000000;
			int y0j = (xy0[j][18]-'0')+(xy0[j][17]-'0')*10+(xy0[j][16]-'0')*100+(xy0[j][15]-'0')*1000+(xy0[j][14]-'0')*10000+(xy0[j][13]-'0')*100000+(xy0[j][11]-'0')*1000000+(xy0[j][10]-'0')*10000000;
			int x_j = (xj+x0j)/2;
			int y_j = (yj+y0j)/2;
			
			
			// if( xy[i] ~ xy[j] )
			if ( ( (x_i<=x_j+POG && x_i>=x_j-POG) || (x_j<=x_i+POG && x_j>=x_i-POG) ) &&  ( (y_i<=y_j+POG && y_i>=y_j-POG) || (y_j<=y_i+POG && y_j>=y_i-POG) ) ) {
				
				int seci = ((time[i][0]-'0')*10 + time[i][1]-'0')*3600+((time[i][3]-'0')*10 + (time[i][4] -'0'))*60 + (time[i][6]-'0')*10 + (time[i][7]-'0');
				int sec0i = ((time0[i][0]-'0')*10 + time0[i][1]-'0')*3600+((time0[i][3]-'0')*10 + (time0[i][4] -'0'))*60 + (time0[i][6]-'0')*10 + (time0[i][7]-'0');
				int secj = ((time[j][0]-'0')*10 + time[j][1]-'0')*3600+((time[j][3]-'0')*10 + (time[j][4] -'0'))*60 + (time[j][6]-'0')*10 + (time[j][7]-'0');
				int sec0j = ((time0[j][0]-'0')*10 + time0[j][1]-'0')*3600+((time0[j][3]-'0')*10 + (time0[j][4] -'0'))*60 + (time0[j][6]-'0')*10 + (time0[j][7]-'0');
				
				//if ( time[i] ~~ time[j] ) WARNING;
					
				if( (sec0i<=sec0j && seci>=sec0j)  || (sec0i>=sec0j) ){
					for(int tmp=0;tmp<15;tmp++){fprintf(result,"%c",IMEI[i][tmp]);}
					fprintf(result,"      &      ");
					for(int tmp=0;tmp<15;tmp++){fprintf(result,"%c",IMEI[j][tmp]);}
					fprintf(result,"\n\n");
				}
			}
		}
	}
	
	fclose(jam); fclose(result);
	return 0;
}