#include "level.h"

struct Player {
  float x_pos, y_pos; 
  int health; 
  std::string id;
  bool is_active;
};

int main() {
  lvl::Level lvl = lvl::load("levels/dragon_layer.lvl");
  Player player = {
    .x_pos = 100.0f, 
    .y_pos = 233.12350f, 
    .health = 100, 
    .id = "Player", 
    .is_active = true,
  };

  // Set global variables
  lvl::number_set(lvl, "level_type", 1);
  
  // Begin an entity composition and set variables for it
  lvl::entity_begin(lvl, "player");
  lvl::real_set(lvl, "pos_x", player.x_pos);
  lvl::real_set(lvl, "pos_y", player.y_pos);
  lvl::number_set(lvl, "health", player.health);
  lvl::string_set(lvl, "id", player.id);
  lvl::bool_set(lvl, "is_active", player.is_active);
  lvl::entity_end(lvl);

  // Save the final form of the level to the file 
  lvl::save(lvl);

  // Retrieve variables 
  Player player2 {
    .x_pos = lvl::real_get(lvl, "player", "pos_x"),
    .y_pos = lvl::real_get(lvl, "player", "pos_y"),
    .health = lvl::number_get(lvl, "player", "health"),
    .id = lvl::string_get(lvl, "player", "id"),
    .is_active = lvl::bool_get(lvl, "player", "is_active"),
  };

  printf("\nPlayer: \n");
  printf("  position = %f, %f\n", player2.x_pos, player2.y_pos);
  printf("  health = %i\n", player2.health);
  printf("  id = %s\n", player2.id.c_str());
  printf("  is_active = %s\n", player2.is_active ? "true" : "false");
}
