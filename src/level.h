#pragma once

#include "entity.h"

#include <string>
#include <unordered_map>

namespace lvl {

struct Level {
  std::string name, path, src, current_entity;
  std::unordered_map<std::string, Entity> entities;
};

// TODO: Maybe make every level heap-allocated instead of passing around a copy?
Level load(const std::string& path);
void unload(Level& level);
bool save(Level& level);

void entity_begin(Level& level, const std::string& name, const bool can_create = true);
void entity_end(Level& level);

void number_set(Level& level, const std::string& name, const int value, const bool can_create = true);
void real_set(Level& level, const std::string& name, const float value, const bool can_create = true);
void bool_set(Level& level, const std::string& name, const bool value, const bool can_create = true);
void string_set(Level& level, const std::string& name, const std::string& value, const bool can_create = true);

const int& number_get(Level& level, const std::string& entt, const std::string& name);
const float& real_get(Level& level, const std::string& entt, const std::string& name);
const bool& bool_get(Level& level, const std::string& entt, const std::string& name);
const std::string& string_get(Level& level, const std::string& entt, const std::string& name);

}
