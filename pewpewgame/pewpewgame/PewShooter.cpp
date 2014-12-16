#include "PewShooter.h"
#include "Projectile.h"

PewShooter::PewShooter(Textures* tex) : Enemy(tex) {
	hp = 20;
	friction_x = 15.0f;
}