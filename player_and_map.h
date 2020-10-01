#include "common.h"
#include "graphics.h"

typedef struct Window {
  int width;
  int height;
} Window;

typedef struct Position {
  int x;
  int y;
  // TILE_TYPE tile;
} Position;

typedef struct Level{
  Position position;
  Window window;
  // Monster ** monsters;
  // Item ** items;
} Level;

typedef struct Player{
  Position position;
  Window window;
  int health;
  int inventory[2];
  // Room * room;
} Player;

Level * mapSetUp();
Player * playerSetup(Level * level);
int playerMove(int y, int x, Player * user);
int handleInput(int input, Player * user);
int checkPos(int newY, int newX, Player * user);
