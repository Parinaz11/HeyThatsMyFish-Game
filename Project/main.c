#include <stdio.h>
#include <math.h>
#include <time.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <string.h>
#include <stdlib.h>

int player_num();
void random_start(char board[8][8], char last, int c, int b);
void random_fish(int r, int i, int j, char board[8][8], int *count1, int *count2, int *count3);
void showfish(int c, int b, char board[8][8]);
void player_chosing_penguin(int num);
void instructions();
void showpenguins(char board[8][8], char last, int c, int b);
void alph_board(char board[8][8]);
void turn_to_dot(char board[8][8], char p);
int out(char board[8][8], char p, char last);
int check_sides(char board[8][8], int x, int y, char last);
int max(int arr[], int size, int *p);
void winner(int points[], int squares[], int num, char board[8][8]);
int check_corner(char board[8][8], int xp, int yp, int x, int y, char last);
int check_sides(char board[8][8], int x, int y, char last);
int check_for_invalid(char board[8][8], int x, int y, char p, char last);
void penguin_place_start(int num, char board[8][8]);

typedef struct{
	char pen1, pen2, pen3, pen4;
}twoplayer;

typedef struct{
	char pen1, pen2;
}fourplayer;

typedef struct{
	char pen1, pen2, pen3;
}threeplayer;

