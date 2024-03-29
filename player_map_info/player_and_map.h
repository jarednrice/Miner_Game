#include "../common.h"
#include "../graphics/graphics.h"
#include "inventory.h"
#include "../graphics/position_and_window.h"
#include <string.h>

typedef struct Shop{
  Position position_start;
  Position position_end;
} Shop;

typedef struct Level{
  Position position;
  Window window;
  int surface;
  /* buildings */
  Shop shop;
  // Monster ** monsters;
  // Item ** items;
} Level;

typedef struct Player{
  Position position;
  Window window;
  int health;
  int money;
  int inventory[4];
  /* Player needs to "know"
  where buildings are to
  use them etc*/
  Shop shop;
  // Room * room;
} Player;

typedef struct Goblin{
  Position position;
  int health;
  int money;
  bool first_flag;
  struct Goblin * next;
} Goblin;

Level * mapSetUp();
void cave_gen(Level * level);
Player * playerSetup(Level * level);
Shop shopSetup(Level * level);
void handleInput(int input, Player * user);
void checkPos(Position newPos, Player * user);
void playerMove(Position newPos, Player * user);
bool mining(Player * user, char ore);
void HUD(Player * user);
void crafting(Player * user);
void clear_text(int y, int x, int end_x);
int random_pos(int min, int max);
Position random_dir(Position pos, Level * level);

void prep_enemies();
void goblin_spawn(Position pos, Goblin * head);
Goblin * get_goblins();
void gob_move(Goblin * gobs, Level * level, Player * user);
void combat(Goblin * gob, Player * user);
void kill_gob(Goblin * gob);
void turn_pause(unsigned int secs);

void game_over(Level * level);
