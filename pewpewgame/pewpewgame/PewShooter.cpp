#include "PewShooter.h"
#include "Projectile.h"

PewShooter::PewShooter(Textures* tex) : Enemy(tex) {
	hp = 25;
	friction_x = 15.0f;
}