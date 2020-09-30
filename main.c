#include "common.h"
#include "graphics.h"
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
int colorSetUp();
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
  colorSetUp();
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

int colorSetUp() {
  start_color();
  init_pair(EARTH_PAIR, COLOR_WHITE, COLOR_YELLOW);
  init_pair(ORE_PAIR, COLOR_WHITE, COLOR_MAGENTA);
  init_pair(GRASS_PAIR, COLOR_WHITE, COLOR_GREEN);
  init_pair(PLAYER_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(ELEVATOR_PAIR, COLOR_WHITE, COLOR_CYAN);
  init_pair(TRAIL_PAIR, COLOR_WHITE, COLOR_BLACK);
  // clear();
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
  int r;

  // attron(COLOR_PAIR(EARTH_PAIR));
  for(i = newLevel->window.height; i > newLevel->window.height/5; i--){
    r = rand() % (newLevel->window.width - 1);
    for(j = 0; j < newLevel->window.width; j++){
      if(j == newLevel->window.width - 1){ // elevator at end of screen
        attron(COLOR_PAIR(ELEVATOR_PAIR));
        mvprintw(i, j, "#");
        attroff(COLOR_PAIR(ELEVATOR_PAIR));
      }
      else if(i == first_layer){ // surface
        attron(COLOR_PAIR(GRASS_PAIR));
        mvprintw(i, j, "^");
        attron(COLOR_PAIR(GRASS_PAIR));
      }
      else if(i == first_layer + 1){ //layer just below surface
        attron(COLOR_PAIR(EARTH_PAIR));
        mvprintw(i, j, "*");
        attroff(COLOR_PAIR(EARTH_PAIR));
      }
      else if(j == r) { // ores
        attron(COLOR_PAIR(ORE_PAIR));
        mvprintw(i, j, "?");
        attroff(COLOR_PAIR(ORE_PAIR));
      }
      else {  // everything else
        attron(COLOR_PAIR(EARTH_PAIR));
        mvprintw(i, j, "x");
        attroff(COLOR_PAIR(EARTH_PAIR));
      }
    }
  }
  // attroff(COLOR_PAIR(EARTH_PAIR));

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
  if(user->position.y == user->window.height/5 + 1){
    attron(COLOR_PAIR(GRASS_PAIR));
    mvprintw(user->position.y, user->position.x, "^");
    attroff(COLOR_PAIR(GRASS_PAIR));
  }
  else if(user->position.x == user->window.width - 1){
    attron(COLOR_PAIR(ELEVATOR_PAIR));
    mvprintw(user->position.y, user->position.x, "#");
    attroff(COLOR_PAIR(ELEVATOR_PAIR));
  }
  else{
    attron(COLOR_PAIR(TRAIL_PAIR));
    mvprintw(user->position.y, user->position.x, ".");
    attroff(COLOR_PAIR(TRAIL_PAIR));
  }

  user->position.y = y;
  user->position.x = x;

  attron(COLOR_PAIR(PLAYER_PAIR));
  mvprintw(user->position.y, user->position.x, "@");
  attroff(COLOR_PAIR(PLAYER_PAIR));
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
  /* have to bitmask the char to 'remove' the color attribute
    for condition checking */
  int testch = mvinch(newY, newX);
  switch(testch & A_CHARTEXT){
    case TRAIL:
    case GRASS:
    case ELEVATOR:
    case EARTH:
      playerMove(newY, newX, user);
      break;
    default:
      move(user->position.y, user->position.x);
      break;
  }
}
