#include "pch.h"
#include "mask.h"

using namespace sf;


// The constructor
Mask::Mask()
{
	m_MaskShape.setSize(Vector2f(8, 8));
	m_MaskShape.setFillColor(Color::Cyan);
}

void Mask::shoot(float startX, float startY,
	float targetX, float targetY)
{
	// Keep track of the bullet
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	// Calculate the gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);

	// Any gradient less than zero needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;
	}

	// Calculate the ratio between x and t
	float ratioXY = m_MaskSpeed / (1 + gradient);

	// Set the "speed" horizontally and vertically
	m_MaskDistanceY = ratioXY;
	m_MaskDistanceX = ratioXY * gradient;

	// Point the mask in the right direction
	if (targetX < startX)
	{
		m_MaskDistanceX *= -1;
	}

	if (targetY < startY)
	{
		m_MaskDistanceY *= -1;
	}

	// Finally, assign the results to the
	// member variables
	m_XTarget = targetX;
	m_YTarget = targetY;

	// Set a max range of 1000 pixels
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	// Position the bullet ready to be drawn
	m_MaskShape.setPosition(m_Position);
}

void Mask::stop()
{
	m_InFlight = false;
}

bool Mask::isInFlight()
{
	return m_InFlight;
}

FloatRect Mask::getPosition()
{
	return m_MaskShape.getGlobalBounds();
}

RectangleShape Mask::getShape()
{
	return m_MaskShape;
}

void Mask::update(float elapsedTime)
{
	// Update the mask position variables
	m_Position.x += m_MaskDistanceX * elapsedTime;
	m_Position.y += m_MaskDistanceY * elapsedTime;

	// Move the mask
	m_MaskShape.setPosition(m_Position);

	// Has the mask gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}

}