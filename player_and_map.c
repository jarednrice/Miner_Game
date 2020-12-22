#include "player_and_map.h"

Level * mapSetUp(){
  Level * newLevel;
  newLevel = malloc(sizeof(Level));

  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

  /* size of terminal */
  newLevel->window.height = size.ws_row;
  newLevel->window.width = size.ws_col;

  /* set surface layer value */
  newLevel->surface = newLevel->window.height/4 + 1;

  /* info at top */
  int x;
  for(x = 0; x < newLevel->window.width; x++){
    mvprintw(2, x, "-");
  }

  /* create and draw the buildings */
  newLevel->shop = shopSetup(newLevel);

  int i;
  int j;
  int r;
  int r_2;

  /* Loop to draw map. A bit hacked together */
  for(i = newLevel->window.height; i > newLevel->surface - 1; i--){
    r = rand() % (newLevel->window.width - 1);
    r_2 = rand() % (newLevel->window.width - 1);
    for(j = 0; j < newLevel->window.width; j++){
      if(j == newLevel->window.width - 1){ // elevator at end of screen
        attron(COLOR_PAIR(ELEVATOR_PAIR));
        mvprintw(i, j, "#");
        attroff(COLOR_PAIR(ELEVATOR_PAIR));
      }
      else if(i == newLevel->surface){ // surface
        if((j > newLevel->shop.position_start.x - 1) &&
          (j < newLevel->shop.position_end.x + 1)){ // location of shop floor
            continue;
        }
        else {
          attron(COLOR_PAIR(GRASS_PAIR));
          mvprintw(i, j, "^");
          attron(COLOR_PAIR(GRASS_PAIR));
        }
      }
      else if(i == newLevel->surface + 1){ //layer just below surface
        attron(COLOR_PAIR(EARTH_PAIR));
        mvprintw(i, j, "*");
        attroff(COLOR_PAIR(EARTH_PAIR));
      }
      else if(j == r) { // ores
        attron(COLOR_PAIR(ORE_PAIR));
        mvprintw(i, j, "?");
        attroff(COLOR_PAIR(ORE_PAIR));
      }
      else if(j == r_2) { // ores
        attron(COLOR_PAIR(ORE_PAIR));
        mvprintw(i, j, "S");
        attroff(COLOR_PAIR(ORE_PAIR));
      }
      else {  // everything else
        attron(COLOR_PAIR(EARTH_PAIR));
        mvprintw(i, j, "x");
        attroff(COLOR_PAIR(EARTH_PAIR));
      }
    }
  }

  /* generate the tunnels and caves */
  cave_gen(newLevel);

  return newLevel;
}

void cave_gen(Level * level){
  /* get random point */
  Position random;
  random.x = random_pos(0, level->window.width - 2); 
  random.y = random_pos(level->surface + 2, level->window.height - 1);
  /* drop point */
  attron(COLOR_PAIR(TRAIL_PAIR));
  mvprintw(random.y, random.x, ".");
  attroff(COLOR_PAIR(TRAIL_PAIR));

  /* loop until path tile count is reached */
  int path_count = 1;
  int total = (((level->window.height - level->surface) * level->window.width - 1) - 1) / 5;  
  while(path_count <= total){
    /* get point in random dir from from */
    random = random_dir(random, level);
    
    /* check if block isn't wall */
    int testch = mvinch(random.y, random.x);
    if((testch & A_CHARTEXT) == '.')
      continue; 
   
    /* drop point and increment */
    /* random chance that point is treasure */
    int t = rand() % 100;
    if(t == 1){
      attron(COLOR_PAIR(TREASURE_PAIR));
      mvprintw(random.y, random.x, "T");
      attroff(COLOR_PAIR(TREASURE_PAIR));
    }
    else if(t == 2){
      /*attron(COLOR_PAIR(ENEMY_PAIR));
      mvprintw(random.y, random.x, "&");
      attroff(COLOR_PAIR(ENEMY_PAIR));*/
      goblin_spawn(random); 
    }
    else {   
      attron(COLOR_PAIR(TRAIL_PAIR));
      mvprintw(random.y, random.x, ".");
      attroff(COLOR_PAIR(TRAIL_PAIR));
    }
    path_count += 1;
  }
}

