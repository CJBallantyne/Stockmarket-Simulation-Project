//Connor Ballantyne 001155925
//Samuel Gordon
//Fayez Kabir 400032338

#pragma once

#ifndef COMPANY_LIST_H
#define COMPANY_LIST_H

#include "Node.h"

class CompanyList{
private:

	Node *head;

public:
	void addCompany(Company c) {
		if (!head)
			head = new Node(c);
		else {
			Node *temp = new Node(c);
			temp->next = head;
			head = temp;
		}
	}

	Company get(int index) {
		Node *temp = head;
		for (int i = 0; i < index; i++) {
			if (temp->next == nullptr)
				break;
			temp = temp->next;
		}
		return temp->company;
	}

	Node *getNode(int index) {
		Node *temp = head;
		for (int i = 0; i < index; i++) {
			if (temp->next == nullptr)
				break;
			temp = temp->next;
		}
		return temp;
	}

	void releaseMemory() {
		delete head;
	}

};

#endif // !LIST_H
