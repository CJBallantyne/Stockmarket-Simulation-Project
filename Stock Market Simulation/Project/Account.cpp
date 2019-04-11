//Connor Ballantyne 001155925
//Samuel Gordon
//Fayez Kabir 400032338

#include "Account.h"

Account::Account() { //Default account constructor
	head = nullptr;
	userName = "Undefined";
	numberOfCompanies = 0;
}

Account::Account(std::string name, std::string pass, double bal) { //Basic Account constructor
	userName = name;
	password = pass;
	balance = bal;
	numberOfCompanies = 0;
}
void Account::releaseMemory() {
	delete[] head;
}

void Account::initializeNumberShares(int shares) {
	if (!head) //If this is the first node - declare as head
		head = new Node(shares);
	else { //otherwise make a new node with the current head as the next node
		Node *temp = new Node(shares);
		temp->next = head;
		head = temp;
	}
	numberOfCompanies++;
}

void Account::deposit(double d) {
	if (d > 0)
		balance += d;
}
void Account::withdraw(double w) {
	if (w > balance)
		balance = 0;
	else if((w <= balance) && w > 0)
		balance -= w;
}
void Account::setNumShares(int index, int num) { // Change value of node at index in linked list
	if (index == 0) {
		head->val = num;
	}
	else {
		Node *temp = head;
		for (int i = 0; i < index; i++) {
			if (temp->next == nullptr)
				break;
			temp = temp->next;
		}
		temp->val = num;
	}
}
bool Account::buyShares(int index, int qty, double price) { //index, qty, price
	if (balance >= price * qty) {	//If enough money buy the shares and return true
		setNumShares(index, (getNumShares(index)) + qty);	//update number of shares owned
		balance -= price * qty;	//Pay for the shares from balance
		return true;
	}
	else {	//else insufficient balance and return false
		return false;
	}
}
//int CompanyIndex, int Quantity, Int SharePrice
void Account::sellShares(int index, int qty, double price) {
	if (getNumShares(index) < qty) {  //Make sure account owns sufficient shares
		qty = getNumShares(index);
	}
	if (qty != 0)
	{
		setNumShares(index, (getNumShares(index)) - qty); //subtract sold shares
		balance += price * qty;							//Add to balance
	}
}

int Account::getNumShares(int index) {
	Node *temp = head;
	for (int i = 0; i < index; i++) {
		if (temp->next == nullptr)
			return 0;
		temp = temp->next;
	}
	return temp->val;
}

std::ofstream& operator <<(std::ofstream &out, Account &acct) {

	out << acct.userName;
	out << ";" << acct.password << ";" << acct.balance << ";";

	//Loop to output all share counts
	for (int i = (acct.numberOfCompanies-1); i >= 0; i--) {
		out << acct.getNumShares(i) << ";";
	}
	out << "~;";

	return out;
}

std::ifstream& operator >>(std::ifstream& in, Account &acct) {
	std::string temp;
	std::getline(in, acct.userName, ';');
	std::getline(in, acct.password, ';');
	std::getline(in, temp, ';');

	acct.balance = stof(temp);

	//Loop to get all share counts
	while (true) {
		std::getline(in, temp, ';');
		if (temp == "~") {
			std::getline(in, temp, '\n');
			break;
		}
		else {
			acct.initializeNumberShares(stoi(temp));
		}
	}

	 return in;
}