int main (){
	system("color F1");
	int i, j, num, x, y, a;
	char last;
	al_init();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_ttf_addon();
	al_init_font_addon();
	al_install_mouse();
	ALLEGRO_DISPLAY *display = al_create_display(976, 833);
	char board[8][8];
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			board[i][j] = '0';
		}
	}
	num = player_num();
	penguin_place_start(num, board);
	twoplayer arr2[2];
	threeplayer arr3[3];
	fourplayer arr4[4];
	if(num == 3)
		last = 'K'; //last penguin
	else
		last = 'H';
	al_clear_to_color(al_map_rgb(255, 255, 255));
	ALLEGRO_BITMAP *map = al_load_bitmap("board_p1.png");
	al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
	al_destroy_bitmap(map);
	int c = 76, b = 81; 
	random_start(board, last, c, b);
	showpenguins(board, last, c, b);
	al_flip_display();
	player_chosing_penguin(num); 
	switch(num){
		case 2:
			a = 4;
			break;
		case 4:
			a = 2;
			break;
		case 3:
			a = 3;
			break;
	}
	char t = 'A';
	if(num == 2){
		for(i=0; i<num; i++){
			arr2[i].pen1 = t;
			arr2[i].pen2 = t + 1;
			arr2[i].pen3 = t + 2;
			arr2[i].pen4 = t + 3;
			printf("Player %d has the penguins %c %c %c %c\n", i+1, t, t+1, t+2,t +3);
			t += 4;
		}
	}
	char t2 = 'A';
	if(num == 3){
		for(i=0; i<num; i++){
			arr3[i].pen1 = t2;
			arr3[i].pen2 = t2 + 1;
			arr3[i].pen3 = t2 + 2;
			if(i == 2){
				arr3[i].pen1 = t2;
				arr3[i].pen2 = t2 + 1;
				printf("Player %d has the penguins %c %c ", i+1, t2, t2+1);
				t2 += 2;
				arr3[i].pen3 = t2 + 2; //because the last one is K not I
				printf("%c\n", t2 + 2);
				break;
			}
			printf("Player %d has the penguins %c %c %c\n", i+1, t2, t2+1, t2+2);
			t2 += 3;
		}
	}
	char t3 = 'A';
	if(num == 4){
		for(i=0; i<num; i++){
			arr4[i].pen1 = t3;
			arr4[i].pen2 = t3 + 1;
			printf("Player %d has the penguins %c %c\n", i+1, t3, t3+1);
			t3 += 2;
		}
	} 
	alph_board(board);
	int points[num]; //number of fish taken by each player
	int squares[num]; //number of squares taken by each player
	int lost[num]; //if a player is out, 0 turns to 1
	int count[num]; //checks how many pinguins of a player is out so that if it was all of them, the player is out
	//setting to zero
	for(i=0; i<num; i++){
		points[i] = 0;
		squares[i] = 0; 
		lost[i] = 0;
		count[i] = 0;
	}
	int players = num; 
	char x1, y1;
	int xm, ym;
	while(players != 0){
		al_clear_to_color(al_map_rgb(255, 255, 255));
		ALLEGRO_BITMAP *map = al_load_bitmap("board_p1.png");
		al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
		showfish(c, b, board);
		c = 76;
		b = 81;
		showpenguins(board, last, c, b);
		al_flip_display();
		int q = 300;
		char p;
		for(i=0; i<num; i++){
			if(lost[i] == 0){
				ALLEGRO_FONT *font = al_load_ttf_font("tahoma.ttf", 25, 0);
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
				al_draw_filled_rectangle(100 + q, 20, 280 + q, 50, al_map_rgb(255,255,255));
				al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 20, 0, "<Player %i turn>", i+1);
				showfish(c, b, board);
				showpenguins(board, last, c, b);
				al_flip_display();
				printf("Select the penguin you wish to move with\n");
				al_draw_text(font, al_map_rgb(0, 0, 0), 200, 700, 0, "Select the penguin you wish to move with");	 
				al_flip_display();
				ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
				al_install_mouse();
 			    al_register_event_source(event_queue, al_get_mouse_event_source());
 			    int w = 0;
				while(w == 0){
					ALLEGRO_EVENT ev;
					al_wait_for_event(event_queue, &ev);
				    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				      if(ev.mouse.button == 1){
				          xm = ev.mouse.x;
				          ym = ev.mouse.y;
				          //and clear the board from the text
				          al_clear_to_color(al_map_rgb(255, 255, 255));
				          al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
					 	  al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
				          al_draw_filled_rectangle(100 + q, 20, 280 + q, 50, al_map_rgb(255,255,255));
			              al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 20, 0, "<Player %i turn>", i+1);
				          showfish(c, b, board);
				          showpenguins(board, last, c, b);
			           	  al_flip_display();
				    	}
					}
					else{
						continue;
					}
					int u, k;
					for(u=0; u<8; u++){
						for(k=0; k<8; k++){
							if(xm >= 161 + k*c && xm <= 161 + k*c + 50 && ym >= 68 + u*b && ym <= 68 + u*b + 58){
								p = board[u][k];
								w = 1;
							}
						}
					}
				}
				//if the player had not chosen a penguin
				while(!('A' <= p && p <= last)){
					w = 0;
					printf("Please select a penguin.\n");
					al_draw_text(font, al_map_rgb(255, 0, 0), 200, 700, 0, "Please select a penguin");	 
					al_flip_display();
					while(w == 0){
						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);
					    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
					      if(ev.mouse.button == 1){
					          xm = ev.mouse.x;
					          ym = ev.mouse.y;
					          //clearing the screen from the text
					          al_clear_to_color(al_map_rgb(255, 255, 255));
					          al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
						 	  al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
					          al_draw_filled_rectangle(100 + q, 20, 280 + q, 50, al_map_rgb(255,255,255));
				              al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 20, 0, "<Player %i turn>", i+1);
					          showfish(c, b, board);
					          showpenguins(board, last, c, b);
				           	  al_flip_display();
					    	}
						}
						else{
							continue;
						}
						int u, k;
						for(u=0; u<8; u++){
							for(k=0; k<8; k++){
								if(xm >= 161 + k*c && xm <= 161 + k*c + 50 && ym >= 68 + u*b && ym <= 68 + u*b + 58){
									p = board[u][k];
									w = 1;
								}
							}
						}
					}
				}
				w = 0;
				//if the penguin was not for that player in two player mode
				while(num == 2 && (p != arr2[i].pen1 && p != arr2[i].pen2 && p != arr2[i].pen3 && p != arr2[i].pen4)){
					w = 0;
					printf("The penguin is not yours, please select again\n");
					al_draw_text(font, al_map_rgb(255, 0, 0), 200, 700, 0, "The penguin is not yours, please select again");	 
					al_flip_display();
					while(w == 0){
						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);
					    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
					      if(ev.mouse.button == 1){
					          xm = ev.mouse.x;
					          ym = ev.mouse.y;
					          //clearing the screen
					          al_clear_to_color(al_map_rgb(255, 255, 255));
					          al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
						 	  al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
					          al_draw_filled_rectangle(100 + q, 20, 280 + q, 50, al_map_rgb(255,255,255));
				              al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 20, 0, "<Player %i turn>", i+1);
					          showfish(c, b, board);
					          showpenguins(board, last, c, b);
				           	  al_flip_display();
					    	}
						}
						else{
							continue;
						}
						int u, k;
						for(u=0; u<8; u++){
							for(k=0; k<8; k++){
								if(xm >= 161 + k*c && xm <= 161 + k*c + 50 && ym >= 68 + u*b && ym <= 68 + u*b + 58){
									p = board[u][k];
									w = 1;
								}
							}
						}
					}
				}
				//if the penguin was not for that player in three player mode
				while(num == 3 && (p != arr3[i].pen1 && p != arr3[i].pen2 && p != arr3[i].pen3)){
					w = 0;
					printf("The penguin is not yours, please select again\n");
					al_draw_text(font, al_map_rgb(255, 0, 0), 200, 700, 0, "The penguin is not yours, please select again");	
					al_flip_display();
					while(w == 0){
						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);
					    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
					      if(ev.mouse.button == 1){
					          xm = ev.mouse.x;
					          ym = ev.mouse.y;
					          //clearing from the text
					          al_clear_to_color(al_map_rgb(255, 255, 255));
					          al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
						 	  al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
					          al_draw_filled_rectangle(100 + q, 20, 280 + q, 50, al_map_rgb(255,255,255));
				              al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 20, 0, "<Player %i turn>", i+1);
					          showfish(c, b, board);
					          showpenguins(board, last, c, b);
				           	  al_flip_display();
					    	}
						}
						else{
							continue;
						}
						int u, k;
						for(u=0; u<8; u++){
							for(k=0; k<8; k++){
								if(xm >= 161 + k*c && xm <= 161 + k*c + 50 && ym >= 68 + u*b && ym <= 68 + u*b + 58){
									p = board[u][k];
									w = 1;
								}
							}
						}
					}
				}
				//if the penguin was not for that player in four player mode
				while(num == 4 && (p != arr4[i].pen1 && p != arr4[i].pen2)){
					w = 0;
					printf("The penguin is not yours, please select again\n");
					al_draw_text(font, al_map_rgb(255, 0, 0), 200, 700, 0, "The penguin is not yours, please select again");
					al_flip_display();
					while(w == 0){
						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);
					    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
					      if(ev.mouse.button == 1){
					          xm = ev.mouse.x;
					          ym = ev.mouse.y;
					          //clearing screen from text
					          al_clear_to_color(al_map_rgb(255, 255, 255));
					          al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
						 	  al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
					          al_draw_filled_rectangle(100 + q, 20, 280 + q, 50, al_map_rgb(255,255,255));
				              al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 20, 0, "<Player %i turn>", i+1);
					          showfish(c, b, board);
					          showpenguins(board, last, c, b);
				           	  al_flip_display();
					    	}
						}
						else{
							continue;
						}
						int u, k;
						for(u=0; u<8; u++){
							for(k=0; k<8; k++){
								if(xm >= 161 + k*c && xm <= 161 + k*c + 50 && ym >= 68 + u*b && ym <= 68 + u*b + 58){
									p = board[u][k];
									w = 1;
								}
							}
						}
					}
				}
				int check;
				printf("penguin = %c\n", p);
				check = out(board, p, last);
				//clearing screen from the text left from out function
	            al_clear_to_color(al_map_rgb(255, 255, 255));
	            al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
		 	    al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
	            al_draw_filled_rectangle(100 + q, 20, 280 + q, 50, al_map_rgb(255,255,255));
                al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 20, 0, "<Player %i turn>", i+1);
	            showfish(c, b, board);
	            showpenguins(board, last, c, b);
           	    al_flip_display();
				if(check == 0){
					count[i]++;	
				}
				if(last == 'H'){
					if(count[i] == 8 / num){
						lost[i] = 1;
						printf("*Player %d is out.*\n", i+1);
						al_draw_textf(font, al_map_rgb(0, 0, 0), 200, 700, 0, "*Player %d is out.*", i+1);	
						al_flip_display();
						al_rest(1.0);
						players--;
						continue;
					}
				}
				if(last == 'K'){
					if(count[i] == 3){
						lost[i] = 1;
						printf("*Player %d is out.*\n", i+1);
						al_draw_textf(font, al_map_rgb(0, 0, 0), 200, 700, 0, "*Player %d is out.*", i+1);	 
						al_flip_display();
						al_rest(1.0);
						players--;
						continue;
					}
				}
				if(check == 0){
					//if the pernguin the player entered was out, its next players'turn
					continue;
				}
				printf("Select the square you wish to move to\n");
				al_draw_text(font, al_map_rgb(0, 0, 0), 200, 700, 0, "Select the square you wish to move to");	 
				al_flip_display();
				al_install_mouse();
 			    al_register_event_source(event_queue, al_get_mouse_event_source());
 			    w = 0;
				while(w == 0){
					ALLEGRO_EVENT ev;
					al_wait_for_event(event_queue, &ev);
				    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
				      if(ev.mouse.button == 1){
				          xm = ev.mouse.x;
				          ym = ev.mouse.y;
				          //clearing the screen from text
				          al_clear_to_color(al_map_rgb(255, 255, 255));
				          al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
					 	  al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
				          al_draw_filled_rectangle(100 + q, 20, 280 + q, 50, al_map_rgb(255,255,255));
			              al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 20, 0, "<Player %i turn>", i+1);
				          showfish(c, b, board);
				          showpenguins(board, last, c, b);
			           	  al_flip_display();
				    	}
					}
					else{
						continue;
					}
					int u, k; 
					for(u=0; u<8; u++){
						for(k=0; k<8; k++){
							if(xm >= 161 + k*c && xm <= 161 + k*c + 50 && ym >= 68 + u*b && ym <= 68 + u*b + 58){
								x = u;
								y = k;
								w = 1;
							}
						}
					}
				}
				//checking if it was invalid
				while(check_for_invalid(board, x, y, p, last)){
					printf("The square you chose is out of your access, please select another x and y\n ");
					al_draw_text(font, al_map_rgb(255, 0, 0), 200, 700, 0, "The square you chose is out of your access, please select again");
					al_flip_display();
					al_install_mouse();
	 			    al_register_event_source(event_queue, al_get_mouse_event_source());
	 			    w = 0;
					while(w == 0){
						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);
					    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
					      if(ev.mouse.button == 1){
					          xm = ev.mouse.x;
					          ym = ev.mouse.y;
					          //clear the screen
					          al_clear_to_color(al_map_rgb(255, 255, 255));
					          al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
						 	  al_draw_scaled_bitmap(map, 0, 0, 576, 433, 0, 0, 976, 833, 0);
					          al_draw_filled_rectangle(100 + q, 20, 280 + q, 50, al_map_rgb(255,255,255));
				              al_draw_textf(font, al_map_rgb(0, 0, 0), 400, 20, 0, "<Player %i turn>", i+1);
					          showfish(c, b, board);
					          showpenguins(board, last, c, b);
				           	  al_flip_display();
					    	}
						}
						else{
							continue;
						}
						int u, k;
						for(u=0; u<8; u++){
							for(k=0; k<8; k++){
								if(xm >= 161 + k*c && xm <= 161 + k*c + 50 && ym >= 68 + u*b && ym <= 68 + u*b + 58){
									x = u;
									y = k;
									w = 1;
								}
							}
						}
					}
				}
				al_destroy_event_queue(event_queue);
				//adding to the players points and squares
				points[i] += board[x][y] - '0'; 
				squares[i]++; 
				turn_to_dot(board, p);
				board[x][y] = p;
				alph_board(board);
				al_uninstall_mouse();
			}
		}
		//al_draw_scaled_bitmap(one, 0, 0, 141, 165, 161, 68, 50, 58, 0);
	}
	winner(points, squares, num, board);
	printf("\n");
	int x_w = 260, y_w = 345, g = 130, q = 300;
	for(i=0; i<num; i++){
		y_w += 50;
		ALLEGRO_FONT *font = al_load_ttf_font("tahoma.ttf", 25, 0);
		al_draw_filled_rectangle(x_w - 300 + q, y_w, x_w - 120 + g + q + 25, y_w + 30, al_map_rgb(136,140,141));
		al_draw_textf(font, al_map_rgb(255, 255, 255), x_w , y_w, 0, "Player %d:  %d fish  %d squares", i+1, points[i], squares[i]);
		al_flip_display();
		printf("Player %d:  %d fish  %d squares\n", i+1, points[i], squares[i]);
	}
	ALLEGRO_BITMAP *end = al_load_bitmap("finish.png");
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_install_mouse();
    al_register_event_source(event_queue, al_get_mouse_event_source());
	int w = 0;
	while(w == 0){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
	    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
	      if(ev.mouse.button == 1){
	          al_draw_scaled_bitmap(end, 0, 0, 995, 800, 0, 0, 976, 833, 0);
	          al_flip_display();
	          w = 1;
	      }
		}
	}
	printf("\n\nThank you for playing, hope you enjoyed:)\n");
	printf("***THE END***");
    w = 0;
	while(w == 0){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
	    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
	      if(ev.mouse.button == 1){
	          al_destroy_display(display);
	          w = 1;
	      }
		}
	}
	al_destroy_event_queue(event_queue);
	return 0;
}

