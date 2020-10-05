#include "common.h"
#include "graphics.h"
#include "inventory.h"

typedef struct Window {
  int width;
  int height;
} Window;

typedef struct Position {
  int x;
  int y;
  // TILE_TYPE tile;
} Position;

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
  int inventory[3];
  /* Player needs to "know"
  where buildings are to
  use them etc*/
  Shop shop;
  // Room * room;
} Player;

Level * mapSetUp();
Player * playerSetup(Level * level);
Shop shopSetup(Level * level);
int handleInput(int input, Player * user);
int checkPos(Position newPos, Player * user);
int playerMove(Position newPos, Player * user);
bool mining(Player * user, char ore);
