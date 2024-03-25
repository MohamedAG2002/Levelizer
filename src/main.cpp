#include "level.h"

int main() {
  lvl::Level lvl = lvl::load("levels/dragon_layer.lvl");

  // Set global variables
  lvl::number_set(lvl, "level_type", 1);
  
  // Begin an entity composition and set variables for it
  lvl::entity_begin(lvl, "player");
  lvl::real_set(lvl, "pos_x", 120.0f);
  lvl::real_set(lvl, "pos_y", 325.1f);
  lvl::number_set(lvl, "health", 100);
  lvl::entity_end(lvl);

  // Save the final form of the level to the file 
  lvl::save(lvl);
}
