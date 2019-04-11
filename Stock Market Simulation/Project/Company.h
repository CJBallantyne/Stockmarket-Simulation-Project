//Connor Ballantyne 001155925
//Samuel Gordon
//Fayez Kabir 400032338

#ifndef COMPANY_H
#define COMPANY_H

#include <iostream>
#include <string>
#include <iomanip>
#include "Account.h"
#include <fstream>
#include <windows.h>

class Company {
private:
	std::string name, ticker;	//name of company and symbol for the market
	int shareNumber, index;		//total number of shares
	static int numOfCompanies;
	double shareValue, dividends, previousValue, rPos;//$value of each individual share, quarterly payout per share
	bool dividend;


public:
	Company() { name = ticker = shareNumber = shareValue = dividends = previousValue = 0; index = numOfCompanies; numOfCompanies++; rPos = rand(); }//default constructor
	Company(std::string, std::string, int, double, double);//the constructor that takes areguments
	Company(const Company &c);

	//accessors
	std::string getName()const { return name; }
	std::string getTicker()const { return ticker; }
	int getNumberOfShares()const { return shareNumber; }
	double getShareValue()const { return shareValue; }
	double getDividends()const { return dividends; }
	int getIndex() const { return index; }

	//mutator
	void setIndex(int i) { index = i; }
	void setName(std::string n) { name = n; }
	void setTicker(std::string t) { ticker = t; }
	void setNumberOfShares(int s) { shareNumber = s; }
	void setShareValue(double v) { shareValue = v; }
	void setDividends(double d) { dividends = d; }
	void setPreviousValue(double d) { previousValue = d; }

	//other functions
	void payDividends(Account p);//pays dividends to to given account holder
	void doubleShares();//doubles number of shares, halves stock prices and dividends
	void doubleShares(Account shares);//doubles the number of shares of this company;
	void update();//changes the stock price randomly
	void print() const; // Displays company information to console
	void display() const;

	friend std::ofstream& operator <<(std::ofstream&, const Company &);

	friend std::ifstream&operator >>(std::ifstream&, Company &);
};

#endif