int random_pos(int min, int max){
  return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

Position random_dir(Position pos, Level * level){
  Position new_pos;

  /* four possibilities */
  /* NSEW */
  int rand_dir = rand() % 4;
  // printf("%d", rand_dir);

  int new_y;
  int new_x;
  /* each num assigned to dir */
  switch(rand_dir){
    case 0: // north
      new_y = pos.y - 1;
      new_x = pos.x;
      break;
    case 1: // south
      new_y = pos.y + 1;
      new_x = pos.x;
      break;
    case 2: // east
      new_x = pos.x + 1;
      new_y = pos.y;
      break;
    case 3: // west
      new_x = pos.x - 1;
      new_y = pos.y;
      break;
  }

  /* check borders */ 
  if(new_x <= 0)
    new_x++;
  else if(new_x >= level->window.width - 1)
    new_x--; 

  if(new_y >= level->window.height - 1)
    new_y--;
  else if(new_y <= level-> surface + 1)
    new_y++;

  new_pos.x = new_x;
  new_pos.y = new_y;

  return new_pos;
}

Shop shopSetup(Level * level){
  // Can I return stuff that isn't malloced lmao
  Shop shop;
  shop.position_start.y = level->surface - 1;
  shop.position_start.x = 2 * level->window.width/3;
  shop.position_end.x = 2 * level->window.width/3 + 8;
  int y = shop.position_start.y;
  int x = shop.position_start.x;

  attron(COLOR_PAIR(SHOP_PAIR));
  /* walls */
  mvprintw(y, x, "|");
  mvprintw(y - 1, x, "|");
  mvprintw(y - 1, x, "|");
  mvprintw(y, x + 8, "|");
  mvprintw(y - 1, x + 8, "|");
  /* roof and floor */
  mvprintw(y - 2, x, "=========");
  mvprintw(y + 1, x, "_________");
  /* logo */
  mvprintw(y , x + 4, "$");
  attroff(COLOR_PAIR(SHOP_PAIR));

  return shop;
}

Player * playerSetup(Level * level){
  Player * newPlayer;
  newPlayer = malloc(sizeof(Player));

  /* initialize window info and player position */
  /* spawn is 3/4 point on calculated width */
  /* surface location is window.height/4 + 1 */
  newPlayer->window.width = level->window.width;
  newPlayer->window.height = level->window.height;
  newPlayer->position.x = newPlayer->window.width * 0.75;
  newPlayer->position.y = newPlayer->window.height/4 + 1;

  /* initialize building info that player must "know" */
  newPlayer->shop = level->shop;

  /* initialize health, money, and inventory */
  newPlayer->health = 20;
  newPlayer->money = 0;
  newPlayer->inventory[PICK_SLOT] = COP_PICK;  // start with copper pick axe
  newPlayer->inventory[WEAPON_SLOT] = 0;  // weapon
  newPlayer->inventory[KEY_SLOT] = 0;
  newPlayer->inventory[IRON_SLOT] = 0;  // ore starting with iron
  newPlayer->inventory[SILVER_SLOT] = 0;  

  playerMove(newPlayer->position, newPlayer);

  return newPlayer;
}

int handleInput(int input, Player * user){
  Position newPos;

  switch (input) {
    case 'w':
      newPos.y = user->position.y - 1;
      newPos.x = user->position.x;
      break;
    case 'a':
      newPos.y = user->position.y;
      newPos.x = user->position.x - 1;
      break;
    case 's':
      newPos.y = user->position.y + 1;
      newPos.x = user->position.x;
      break;
    case 'd':
      newPos.y = user->position.y;
      newPos.x = user->position.x + 1;
      break;
    default: break;
  }

  checkPos(newPos, user);
}

int checkPos(Position newPos, Player * user){
  /* save old postions for functions */
  Position oldPos;
  oldPos.y = user->position.y;
  oldPos.x = user->position.x;

  /* have to bitmask the char to 'remove' the color attribute
    for condition checking */
  int testch = mvinch(newPos.y, newPos.x);
  switch(testch & A_CHARTEXT){
    case TRAIL:
    case GRASS:
    case ELEVATOR:
    case EARTH:
    case FLOOR:
      playerMove(newPos, user);
      break;
    case IRON:
      if(mining(user, IRON))
        playerMove(newPos, user);
      else
        move(oldPos.y, oldPos.x);
      break;
    case SILVER:
      if(mining(user, SILVER))
        playerMove(newPos, user);
      else
        move(oldPos.y, oldPos.x);
      break;
    default:
      move(oldPos.y, oldPos.x);
      break;
  }
}

int playerMove(Position newPos, Player * user){
  Position oldPos;
  oldPos.y = user->position.y;
  oldPos.x = user->position.x;

  /* Dont' want all blocks to be replaced by '.' */
  if(oldPos.y == user->window.height/4 + 1){
    if((oldPos.x > user->shop.position_start.x - 1) &&
      (oldPos.x < user->shop.position_end.x + 1)){
      attron(COLOR_PAIR(SHOP_PAIR));
      mvprintw(oldPos.y, oldPos.x, "_");
      attroff(COLOR_PAIR(SHOP_PAIR));
      /* Player is in shop */
      crafting(user);
    }
    else {
      attron(COLOR_PAIR(GRASS_PAIR));
      mvprintw(oldPos.y, oldPos.x, "^");
      attroff(COLOR_PAIR(GRASS_PAIR));
    }
  }
  else if(oldPos.x == user->window.width - 1){
    attron(COLOR_PAIR(ELEVATOR_PAIR));
    mvprintw(oldPos.y, oldPos.x, "#");
    attroff(COLOR_PAIR(ELEVATOR_PAIR));
  }
  else{
    attron(COLOR_PAIR(TRAIL_PAIR));
    mvprintw(oldPos.y, oldPos.x, ".");
    attroff(COLOR_PAIR(TRAIL_PAIR));
  }

  user->position.y = newPos.y;
  user->position.x = newPos.x;

  attron(COLOR_PAIR(PLAYER_PAIR));
  mvprintw(user->position.y, user->position.x, "@");
  attroff(COLOR_PAIR(PLAYER_PAIR));
  move(user->position.y, user->position.x);
}

bool mining(Player * user, char ore){
  int j = user->inventory[PICK_SLOT]; // what type of pick axe does the player have?
  /* clear extra chars for display message */
  clear_text(1, 0, 15);
  
  if(ore == IRON && j >= COP_PICK){
    int i = user->inventory[IRON_SLOT];
    user->inventory[IRON_SLOT] = ++i;
    HUD(user);
    clear_text(1, 0, 30);
    mvprintw(1, 0, "Got iron ore!");
    return true;
  }
  else if(ore == SILVER && j >= IRON_PICK){
    int i = user->inventory[SILVER_SLOT];
    user->inventory[SILVER_SLOT] = ++i;
    HUD(user);
    clear_text(1, 0, 30);
    mvprintw(1, 0, "Got silver ore!");
    return true;
  }
  else
    return false;
}

void HUD(Player * user){
  /* ores */
  
  /* iron */
  /* allocate mem for strings */
  /* set init data */
  char * iron_string = (char*) malloc(7); 
  strcpy(iron_string, "Iron: ");

  int length = snprintf(NULL, 0, "%d",  user->inventory[IRON_SLOT]);  
  char * num = (char*) malloc (length + 1);
  snprintf(num, length + 1, "%d",  user->inventory[IRON_SLOT]);
  
  int final_length = strlen(iron_string) + strlen(num);
  iron_string = (char*) realloc(iron_string, final_length + 2);
  strcat(iron_string, num);
  clear_text(3, 0, final_length + 1); 
  mvprintw(3, 0, iron_string);

  /* silver */
  char * silver_string = (char*) malloc(9); 
  strcpy(silver_string, "Silver: ");

  length = snprintf(NULL, 0, "%d",  user->inventory[SILVER_SLOT]);  
  snprintf(num, length + 1, "%d",  user->inventory[SILVER_SLOT]);
  
  final_length = strlen(silver_string) + strlen(num);
  silver_string = (char*) realloc(silver_string, final_length + 2);
  strcat(silver_string, num); 
  clear_text(4, 0, final_length + 1); 
  mvprintw(4, 0, silver_string);

  /* pickaxe */
  char * pick_string = (char*) malloc(11);
  strcpy(pick_string, "Pick axe: ");
  
  int pick = user->inventory[PICK_SLOT];
  switch(pick){
    case COP_PICK:
      pick_string = (char*) realloc(pick_string, (strlen(pick_string) + 8));
      strcat(pick_string, "Copper");
      mvprintw(3, 10, pick_string);
      break; 
    case IRON_PICK:
      /* clear extra chars */
      clear_text(3, 10, 30);
      pick_string = (char*) realloc(pick_string, (strlen(pick_string) + 6));
      strcat(pick_string, "Iron");
      mvprintw(3, 10, pick_string);
      break; 
    case SILVER_PICK:
      /* clear extra chars */
      clear_text(3, 10, 30);
      pick_string = (char*) realloc(pick_string, (strlen(pick_string) + 8));
      strcat(pick_string, "Silver");
      mvprintw(3, 10, pick_string);
      break; 
  } 

  /* weapon */
  char * weap_string = (char*) malloc(9);
  strcpy(weap_string, "Weapon: ");

  int weapon = user->inventory[WEAPON_SLOT];
  switch(weapon){
    case IRON_SWORD:
      clear_text(3, 29, 100);
      weap_string = (char*) realloc(weap_string, (strlen(weap_string) + 12));
      strcat(weap_string, "Iron Sword"); 
      mvprintw(3, 29, weap_string);
      break;
    case SILVER_SWORD:
      clear_text(3, 29, 100);
      weap_string = (char*) realloc(weap_string, (strlen(weap_string) + 14));
      strcat(weap_string, "Silver Sword"); 
      mvprintw(3, 29, weap_string);
      break;
    default:
      weap_string = (char*) realloc(weap_string, (strlen(weap_string) + 12)); 
      strcat(weap_string, "No weapon");
      mvprintw(3, 29, weap_string); 
  }
  
  free(iron_string);
  free(silver_string);
  free(num);
  free(pick_string);
  free(weap_string);
}

void crafting(Player * user){
   mvprintw(5, 0, "Craft iron pickaxe (1)");
   mvprintw(5, 25, "Craft iron sword (2)"); 
   mvprintw(6, 0, "Craft silver pickaxe (3)"); 
   mvprintw(6, 27, "Craft silver sword (4)");
   
   int ch = getch();
   switch(ch){
     case '1':
       if(user->inventory[IRON_SLOT] >= 3){
         user->inventory[IRON_SLOT] -= 3;
         user->inventory[PICK_SLOT] = IRON_PICK;
         HUD(user);
       }
       else {
	 clear_text(1, 0, 30);
         mvprintw(1, 0, "Need more iron!");
       }
       break;
     case '2':
       if(user->inventory[IRON_SLOT] >= 3){
         user->inventory[IRON_SLOT] -= 3;
         user->inventory[WEAPON_SLOT] = IRON_SWORD;
         HUD(user);
       }
       else {
	 clear_text(1, 0, 30);
         mvprintw(1, 0, "Need more iron!");
       }
       break;
     case '3':
       if(user->inventory[SILVER_SLOT] >= 3){
         user->inventory[SILVER_SLOT] -= 3;
         user->inventory[PICK_SLOT] = SILVER_PICK;
         HUD(user);
       }
       else {
	 clear_text(1, 0, 30);
         mvprintw(1, 0, "Need more silver!");
       }
       break;
     case '4': 
       if(user->inventory[SILVER_SLOT] >= 3){
         user->inventory[SILVER_SLOT] -= 3;
         user->inventory[WEAPON_SLOT] = SILVER_SWORD;
         HUD(user);
       }
       else {
	 clear_text(1, 0, 30);
         mvprintw(1, 0, "Need more silver!");
       }
       break;
   } 
   
   clear_text(5, 0, 60);
   clear_text(6, 0, 60);
}

void clear_text(int y, int x, int end_x){
  int i = x;
  for(i; i < end_x; i++){
    if(mvinch(y, i))
      mvprintw(y, i, " ");
  }
}

/* HANDLE ENEMIES */

/* need to make like an array of enemies or something */

void goblin_spawn(Position pos){
  Goblin * new_goblin;
  new_goblin = malloc(sizeof(Goblin));

  new_goblin->health = 10;
  new_goblin->money = 5;

  new_goblin->position.x = pos.x;
  new_goblin->position.y = pos.y;
  
  attron(COLOR_PAIR(ENEMY_PAIR));
  mvprintw(pos.y, pos.x, "&");
  attroff(COLOR_PAIR(ENEMY_PAIR)); 
}