int player_num(){
	//This function gets the number of players at the start of the game and returns it to the main
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR); //for the quality of the image
	ALLEGRO_BITMAP *image = al_load_bitmap("start2.jpg");
	ALLEGRO_FONT *font = al_load_ttf_font("tahoma.ttf", 25, 0);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_mouse_event_source());
	//the number of players
	al_draw_scaled_bitmap(image, 0, 0, 512, 512, 0, 0, 976, 833, 0);
	al_draw_text(font, al_map_rgb(255, 255, 255), 250, 20, 0, "Welcome to the game <Hey That's My Fish!>");	
	al_draw_text(font, al_map_rgb(255, 255, 255), 300, 80, 0, "Select the number of players:");	
	int c = 300;
	//showing the options
	al_draw_filled_rectangle(50 + c, 150, 100 + c, 200, white);
	al_draw_text(font, black, 70+c, 155, 0, "2");	
	al_draw_filled_rectangle(150+c, 150, 200 + c, 200, white);
	al_draw_text(font, black, 170+c, 155, 0, "3");	
	al_draw_filled_rectangle(250+c, 150, 300 + c, 200, white);
	al_draw_text(font, black, 270+c, 155, 0, "4");	
	al_flip_display();
	int x, y;
	//player selecting an option
	while(1){	
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			if(ev.mouse.button & 1){
				x = ev.mouse.x;
				y = ev.mouse.y;
			}
		}
		if(x >= 50+c && x <= 100+c && y >= 150 && y <= 200)
			return  2;
		if(x >= 150 && x <= 200+c && y >= 150 && y <= 200)
			return  3;
		if(x >= 250 && x <= 300+c && y >= 150 && y <= 200)
			return  4;
	}
	
}

