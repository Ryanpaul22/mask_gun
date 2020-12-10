#include "pch.h"
#include "customer.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;


void Customer::spawn(float startX, float startY, int type, int seed)
{

	switch (type)
	{
	case 0:
		// Customer 1
		customerNumber = 1;
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/customer_1.png"));

		m_Speed = 10;
		m_Health = .1;
		break;

	case 1:
		// Customer 2
		customerNumber = 2;
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/customer_2.png"));

		m_Speed = 30;
		m_Health = .1;
		break;

	case 2:
		// Customer 3
		customerNumber = 3;
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/customer_3.png"));

		m_Speed = 50;
		m_Health = .1;
		break;

	case 3:
		// Customer 4
		customerNumber = 4;
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/customer_4.png"));

		m_Speed = 70;
		m_Health = .1;
		break;

	case 4:
		// Customer 5
		customerNumber = 5;
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/customer_5.png"));

		m_Speed = 70;
		m_Health = .1;
		break;
	}

	// Modify the speed to make the customer unique
	// Every customer is unique. Create a seed modifier
	srand((int)time(0) * seed);
	// Somewhere between 80 an 100
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;
	// Express as a fraction of 1
	modifier /= 100; // Now equals between .7 and 1
	m_Speed *= modifier;

	m_Position.x = startX;
	m_Position.y = startY;

	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
}

bool Customer::hit()
{
	m_Health--;

	if (m_Health < 0)
	{
		// unmasked
		m_unMasked = false;

		if (customerNumber == 1)
		{
			m_Sprite.setTexture(TextureHolder::GetTexture(
				"graphics/customer_1_masked.png"));
		}
		if (customerNumber == 2)
		{
			m_Sprite.setTexture(TextureHolder::GetTexture(
				"graphics/customer_2_masked.png"));
		}
		if (customerNumber == 3)
		{
			m_Sprite.setTexture(TextureHolder::GetTexture(
				"graphics/customer_3_masked.png"));
		}
		if (customerNumber == 4)
		{
			m_Sprite.setTexture(TextureHolder::GetTexture(
				"graphics/customer_4_masked.png"));
		}
		if (customerNumber == 5)
		{
			m_Sprite.setTexture(TextureHolder::GetTexture(
				"graphics/customer_5_masked.png"));
		}

		return true;
	}

	// hit, but not masked yet
	return false;
}

bool Customer::isUnMasked()
{
	return m_unMasked;
}

FloatRect Customer::getPosition()
{
	return m_Sprite.getGlobalBounds();
}


Sprite Customer::getSprite()
{
	return m_Sprite;
}

int Customer::getSpeed()
{
	return m_Speed;
}

void Customer::setSpeed(int wave)
{
	m_Speed = m_Speed + (wave * 20);
}

void Customer::update(float elapsedTime,
	Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	// Update the customer position variables
	if (playerX > m_Position.x)
	{
		m_Position.x = m_Position.x +
			m_Speed * elapsedTime;
	}

	if (playerY > m_Position.y)
	{
		m_Position.y = m_Position.y +
			m_Speed * elapsedTime;
	}

	if (playerX < m_Position.x)
	{
		m_Position.x = m_Position.x -
			m_Speed * elapsedTime;
	}

	if (playerY < m_Position.y)
	{
		m_Position.y = m_Position.y -
			m_Speed * elapsedTime;
	}

	// Move the sprite
	m_Sprite.setPosition(m_Position);

	// Face the sprite in the correct direction
	float angle = (atan2(playerY - m_Position.y,
		playerX - m_Position.x)
		* 180) / 3.141;

	m_Sprite.setRotation(angle);


}