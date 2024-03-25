#pragma once

#include <string>
#include <unordered_map>

namespace lvl {

struct Entity {
  std::string name;   

  std::unordered_map<std::string, float> reals;
  std::unordered_map<std::string, int> numbers;
  std::unordered_map<std::string, bool> bools;
  std::unordered_map<std::string, std::string> strings;
};

void entity_destroy(Entity& entity);

}