void penguin_place_start(int num, char board[8][8]){
	//players choosing where to put their penguins 
	int i, u;
	ALLEGRO_FONT *font = al_load_ttf_font("tahoma.ttf", 25, 0);
	ALLEGRO_BITMAP *choose = al_load_bitmap("square_board_p1.png");
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_mouse_event_source());
	int xm, ym;
	int t = 'A';
	for(i=0; i<num; i++){
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_draw_scaled_bitmap(choose, 0, 0, 558, 447, 0, 0, 976, 833, 0);
		al_install_mouse();
		if(num == 3){
			for(u=0; u<3; u++){
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_scaled_bitmap(choose, 0, 0, 558, 447, 0, 0, 976, 833, 0);
				al_draw_textf(font, al_map_rgb(0, 0, 0), 100, 20, 0, "Player %d choose where you want to put your %d penguin", i+1, u+1);
				al_flip_display();
				int w = 0;
				while(w == 0){	
					if(t == 'K')
						break;
					ALLEGRO_EVENT ev;
					al_wait_for_event(event_queue, &ev);
					if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
						if(ev.mouse.button & 1){
							xm = ev.mouse.x;
							ym = ev.mouse.y;
						}
					}
					else{
						continue;
					}
					int r, f, x, y, c = 76, b = 81; 
					for(r=0; r<8; r++){
						for(f=0; f<8; f++){
							if(xm >= 161 + f*c && xm <= 161 + f*c + 50 && ym >= 68 + r*b && ym <= 68 + r*b + 58){
								x = r;
								y = f;
								w = 1;
							}
						}
					}
					//checking for the square to not be taken by another penguin
					int w2 = 0, ch = 0;
					while((('A' <= board[x][y] && board[x][y] <= 'H') || (board[x][y] == 'K')) && w2 == 0){
						if(ch == 0){
							printf("There is already a penguin there, please select again\n");
							al_draw_text(font, al_map_rgb(255, 0, 0), 200, 700, 0, "There is already a penguin there, please select again");
							al_flip_display();
							al_rest(2.0);
							ch = 1;
						}
						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);
						if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
							if(ev.mouse.button & 1){
								xm = ev.mouse.x;
								ym = ev.mouse.y;
							}
						}
						else{
							continue;
						}
						for(r=0; r<8; r++){
							for(f=0; f<8; f++){
								if(xm >= 161 + f*c && xm <= 161 + f*c + 50 && ym >= 68 + r*b && ym <= 68 + r*b + 58){
									x = r;
									y = f;
									if(('A' <= board[x][y] && board[x][y] <= 'H') || (board[x][y] == 'K')){
										w2 = 0;
										printf("There is already a penguin there, please select again\n");
										al_draw_text(font, al_map_rgb(255, 0, 0), 200, 700, 0, "There is already a penguin there, please select again");
										al_flip_display();
										al_rest(2.0);
									}
									else
										w2 = 1;
								}
							}
						}
					}
					board[x][y] = t; //becuase letters are automatically given to the players
					t++;
					if(board[x][y] == 'I')
						board[x][y] = 'K';
					printf("board[%d][%d] = %c\n", x, y, board[x][y]);
				}
				
			}
		}
		else{
			int k;
			k = 8 / num;
			for(u=0; u<k; u++){
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_scaled_bitmap(choose, 0, 0, 558, 447, 0, 0, 976, 833, 0);
				al_draw_textf(font, al_map_rgb(0, 0, 0), 100, 20, 0, "Player %d choose where you want to put your %d penguin", i+1, u+1);
				al_flip_display();
				int w = 0;
				while(w == 0 && t <= 'H'){	
					ALLEGRO_EVENT ev;
					al_wait_for_event(event_queue, &ev);
					if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
						if(ev.mouse.button & 1){
							xm = ev.mouse.x;
							ym = ev.mouse.y;
						}
					}
					else{
						continue;
					}
					int r, f, x, y, c = 76, b = 81; 
					for(r=0; r<8; r++){
						for(f=0; f<8; f++){
							if(xm >= 161 + f*c && xm <= 161 + f*c + 50 && ym >= 68 + r*b && ym <= 68 + r*b + 58){
								x = r;
								y = f;
								w = 1;
							}
						}
					}
					//checking for the square to not be taken by another penguin
					//-----------------------------------------------------------------------------------------------------------------------
					int w2 = 0, ch = 0;
					while((('A' <= board[x][y] && board[x][y] <= 'H') || (board[x][y] == 'K')) && w2 == 0){
						if(ch == 0){
							printf("There is already a penguin there, please select again\n");
							al_draw_text(font, al_map_rgb(255, 0, 0), 200, 700, 0, "There is already a penguin there, please select again");
							al_flip_display();
							al_rest(2.0);
							ch = 1;
						}
						ALLEGRO_EVENT ev;
						al_wait_for_event(event_queue, &ev);
						if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
							if(ev.mouse.button & 1){
								xm = ev.mouse.x;
								ym = ev.mouse.y;
							}
						}
						else{
							continue;
						}
						for(r=0; r<8; r++){
							for(f=0; f<8; f++){
								if(xm >= 161 + f*c && xm <= 161 + f*c + 50 && ym >= 68 + r*b && ym <= 68 + r*b + 58){
									x = r;
									y = f;
									if(('A' <= board[x][y] && board[x][y] <= 'H')){
										printf("There is already a penguin there, please select again\n");
										al_draw_text(font, al_map_rgb(255, 0, 0), 200, 700, 0, "There is already a penguin there, please select again");
										al_flip_display();
										al_rest(2.0);
										w2 = 0;
									}
									else
										w2 = 1;
								}
							}
						}
					}
					board[x][y] = t;
					t++;
					printf("board[%d][%d] = %c\n", x, y, board[x][y]);
				}
				
			}
		}
	}
	al_uninstall_mouse();	
}
		
	

