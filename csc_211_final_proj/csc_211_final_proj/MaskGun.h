#pragma once
#include "Customer.h"

using namespace sf;

int createBackground(VertexArray& rVA, IntRect arena);
Customer* createCustomers(int numCustomers, IntRect arena);

