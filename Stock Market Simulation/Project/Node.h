//Connor Ballantyne 001155925
//Samuel Gordon 
//Fayez Kabir 400032338

#pragma once

#ifndef NODE_H
#define NODE_H

#include "Company.h"

struct Node {
	Company company;
	Node *next;
	Node(Company c) : company(c), next(nullptr) {};
	
};

#endif // !NODE_H
