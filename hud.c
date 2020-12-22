#include "hud.h"
#include "player_and_map.h"

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
  
  iron_string = (char*) realloc(iron_string, (strlen(iron_string) + strlen(num) + 2));
  strcat(iron_string, num); 
  mvprintw(3, 0, iron_string);

  /* silver */
  char * silver_string = (char*) malloc(9); 
  strcpy(silver_string, "Silver: ");

  length = snprintf(NULL, 0, "%d",  user->inventory[SILVER_SLOT]);  
  snprintf(num, length + 1, "%d",  user->inventory[SILVER_SLOT]);
  
  silver_string = (char*) realloc(silver_string, (strlen(silver_string) + strlen(num) + 2));
  strcat(silver_string, num); 
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
