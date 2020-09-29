#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct Window {
  int width;
  int height;
} Window;

typedef struct Position {
  int x;
  int y;
  // TILE_TYPE tile;
} Position;

typedef struct Player{
  Position position;
  Window window;
  int health;
  // Room * room;
} Player;

typedef struct Level{
  Position position;
  Window window;
  // Monster ** monsters;
  // Item ** items;
} Level;

int screenSetUp();
Level * mapSetUp();
Player * playerSetup(Level * level);
int playerMove(int y, int x, Player * user);
int handleInput(int input, Player * user);
int checkPos(int newY, int newX, Player * user);

int main(){
  Player * user;
  Level * level;
  int ch;

  screenSetUp();
  level = mapSetUp();

  user = playerSetup(level);

  /* main game loop */
  while((ch = getch()) != 'q'){
    handleInput(ch, user);
  }
  endwin();

  free(user);
  free(level);
  return 0;
}

int screenSetUp(){
  initscr();
  printw("Hello world!");
  noecho();
  refresh();

  srand(time(NULL));

  return 0;
}

Level * mapSetUp(){
  Level * newLevel;
  newLevel = malloc(sizeof(Level));

  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  newLevel->window.height = size.ws_row;
  newLevel->window.width = size.ws_col;

  //printf("%d %d\n", newLevel->window.height, newLevel->window.width);
  int i;
  int j;
  int first_layer = newLevel->window.height/5 + 1;
  for(i = newLevel->window.height; i > newLevel->window.height/5; i--){
    for(j = 0; j < newLevel->window.width; j++){
      if(j == newLevel->window.width - 1) // tunnel at end of screen
        mvprintw(i, j, "#");
      else if(i == first_layer) // surface
        mvprintw(i, j, "^");
      else  // everything else
        mvprintw(i, j, ".");
    }
  }

  return newLevel;
}

Player * playerSetup(Level * level){
  Player * newPlayer;
  newPlayer = malloc(sizeof(Player));

  newPlayer->window.width = level->window.width;
  newPlayer->window.height = level->window.height;
  newPlayer->position.x = newPlayer->window.width/2;
  newPlayer->position.y = newPlayer->window.height/5 + 1;
  newPlayer->health = 20;

  playerMove(newPlayer->position.y, newPlayer->position.x, newPlayer);

  return newPlayer;
}

int playerMove(int y, int x, Player * user){
  if(user->position.y == user->window.height/5 + 1)
    mvprintw(user->position.y, user->position.x, "^");
  else if(user->position.x == user->window.width - 1)
    mvprintw(user->position.y, user->position.x, "#");
  else
    mvprintw(user->position.y, user->position.x, ".");

  user->position.y = y;
  user->position.x = x;

  mvprintw(user->position.y, user->position.x, "@");
  move(user->position.y, user->position.x);
}

int handleInput(int input, Player * user){
  int newY;
  int newX;

  switch (input) {
    case 'w':
      newY = user->position.y - 1;
      newX = user->position.x;
      break;
    case 'a':
      newY = user->position.y;
      newX = user->position.x - 1;
      break;
    case 's':
      newY = user->position.y + 1;
      newX = user->position.x;
      break;
    case 'd':
      newY = user->position.y;
      newX = user->position.x + 1;
      break;
    default: break;
  }

  checkPos(newY, newX, user);
}

int checkPos(int newY, int newX, Player * user){
  int space;
  switch(mvinch(newY, newX)){
    case '.':
    case '^':
    case '#':
      playerMove(newY, newX, user);
      break;
    default:
      move(user->position.y, user->position.x);
      break;
  }
}
