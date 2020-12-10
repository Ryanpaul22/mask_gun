#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Customer
{
private:

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

	// What customer number is this sprite?
	int customerNumber;

public:

	// Handle when a mask hits a customer
	bool hit();

	// Find out if the customer is unmasked
	bool isUnMasked();

	// Spawn a new customer
	void spawn(float startX, float startY, int type, int speed);

	// Return a rectangle that is the position in the world
	FloatRect getPosition();

	// Get a copy of the sprite to draw
	Sprite getSprite();

	// get the speed of the customer
	int getSpeed();

	// set the customers speed based on the wave
	void setSpeed(int wave);

	// Update the customer each frame
	void update(float elapsedTime, Vector2f playerLocation);
};
