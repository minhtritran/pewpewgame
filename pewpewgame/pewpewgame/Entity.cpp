#include "Entity.h"

Entity::Entity() {
	x = 0.0f;
	y = 0.0f;
	width = 0.2f;
	height = 0.2f;
	rotation = 0.0f;
	scale_x = 1.0f;
	scale_y = 1.0f;

	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f;
	friction_x = 0.0f;
	friction_y = 0.0f;
	mass = 0.0f;
	isStatic = false;
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;

	sprite_in_use = SPRITE;
}

Entity::~Entity() {}

void Entity::BuildMatrix()
{
	Matrix translateMatrix;
	Matrix rotateMatrix;
	Matrix scaleMatrix;

	translateMatrix.identity();
	translateMatrix.m[3][0] = x;
	translateMatrix.m[3][1] = y;

	rotateMatrix.identity();
	rotateMatrix.m[0][0] = cos(rotation);
	rotateMatrix.m[1][0] = -1.0f * sin(rotation);
	rotateMatrix.m[0][1] = sin(rotation);
	rotateMatrix.m[1][1] = cos(rotation);

	scaleMatrix.identity();
	scaleMatrix.m[0][0] = scale_x;
	scaleMatrix.m[1][1] = scale_y;

	matrix = scaleMatrix * rotateMatrix * translateMatrix;
}

void Entity::setScale(float scale) {
	width = scale * sprite.spriteWidth;
	height = scale * sprite.spriteHeight;
}

void Entity::Update(float elapsed) {

}

void Entity::FixedUpdate() {
	if (!isStatic) {
		if (collidedBottom) {
			velocity_y = 0.0f;
		}
		if (collidedTop)
			velocity_y = 0.0f;
		if (collidedLeft)
			velocity_x = 0.0f;
		if (collidedRight)
			velocity_x = 0.0f;

		collidedBottom = false;
		collidedTop = false;
		collidedLeft = false;
		collidedRight = false;
	}

	velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
	velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);

	velocity_x += acceleration_x * FIXED_TIMESTEP;
	velocity_y += acceleration_y * FIXED_TIMESTEP;
}

void Entity::Render(float elapsed) {
	BuildMatrix();
	if (sprite_in_use == SPRITE)
		sprite.Draw(width, height, matrix, elapsed);
}

bool Entity::collidesWith(Entity* entity) {
	float e1_y_max = y + height / 2;
	float e1_y_min = y - height / 2;
	float e1_x_max = x + width / 2;
	float e1_x_min = x - width / 2;

	float e2_y_max = entity->y + entity->height / 2;
	float e2_y_min = entity->y - entity->height / 2;
	float e2_x_max = entity->x + entity->width / 2;
	float e2_x_min = entity->x - entity->width / 2;

	if (e1_y_min > e2_y_max)
		return false;
	if (e1_y_max < e2_y_min)
		return false;
	if (e1_x_min > e2_x_max)
		return false;
	if (e1_x_max < e2_x_min)
		return false;
		
	return true;
}

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}