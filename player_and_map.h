#include "common.h"
#include "graphics.h"
#include "inventory.h"
#include "position_and_window.h"
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
} Goblin;

Level * mapSetUp();
void cave_gen(Level * level);
Player * playerSetup(Level * level);
Shop shopSetup(Level * level);
int handleInput(int input, Player * user);
int checkPos(Position newPos, Player * user);
int playerMove(Position newPos, Player * user);
bool mining(Player * user, char ore);
void HUD(Player * user);
void crafting(Player * user);
void clear_text(int y, int x, int end_x);
int random_pos(int min, int max);
Position random_dir(Position pos, Level * level);

void goblin_spawn(Position pos);