void random_start(char board[8][8], char last, int c, int b){
	int i, j, r, count1=0, count2=0, count3=0;
	al_init_image_addon();
	ALLEGRO_BITMAP *one = al_load_bitmap("one.png"),
		*two = al_load_bitmap("two.png"), *three = al_load_bitmap("three.png");
	//randomly putting fish at start
	srand(time(NULL)); 
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			r = rand() % 3 + 1;
			if('A' <= board[i][j] && board[i][j] <= last)
				continue;
			board[i][j] = r + '0';
			random_fish(r, i, j, board, &count1, &count2, &count3);
		}
	}
	al_flip_display();
	//randomly putting penguin at start 
	/*int r1, r2, u;
	char a;
	a = 'A';
	if(last == 'K')
		u = 9;
	else
		u = 8;
	for(i=0; i<u; i++){ 
		r1 = rand() % 8; 
		r2 = rand() % 8; 
		if(!('A' <= board[r1][r2] && board[r1][r2] <= last) && board[r1][r2] == '1'){ 
			//if there wasn't a penguin in that part already
			board[r1][r2] = a;
			a++;
			if(i == 8)
				board[r1][r2] = 'K';
		}
  		else{
			do{
				r1 = rand() % 8; 
				r2 = rand() % 8; 
			}while(('A' <= board[r1][r2] && board[r1][r2] <= last) || board[r1][r2] != '1');
			board[r1][r2] = a;
			a++;
			if(i == 8)
				board[r1][r2] = 'K';
		}
	}*/
	
}

