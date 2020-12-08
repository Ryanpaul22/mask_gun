#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Customer
{
private:
	// How fast is each customer type?
	const float ADULT_SPEED = 40;
	const float TEEN_SPEED = 80;
	const float BABY_SPEED = 20;

	// How tough is each customer type
	const float ADULT_HEALTH = 5;
	const float TEEN_HEALTH = 1;
	const float BABY_HEALTH = 3;

	// Make each customer vary its speed slightly
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	// Where is this customer?
	Vector2f m_Position;

	// A sprite for the customer
	Sprite m_Sprite;

	// How fast can this one run/crawl?
	float m_Speed;

	// How much health has it got?
	float m_Health;

	// Is it still unMasked?
	bool m_unMasked;

public:

	// Handle when a mask hits a customer
	bool hit();

	// Find out if the customer is unmasked
	bool isUnMasked();

	// Spawn a new zombie
	void spawn(float startX, float startY, int type, int seed);

	// Return a rectangle that is the position in the world
	FloatRect getPosition();

	// Get a copy of the sprite to draw
	Sprite getSprite();

	// Update the zombie each frame
	void update(float elapsedTime, Vector2f playerLocation);
};
