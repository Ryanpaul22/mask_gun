#include "pch.h"
#include "MaskGun.h"
#include "Customer.h"

Customer* createCustomers(int numCustomers, IntRect arena)
{
	Customer* customers = new Customer[numCustomers];

	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	for (int i = 0; i < numCustomers; i++)
	{

		// Which side should the customer spawn
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;

		switch (side)
		{
		case 0:
			// left
			x = minX;
			y = (rand() % maxY) + minY;
			break;

		case 1:
			// right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;

		case 2:
			// top
			x = (rand() % maxX) + minX;
			y = minY;
			break;

		case 3:
			// bottom
			x = (rand() % maxX) + minX;
			y = maxY;
			break;
		}

		// Type of customer
		srand((int)time(0) * i * 2);
		int type = (rand() % 5);

		// Spawn the new customer into the array
		customers[i].spawn(x, y, type, i);

	}
	return customers;
}