void random_fish(int r, int i, int j, char board[8][8], int *count1, int *count2, int *count3){ 
	int c = 76; 
	int b = 81; 
	switch(r){
		case 1:
			(*count1)++;
			break;
		case 2:
			(*count2)++;
			break;
		case 3:
			(*count3)++;
			break;	
	}
	
	if(board[i][j] == '2' && (*count2) > 20){
		if((*count1) < 34)
			board[i][j] = '1';
		else 
			board[i][j] = '3';
	}
	if(board[i][j] == '1' && (*count1) > 34){
		if((*count2) < 20)
			board[i][j] = '2';
		else 
			board[i][j] = '3';
	}
	if(board[i][j] == '3' && (*count3) > 10){
		if((*count1) < 34)
			board[i][j] = '1';
		else 
			board[i][j] = '2';
	}
	
	showfish(c, b, board);
	al_flip_display();
}

void showfish(int c, int b, char board[8][8]){
	al_init_image_addon();
	ALLEGRO_BITMAP *one = al_load_bitmap("one.png"),
		*two = al_load_bitmap("two.png"), *three = al_load_bitmap("three.png");
	int i, j;
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(board[i][j] == '1')
				al_draw_scaled_bitmap(one, 0, 0, 141, 165, 161 + j*c, 68 + i*b , 50, 58, 0);
			else if(board[i][j] == '2')
				al_draw_scaled_bitmap(two, 0, 0, 138, 158, 161 + j*c, 68 + i*b , 50, 58, 0);
			else if(board[i][j] == '3')
				al_draw_scaled_bitmap(three, 0, 0, 141, 164, 161 + j*c, 68 + i*b , 50, 58, 0);
			
		}
	}
	al_flip_display();
}

void player_chosing_penguin(int num){
	/*in this function, each player choses a penguin and the number of players
 	  in returned so that we would have further use of it in the game*/
	int p, i, j, a, last;
	instructions();
	printf("**START!**\n");
	if(num == 2 || num == 4){
		printf("The 8 penguins are shown in the board.\n");
		printf("There are %d penguins for each player.\n", 8/num);
	}
	else if(num == 3){
		printf("The 9 penguins are shown in the board.\n");
		printf("There are 3 penguins for each player.\n");
	}
	
}

void alph_board(char board[8][8]){
	int i, j;
	printf(" ");
	for(i=1; i<9; i++){ 
		printf("    %d", i);
	}
	printf("\n\n");
	for(i=0; i<8; i++){ 
		for(j=0; j<8; j++){
			if(j == 0){
				printf("%d    ", i+1);
			}
			printf("%c    ", board[i][j]);
		}
		printf("\n\n");
	}
}

void showpenguins(char board[8][8], char last, int c, int b){
	int i, j;
	al_init_image_addon();
	ALLEGRO_BITMAP *p_one = al_load_bitmap("penguin1.jpeg"),
	*p_two = al_load_bitmap("penguin2.jpeg"), *p_three = al_load_bitmap("penguin3.jpeg"),
	*p_four = al_load_bitmap("penguin4.jpeg"), *p_five = al_load_bitmap("penguin5.jpeg"),
	*p_six = al_load_bitmap("penguin6.jpeg"), *p_seven = al_load_bitmap("penguin7.jpeg"),
	*p_eight = al_load_bitmap("penguin8.jpeg"), *p_nine = al_load_bitmap("penguin9.jpeg"),
	*ice = al_load_bitmap("ice.jpg");
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(board[i][j] == 'A')
				al_draw_scaled_bitmap(p_one, 0, 0, 656, 1080, 161 + j*c, 68 + i*b , 50, 68, 0);
			else if(board[i][j] == 'B')
				al_draw_scaled_bitmap(p_two, 0, 0, 656, 1080, 161 + j*c, 68 + i*b , 50, 68, 0);
			else if(board[i][j] == 'C')
				al_draw_scaled_bitmap(p_three, 0, 0, 656, 1080, 161 + j*c, 68 + i*b , 50, 68, 0);
			else if(board[i][j] == 'D')
				al_draw_scaled_bitmap(p_four, 0, 0, 656, 1080, 161 + j*c, 68 + i*b , 50, 68, 0);
			else if(board[i][j] == 'E')
				al_draw_scaled_bitmap(p_five, 0, 0, 656, 1080, 161 + j*c, 68 + i*b , 50, 68, 0);
			else if(board[i][j] == 'F')
				al_draw_scaled_bitmap(p_six, 0, 0, 656, 1080, 161 + j*c, 68 + i*b , 50, 68, 0);
			else if(board[i][j] == 'G')
				al_draw_scaled_bitmap(p_seven, 0, 0, 656, 1080, 161 + j*c, 68 + i*b , 50, 68, 0);
			else if(board[i][j] == 'H')
				al_draw_scaled_bitmap(p_eight, 0, 0, 656, 1080, 161 + j*c, 68 + i*b , 50, 68, 0);
			else if(board[i][j] == 'K')
				al_draw_scaled_bitmap(p_nine, 0, 0, 656, 1080, 161 + j*c, 68 + i*b , 50, 68, 0);
			else if(board[i][j] == '.')
				al_draw_scaled_bitmap(ice, 0, 0, 255, 255, 161 + j*c, 68 + i*b , 50, 58, 0);
		}
	}
}

