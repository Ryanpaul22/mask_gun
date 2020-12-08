#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Mask
{
private:
	// Where is the mask?
	Vector2f m_Position;

	// What each mask looks like
	RectangleShape m_MaskShape;

	// Is this mask currently whizzing through the air
	bool m_InFlight = false;

	// How fast does a mask travel?
	float m_MaskSpeed = 1000;

	// What fraction of 1 pixel does the mask travel, 
	// Horizontally and vertically each frame?
	// These values will be derived from m_MaskSpeed
	float m_MaskDistanceX;
	float m_MaskDistanceY;

	// Where is this mask headed to?
	float m_XTarget;
	float m_YTarget;

	// Some boundaries so the mask doesn't fly forever
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

	// Public function prototypes go here
public:
	// The constructor
	Mask();

	// Stop the bullet
	void stop();

	// Returns the value of m_InFlight
	bool isInFlight();

	// Launch a new bullet
	void shoot(float startX, float startY,
		float xTarget, float yTarget);

	// Tell the calling code where the bullet is in the world
	FloatRect getPosition();

	// Return the actual shape (for drawing)
	RectangleShape getShape();

	// Update the bullet each frame
	void update(float elapsedTime);

};
