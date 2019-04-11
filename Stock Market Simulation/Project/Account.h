//Connor Ballantyne 001155925
//Samuel Gordon
//Fayez Kabir 400032338

#pragma once

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string>
#include <fstream>
#include "List.h"
/**
* Holds Data for an account holder
**/
class Account {
private:

	double balance; //Stores Account balance used to purchase stocks
	std::string userName, password;  //Username and password for Accounts
	int numberOfCompanies; //Number of companies (used as index for shares list)
	struct Node // Linked list structure for share count
	{
		int val;
		Node *next;
		Node(int i) : val(i), next(nullptr) {};
	};
	Node *head;
public:
	Account(); //Default constructor
	Account(std::string, std::string, double);

	// Member Fuctions
	void deposit(double d); 
	void withdraw(double w);
	void releaseMemory();

	//Linked List Accessors and Mutators
	void initializeNumberShares(int);		//Add number of shares to linked list
	int getNumShares(int);					//Return number of owned shares of company index
	bool buyShares(int, int, double);		//Add amount of shares at company share price if sufficient balance
	void sellShares(int, int, double);		//Sell amount of shares at company price, add to balance
	void setNumShares(int, int);			//At company index, set number of shares

	//Mutators
	void setUserName(std::string s) { userName = s; }
	void setPassword(std::string s) { password = s; }

	//Accessors
	int getNumberOfCompanies() const { return numberOfCompanies; }
	double getBalance() const { return balance; }
	std::string getUserName() const { return userName; }
	std::string getPassword() const { return password; }

	//Overloaded file stream operators
	friend std::ofstream& operator <<(std::ofstream &, Account &);
	friend std::ifstream& operator >>(std::ifstream&, Account &);
};

#endif // !ACCOUNT_HOLDER_H