int out(char board[8][8], char p, char last){
	//if the penguin  was surrounded from all 8 sides by dots or other penguins, it is out;
	ALLEGRO_FONT *font = al_load_ttf_font("tahoma.ttf", 25, 0);
	int i, j, x, y, check;
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(board[i][j] == p){
				x = i;
				y = j;
			}
		}
	}
	//if there was dots and penguins in all 8 sides
	check = check_sides(board, x, y, last); 
	if(check == 0){
		board[x][y] = '.';
		alph_board(board);
		printf("*Penguin %c is out*\n", p);
		int p1;
		p1 = p - 64;
		if(p == 'K')
			p1 = 9;
		al_draw_textf(font, al_map_rgb(0, 0, 0), 200, 700, 0, "Penguin %d is out", p1);	
		al_flip_display();
		al_rest(1.0);
		return 0;
	}
	return 1;
}

int check_sides(char board[8][8], int x, int y, char last){
	/*if a penguin was surrounded by othere penguins or dots and had no where to go,
	this function returns 0 instead of 1 and tells us that the penguin is out*/
	int u, yes=0;
	char peng[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', '\0'};
	if(last == 'H'){
		u = 8;
	}	
	else{
		u = 9;
		peng[8] = 'K';;
		peng[9] = '\0';
	}
	int check1=0, check2=0, check3=0, check4=0, i;
	//because the penguin can not go up or down
	if(y-1 >= 0){
		check1 = 1;
		for(i=0; i<u; i++){ 
			if(board[x][y-1] == peng[i] || board[x][y-1] == '.')
				check1 = 0;	
		}
	}
	if(y+1 <= 7){ 
		check2 = 1;
		for(i=0; i<u; i++){ 
			if(board[x][y+1] == peng[i] || board[x][y+1] == '.')
				check2 = 0;
		}
	}
	
	/*if(x-1 >= 0){
		check3 = 1;
		for(i=0; i<u; i++){ 
			if(board[x-1][y] == peng[i] || board[x-1][y] == '.')
				check3 = 0;	
		}
	}
	if(x+1 <= 7){ 
		check4 = 1;
		for(i=0; i<u; i++){ 
			if(board[x+1][y] == peng[i] || board[x+1][y] == '.')
				check4 = 0;
		}
	}*/
	//checking for corners
	int check5=0, check6=0, check7=0, check8=0;
	if(x+1 <= 7 && y+1 <= 7){
		check5 = 1;
		for(i=0; i<u; i++){ 
			if(board[x+1][y+1] == peng[i] || board[x+1][y+1] == '.')
				check5 = 0;
		}
	}
	if(x+1 <= 7 && y-1 >= 0){
		check6 = 1;
		for(i=0; i<u; i++){ 
			if(board[x+1][y-1] == peng[i] || board[x+1][y-1] == '.')
				check6 = 0;
		}
	}
	if(x-1 >= 0 && y+1 <= 7){ 
		check7 = 1;
		for(i=0; i<u; i++){ 
			if(board[x-1][y+1] == peng[i] || board[x-1][y+1] == '.')
				check7 = 0;
		}
	}
	if(x-1 >= 0 && y-1 >= 0){ 
		check8 = 1;
		for(i=0; i<u; i++){ 
			if(board[x-1][y-1] == peng[i] || board[x-1][y-1] == '.')
				check8 = 0;
		}
	}
	if(check1==0 && check2==0 && check3==0 && check4==0)
		yes++;
	if(check5==0 && check6==0 && check7==0 && check8==0)
		yes++;
	if(yes == 2){
		return 0;
	}
	return 1;
}


void turn_to_dot(char board[8][8], char p){
	//turning the passed squares to a dot
	int i, j;
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(board[i][j] == p){
				board[i][j] = '.';
			}
		}
	}
}

void winner(int points[], int squares[], int num, char board[8][8]){
	int xw = 270, yw = 295, q = 300, r = 130;
	al_init_ttf_addon();
	al_init_font_addon();
	//this function announces which player has won
	int m, s, i, a, repeat=0, p=0;
	m = max(points, num, &repeat);
	if(repeat != 0){
		a = max(squares, num, &p);
		if(p != 0){ //if the squares of more than one player was equal
			for(i=0; i<num; i++){
				if(squares[i] == a && points[i] == m){
					ALLEGRO_FONT *font = al_load_ttf_font("tahoma.ttf", 25, 0);
					al_draw_filled_rectangle(xw - 300 + q, yw, xw - 120 + q + r + 200, yw + 30, al_map_rgb(32,178,170));
					al_draw_textf(font, al_map_rgb(250, 250, 51), xw, yw, 0, "Player %d wins with %d fish and %d squares!", i+1, points[i], a);
					al_flip_display();
					al_rest(5.0); //so that if we have more than one winner, it shows on the same spot
					printf("Player %d wins with %d fish and %d squares!", i+1, points[i], a);
				}
			}
		}
		else{
			for(i=0; i<num; i++){
				if(squares[i] == a){
					ALLEGRO_FONT *font = al_load_ttf_font("tahoma.ttf", 25, 0);
					al_draw_filled_rectangle(xw - 300 + q, yw, xw - 120 + r + q + 200, yw + 30, al_map_rgb(32,178,170));
					al_draw_textf(font, al_map_rgb(250, 250, 51), xw, yw, 0, "Player %d wins with %d fish and %d squares!", i+1, points[i], a);
					al_flip_display();
					//al_rest(10.0);
					printf("Player %d wins with %d fish and %d squares!", i+1, points[i], a);
				}
			}
		}
	}
	else{
		for(i=0; i<num; i++){
			if(points[i] == m){
				ALLEGRO_FONT *font = al_load_ttf_font("tahoma.ttf", 25, 0);
				al_draw_filled_rectangle(xw - 300 + q, yw, xw - 120 + r + q, yw + 30, al_map_rgb(32,178,170));
				al_draw_textf(font, al_map_rgb(250, 250, 51), xw, yw, 0, "Player %d wins with %d fish!", i+1, m);
				al_flip_display();
				printf("Player %d wins with %d fish!", i+1, m);
			}
		}
	}
}

