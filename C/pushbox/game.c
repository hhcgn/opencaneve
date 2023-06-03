#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define green "\033[42;31m  \033[0m" 
#define red "\033[41;31m  \033[0m"//红色
#define black "\033[40;31m  \033[0m"//黑色
#define yellow "\033[43;37m  \033[0m"
#define blue "\033[44;37m  \033[0m"
#define pink "\033[45;37m  \033[0m"
#define l_blue "\033[46;37m  \033[0m"
#define white "\033[47;37m  \033[0m"

#define box_color    black
#define wall_color    yellow
#define self_color  blue//"\033[46;31m  \033[0m"浅蓝色
#define space_color   green  //"\033[42;31m  \033[0m"
#define target_color    pink   //"\033[43;31m  \033[0m"


#define h 15
#define w 15
#define wall 1
#define box 2
#define space 0
#define self 3
#define target 4
#define up  'w'
#define down 's'
#define left 'a'
#define right 'd'
#define pos(r,c) (r*w+c) 
#define map0 0
#define map1 1
#define map2 2


int r=0,c=0;
int box_x = 5,box_y = 5,self_x=2,self_y=2,target_x=4,target_y=13;
int map[h][w]={0};
void map_init(){
	
	map[box_y][box_x] = box;
	map[self_y][self_x] =self;
		for(r=0;r<h;r++){
		for(c=0;c<w;c++){
			//  if(r==0 || c==0 || r==(h-1) || c==(w-1))
			 if(r==0|| c==0|| r==(h-1) || c==(w-1)
                ||r<=10  &&r>=8  &&c<=8  &&c>=6
                ||r<=7   &&r>=4  &&c<=3  &&c>=0
                ||r<=4   &&r>=0  &&c<=10 &&c>=7
                ||r<=11  &&r>=9  &&c<=13 &&c>=11
              )
				map[r][c]=wall;
			else if(r==self_y&&c==self_x)
				map[r][c]=self;
			else if(r==target_y&&c==target_x)
				map[r][c]=target;
			// printf("%d",map[r*w+c]);
		}
	}
}
void display_map(){
	system("clear");
	for(r=0;r<h;r++){
		for(c=0;c<w;c++){
			switch(map[r][c]){
				case wall:
					printf(wall_color);	
				break;
				case space:
					printf(space_color);
				break;
				case box:
					printf(box_color);
				break;
				case self:
					printf(self_color);
				break;
				case target:
					printf(target_color);
					break;
			}
		}
		printf("\n");

	}
}
void move_control(){
	
	map[self_y][self_x] = space;
	// map[box_y][box_x]=space;
	map[target_y][target_x]=target;
	char cmd=getchar();
	switch(cmd){
		case up:
		if(map[self_y-1][self_x] !=wall ){
			self_y--;
			if(map[self_y][self_x]==box&&map[self_y-1][self_x]!=wall)
				box_y--;
			else if(map[self_y-1][self_x] ==wall &&map[self_y][self_x]==box) self_y++;
			
		}
			
		break;
		case down:
		if(map[self_y+1][self_x] !=wall ){
			self_y++;
			if(map[self_y][self_x]==box&&map[self_y+1][self_x]!=wall)
				box_y++;
			else if(map[self_y][self_x]==box&&map[self_y+1][self_x] ==wall ) self_y--;
			
		}
			
		break;	
		case left:
		if(map[self_y][self_x-1] !=wall){
			self_x--;
			if(map[self_y][self_x]==box&&map[self_y][self_x-1]!=wall)
				box_x--;
			else if(map[self_y][self_x]==box&&map[self_y][self_x-1] ==wall) self_x++;
			
		}
		
		break;	
		case right:
		if(map[self_y][self_x+1] !=wall){
			self_x++;
			if(map[self_y][self_x]==box&&map[self_y][self_x+1]!=wall)
					box_x++;
		else if(map[self_y][self_x]==box&&map[self_y][self_x+1] ==wall)self_x--;
		}
			
		break;
		// case 'r':
		// goto init;
		// break;
	}
	map[box_y][box_x]=box;
	map[self_y][self_x]  = self;

}

void main(){

	system("clear");
	int map[10][10]={0};
	map_init();
    system("stty -icanon");//直接获取键盘值，不用按enter
	 while(1){
		while(1){
	display_map();
	move_control();
	if(box_x==target_x&&box_y==target_y){
		
		display_map();
		printf("YOU WIN !\n");
		break;
	}
	} 
	getchar();
	system("clear");
	printf("input map number\n");
	switch(getchar()){
		case map0:
			map_init();
		break;
		case map1:

		break;
		case map2:

		break;
	 }
	 }
	
}
