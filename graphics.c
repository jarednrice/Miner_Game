#include "graphics.h"
#include <time.h>

int screenSetUp(){
  initscr();
  //printw("Hello world!");
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
  init_pair(SHOP_PAIR, COLOR_WHITE, COLOR_RED);
  // clear();
}
