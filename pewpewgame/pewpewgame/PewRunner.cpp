#include "PewRunner.h"

PewRunner::PewRunner(Textures* tex) : Enemy(tex) {
	hp = 20;
	friction_x = 3.0f;
	is_jumper = true;
}