#include "entity.h"

#include <unordered_map>

namespace lvl {

void entity_destroy(Entity& entity) {
  entity.numbers.clear();
  entity.reals.clear();
  entity.bools.clear();
  entity.strings.clear();
}

}
