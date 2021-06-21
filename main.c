#include "common.h"
#include "graphics.h"
#include "player_and_map.h"
#include <math.h>

int main(){
  Player * user;
  Shop shop;
  Level * level;
  Goblin * goblins;
  int ch;

  /* set up screen and visual info */
  screenSetUp();
  colorSetUp();

  /* set up earth level and player info */
  prep_enemies(); // initialize enemy arrays
  level = mapSetUp();
  user = playerSetup(level);

  /* printf("Height %d", level->window.height);
  printf("Width %d", level->window.width); */

  /* get enemy info */
  goblins = get_goblins();

  /* main game loop */
  while((ch = getch()) != 'q', check_player_life(user)){
    // check_player_life(user);
    HUD(user);
    handleInput(ch, user);
    gob_move(goblins, level, user);
  }
  endwin();

  // free(goblins);
  free(user);
  free(level);
  return 0;
}
