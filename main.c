#include "common.h"
#include "graphics/graphics.h"
#include "player_map_info/player_and_map.h"
#include <math.h>

int main(){
  Player * user;
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
  while((ch = getch()) != 'q'){
    if(user->health <= 0){
      game_over(level);
    }
    else{
      HUD(user);
      handleInput(ch, user);
      gob_move(goblins, level, user);
    }
  }
  endwin();

  // free(goblins);
  free(user);
  free(level);
  return 0;
}