int max(int arr[], int size, int *p){
	//finding out the max in an array, if there was more than one maxes, p = 1 and max is returned
	int max = -1, i, count=0;
	for(i=0; i<size; i++){
		if(arr[i] > max)
			max = arr[i];
	}
	//checking for more than one maxes
	for(i=0; i<size; i++){
		if(arr[i] == max){
			count++;
		}
	}
	if(count == 1)
		*p = 0;
	else
		*p = 1;
	
	return max;	
}

int check_for_invalid(char board[8][8], int x, int y, char p, char last){
	//this function checks wether the penguins' move is valid or not and returns 0 it its okay
	int i, j, xp, yp, m=0;
	//if they crossed the array limit
	if(x < 0 || x > 7 || y < 0 || y > 7){
		return 1;
	}
	//checking for the straight line
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(board[i][j] == p){
				xp = i;
				yp = j;
			}
		}
	}
	//check for empty squares on the way(dots) and penguins except for corners
	if(xp == x){
		if(yp > y){
			for(i=yp-1; i>=y; i--){
				if(board[x][i] == '.' || ('A' <= board[x][i] && board[x][i] <= last)){
					return 1;
				}
			}
		}
		else{
			for(i=yp+1; i<=y; i++){
				if(board[x][i] == '.' || ('A' <= board[x][i] && board[x][i] <= last)){
					return 1;
				}
			}
		}
		return 0;
	}
	else if(yp == y){
		return 1;
		/*if(xp > x){
			for(i=xp-1; i>=x; i--){
				if(board[i][y] == '.' || ('A' <= board[i][y] && board[i][y] <= last)){
					return 1;
				}
			}
		}
		else{
			for(i=xp+1; i<=x; i++){
				if(board[i][y] == '.' || ('A' <= board[i][y] && board[i][y] <= last)){
					return 1;
				}
			}
		}
		return 0;*/
	}
	//checking for corners
	m = check_corner(board, xp, yp, x, y, last);
	if(m == 1)
		return 1;
	return 0;
}

int check_corner(char board[8][8], int xp, int yp, int x, int y, char last){
	/*this function checks whether there is a dot or another penguin on
	the penguins' way to board[x][y] and if there is, it returns 1, if there isn't it returns 0*/
	if(xp == x && yp == y){
		return 0;
	}
	if(abs(x - xp) != abs(y - yp)){
		return 1;
	}
	else if(x > xp && y > yp){
		xp++;
		yp++;
		if(board[xp][yp] == '.' || ('A' <= board[xp][yp] && board[xp][yp] <= last))
			return 1;
		else
			check_corner(board, xp, yp, x, y, last);
	}
	else if(x > xp && y < yp){
		xp++;
		yp--;
		if(board[xp][yp] == '.' || ('A' <= board[xp][yp] && board[xp][yp] <= last))
			return 1;
		else
			check_corner(board, xp, yp, x, y, last);
	}
	else if(x < xp && y > yp){
		xp--;
		yp++;
		if(board[xp][yp] == '.' || ('A' <= board[xp][yp] && board[xp][yp] <= last))
			return 1;
		else
			check_corner(board, xp, yp, x, y, last);
	}
	else if(x < xp && y < yp){
		xp--;
		yp--;
		if(board[xp][yp] == '.' || ('A' <= board[xp][yp] && board[xp][yp] <= last))
			return 1;
		else
			check_corner(board, xp, yp, x, y, last);
	}
	else
		return 0;
}

void instructions(){
	printf("This game can be played by 2, 3 or 4 players.\n");
	printf("Each player has alphabet letters for their penguins at the start of game.\n");
	printf("The penguins can move on the board from square to square and collect fish from the squares they step on.\n");
	printf("When a penguin steps on a square, it would take the square for itself.\n");
	printf("Penguins can move to 4 sides of the square and can not jump across a taken square or another penguin on their way.\n");
	printf("When there is not an accessible square for the penguin to step on, it would be out.\n");
	printf("The player whould be out if all of their penguins are.\n");
	printf("10 squares have three fish, 20 squares have two and 34 have one.\n");
	printf("The winner is the person whose penguins collected the most fish.\n");
	printf("If the number of fish for more than one player was the same, the person with the most collected squares would win.\n");
	printf("If the number of squares was equal, the players with the most fish and squares would win.\n\n");
}
