#include "common.h"
#include "graphics.h"
#include "player_and_map.h"
#include <math.h>

int main(){
  Player * user;
  Shop shop;
  Level * level;
  int ch;

  /* set up screen and visual info */
  screenSetUp();
  colorSetUp();

  /* set up earth level and player info */
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
