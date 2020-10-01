#include "player_and_map.h"

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

  /* initialize window info and player position */
  newPlayer->window.width = level->window.width;
  newPlayer->window.height = level->window.height;
  newPlayer->position.x = newPlayer->window.width/2;
  newPlayer->position.y = newPlayer->window.height/5 + 1;

  /* initialize health and inventory */
  newPlayer->health = 20;
  newPlayer->inventory[0] = 0;
  newPlayer->inventory[1] = 0;

  playerMove(newPlayer->position.y, newPlayer->position.x, newPlayer);

  return newPlayer;
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
