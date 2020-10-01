#include "common.h"
#include "graphics.h"
#include "player_and_map.h"
#include <math.h>

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
