#include <cstdint>
#include "actor/swordsman.h"

namespace state {

Swordsman::Swordsman() : Actor() {}

void Swordsman::Update(int64_t delta_time) {
	position = position + velocity * delta_time;
}

}
