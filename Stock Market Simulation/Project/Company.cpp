//Connor Ballantyne 001155925
//Samuel Gordon
//Fayez Kabir 400032338

#include "Company.h"

int Company::numOfCompanies = 0;

Company::Company(std::string n,std::string t,int num,double v,double d){
	setName(n);
	setTicker(t);
	setNumberOfShares(num);
	setShareValue(v);
	setDividends(d);
	previousValue=0;
}

Company::Company(const Company &c) {
	this->name = c.name;
	this->ticker = c.ticker;
	this->dividends = c.dividends;
	this->rPos = c.rPos;
	this->index = c.index;
	this->shareNumber = c.shareNumber;
	this->shareValue = c.shareValue;
	this->previousValue = c.previousValue;
}

void Company::doubleShares(){
	setNumberOfShares(shareNumber*2);
	setShareValue(shareValue/2.0);
}

std::ofstream& operator <<(std::ofstream &out ,const Company &com){
	out << com.getName() << ";" <<com.getTicker()<<';'<<com.getNumberOfShares() << ";"<< com.getShareValue()<<';'<<com.getDividends() << ";~;";
	return out;
}

std::ifstream&operator >>(std::ifstream &in, Company &com) {
	std::string temp;
	std::getline(in, com.name, ';');
	std::getline(in, com.ticker, ';');

	std::getline(in, temp, ';');
	com.shareNumber = stoi(temp);

	std::getline(in, temp, ';');
	com.shareValue = stof(temp);

	std::getline(in, temp, ';');
	com.dividends = stof(temp);

	std::getline(in, temp, '\n');

	return in;
}

void Company::payDividends(Account p){
	p.deposit(p.getNumShares(index)*dividends);
}

void Company::doubleShares(Account p){
	p.setNumShares(index,p.getNumShares(index)*2);
}

void Company::update(){
	previousValue = shareValue;
	shareValue += ((double) rand()/RAND_MAX*0.1 - 0.05)*shareValue;
	if (shareValue < 0.01)
		shareValue = 0.01;
}

void Company::print() const
{
	std::cout << "Company name: " << getName() << " (" << getTicker() << ")\n" << "Total shares: " << getNumberOfShares() << std::endl
		<< "Share Value: $" << std::setprecision(2) << std::fixed << getShareValue() << "\nDividends per share: ";
	if (dividend)
		std::cout << "$" << getDividends()<<std::endl;
	else
		std::cout << "N/A"<<std::endl;
}

void Company::display() const
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << std::setprecision(2) << std::fixed << std::right;
	if(this->getShareValue() - this->previousValue >= 0){
		SetConsoleTextAttribute(hConsole,10);

		std::cout << std::setw(7) << getTicker() << std::setw(12) << this->getShareValue() 
			<< std::setw(8) << "+" << (this->getShareValue() - this->previousValue) << std::endl;

	}else if(this->getShareValue() - this->previousValue <0){
		SetConsoleTextAttribute(hConsole,12);

		std::cout << std::setw(7) << getTicker() << std::setw(12) << this->getShareValue() 
			<< std::setw(12)  << (this->getShareValue() - this->previousValue) << std::endl;

	}
	
	SetConsoleTextAttribute(hConsole,7);
}