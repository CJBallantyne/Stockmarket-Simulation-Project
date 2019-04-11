//Connor Ballantyne 001155925
//Samuel Gordon
//Fayez Kabir 400032338

#include "Account.h"
#include <cmath>
#include "CompanyList.h"
#include "Node.h"

using namespace std;

void updateStockMarket(CompanyList, int);
void tradeStocks(Account*, CompanyList, int);
void manageBalance(Account*);
void viewPortfolio(Account*, CompanyList);
void viewCompanies(CompanyList, int);
void buyShares(Account*, int, Node*);
void sellShares(Account*, int, Node*);
void centerstring(char*);
bool signUpAccount(Account*, int*, int);
Account *logInAccount(Account*, int);

int main() {
	//Initialize Window Properties
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	system("CLS"); 
	srand(time(0));
	//**************************************************Declarations********************************************************************************

	int const MAX_ACCOUNTS=10;
	int totalAccounts = 0;
	int totalCompanies = 0;
	int choice;
	int menuItems = 3;
	ifstream inFile;
	ofstream outFile;
	string const accountFileName = "Accounts.txt";
	string const companyFileName = "Companies.txt";
	Account *activeAccount = nullptr;
	Account account[MAX_ACCOUNTS];
	CompanyList companies;
	char title[] = "Stock Market";
	char message[] = "Message";
	bool loggedIn = false;

	//**End of Declarations

	//*********************************************Get Companies from text File**********************************************************************
	
	Company c;
	inFile.open(companyFileName);
	if (!inFile) {
		cout << "Error 404: Accounts File Not found." << endl;
		system("pause");
		exit(1);
	}
	else {
		while (!inFile.eof()) {
			inFile >> c;
			c.setPreviousValue(c.getShareValue());
			companies.addCompany(c);
			totalCompanies++;
		}
	}

	//set index
	for (int i = 0; i < totalCompanies; i++) 
		companies.getNode(i)->company.setIndex(i); 

	inFile.close();
	//**End of Companies File Read

	//********************************************Get Accounts from text File***********************************************************************

	inFile.open(accountFileName);
	if (!inFile) {
		cout << "Error 404: Accounts File Not found." << endl;
		system("pause");
		exit(1);
	}
	else {
		while (!inFile.eof()) {
			inFile >> account[totalAccounts];
			if (account[totalAccounts].getNumberOfCompanies() < totalCompanies) { // Initialize any missing company share counts to 0
				for (int i = account[totalAccounts].getNumberOfCompanies(); i < totalCompanies; i++)
					account[totalAccounts].initializeNumberShares(0);
			}
			totalAccounts++;
		}
	}
	inFile.close();
	//**End of Account File Read

	//*********************************************************Menu Driven Interaction**************************************************************

	do {
		centerstring(title); // Display Title
		cout << endl << endl;
		if (menuItems == 3) { //Display default options
			cout << left << "Please select an option: \n"
							<< "[1] Sign Up \n"
							<< "[2] Log in \n"
							<< "[3] View Market without logging in \n";
		}
		else { //Display Options for Logged in User
			cout << "Logged in as: " << activeAccount->getUserName() << endl
				<< "[1] Manage Balance \n"
				<< "[2] Log out \n"
				<< "[3] View Market \n"
				<< "[4] Trade Stocks \n"
				<< "[5] View Portfolio \n";
		}
		cout << "[0] Exit Program \n";
		cin >> choice;
		while (true) { //Validate Input
			if (cin.fail()) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (choice > menuItems || choice < 0) {
				cout << "Invalid entry, please re-enter an option: ";
				cin >> choice;
			}
			if (!cin.fail() && choice >= 0 && choice <= menuItems)
				break;
		}
		system("CLS");
		switch (choice) {
		case 1: {
			if (menuItems == 5) {  //If logged in - Manage Balance
				manageBalance(activeAccount);
			}
			else { //If Not logged in - Sign up option
				if (totalAccounts == MAX_ACCOUNTS) { //check if max accounts has been reached
					centerstring(message);
					cout << endl << endl;
					cout << "Unable to add account. Max accounts has been reached." << endl;
				}
				else { //add account
					if (signUpAccount(account, &totalAccounts, totalCompanies)) {
						cout << "Account creation successful." << endl;
					}
					else {
						centerstring(message);
						cout << endl << endl;
						cout << "No New Account Created." << endl;
					}
				}
				system("pause");
				system("CLS");
			}
			break;
		}
		case 2: { //Login or Logout Option
			if (activeAccount != nullptr)
			{
				centerstring(message);
				cout << endl << endl;
				cout << activeAccount->getUserName() << " has logged out." << endl;
				activeAccount = nullptr;
				menuItems = 3;
			}
			else {//New Options if logged in
				activeAccount = logInAccount(account, totalAccounts);
				if (activeAccount != nullptr) {
					menuItems = 5;
				}
				else {
					centerstring(message);
					cout << endl << endl;
					cout << "Log in Failed." << endl;
				}
			}
			system("pause");
			system("CLS");
			break;
		}
		case 3: { //View Market option
			viewCompanies(companies, totalCompanies);
			centerstring(message);
			cout << endl << endl;
			cout << "Returning to Menu." << endl;
			system("pause");
			system("CLS");
			break;
		}
		case 4: { //Open stock trading platform
			tradeStocks(activeAccount, companies, totalCompanies);
			break;
		}
		case 5: { //Display share count of active user for each company and total value of shares
			viewPortfolio(activeAccount, companies);
			system("pause");
			system("CLS");
			break;
		}
		case 0: { //Exit program
			centerstring(message);
			cout << endl << endl;
			cout << "Ending program. \n";
			system("pause");
		}
		}
	} while (choice != 0);


	//**********************************************************************************************************************************************

	//*****************************************************Save Account Information In File*********************************************************
	
	outFile.open(accountFileName);

	for (int i = 0; i < totalAccounts; i++) {
		outFile << account[i];
		if (i != (totalAccounts - 1)) //Make sure not to add unneeded newline
			outFile << "\n";
	}
	outFile.close();
	//**End of Accounts Save**

	//*****************************************************Save Company Information In File*********************************************************
	
	outFile.open(companyFileName);

	for (int i = (totalCompanies -1); i >= 0; i--) {
		outFile << Company(companies.get(i));
		if (i != 0) //Make sure not to add unneeded newline
			outFile << "\n";
	}
	outFile.close();
	//**End of Companies Save


	//Release Dynamic Memory
	for (int i = 0; i < totalAccounts; i++)
		account[i].releaseMemory();

	companies.releaseMemory();

	return 0;
}
void updateStockMarket(CompanyList companies, int companyCount) { //Update the share value of each company in the companyList object
	for (int i = 0; i < companyCount; i++) {
		companies.getNode(i)->company.update();
	}
}
int getIndexFromTicker(string ticker, CompanyList companies, int companyCount) { //Get index of a company given a ticker (3 letters)
	int index = -1;
	for (int i = 0; i < companyCount; i++) {
		if (ticker == companies.get(i).getTicker()) {
			index = i;
			break;
		}
	}
	return index;
}
void tradeStocks(Account *account, CompanyList companies, int companyCount) { //Stock Trading Platform
	//Search for company using ticker string
	char userIn[5] = { '0' };
	string tickerSearch = "";
	int index = -1, choice, qty;
	char title[] = "Trading Platform";

	while (tickerSearch != "0000") { //Primary loop - user enters ticker of desired company
		centerstring(title);
		cout << endl << endl;
		cout << "Enter '0000' to exit" << endl;
		cout << "Enter the 3 character ticker for the company of interest: "; //Enter Ticker
		cin >> userIn;
		userIn[4] = '\0';
		tickerSearch = "";	//Reset Search String
		for (int i = 0; i < 3; i++)
			tickerSearch = tickerSearch + ((char)toupper(userIn[i])); //search for the upper case letters

		if ((string)userIn == "0000") { //Exit Primary loop condition
			cout << "Exiting Trading Platform." << endl;
			system("pause");
			system("CLS");
			break;
		}
		index = getIndexFromTicker(tickerSearch, companies, companyCount);
		if(index == -1) { //Ticker Not Found
			cout << "The ticker you entered was not found." << endl;
			system("pause");
			system("CLS");
		}
		else { //Display current shares and balance
			system("CLS");
			//Buy or Sell Menu
			do {
				centerstring(title);
				cout << endl << endl << setprecision(2) << fixed;
				cout << "Logged in as: " << account->getUserName() << endl
					<< "Selected Company: " << companies.get(index).getTicker() << endl
					<< "Company Name: " << companies.get(index).getName() << endl
					<< "Currently Owned Shares: " << account->getNumShares(index) << endl
					<< "Share Price: $" << companies.get(index).getShareValue() << endl
					<< "Account Balance: $" << account->getBalance() << endl
					<< "Please select an option: \n"
					<< "[1] Buy \n"
					<< "[2] Sell \n"
					<< "[0] Return to Stock Selection \n";
				cin >> choice;
				while (true) {
					if (cin.fail()) {
						cin.clear();
						cin.ignore(256, '\n');
					}
					if (choice > 2 || choice < 0) {
						cout << "Invalid entry, please re-enter an option: ";
						cin >> choice;
					}
					if (!cin.fail() && choice >= 0 && choice <= 2)
						break;
				}
				switch (choice) {
				case 1: {
					qty = 0;
					cout << "Enter the number of shares of " << companies.get(index).getTicker() << " you would like to buy: ";
					cin >> qty;
					if (qty > 0)
						buyShares(account, qty, companies.getNode(index));
					else 
						cout << "Invalid Quantity." << endl;
					
					break;
				}
				case 2: {
					qty = 0;
					cout << "Enter the number of shares of " << companies.get(index).getTicker() << " you would like to sell: ";
					cin >> qty;
					if (qty > 0)
						sellShares(account, qty, companies.getNode(index));
					else 
						cout << "Invalid Quantity." << endl;
					
					break;
				}
				case 0: {
					break;
				}
				}
				system("pause");
				system("CLS");
			} while (choice != 0);
		}
	}
}
void viewCompanies(CompanyList companies, int companyCount) { //List each company's ticker, share price and change in value.
	char title[] = "Company Share Price Changes";
	int choice = 1;
	do {
		centerstring(title);
		cout << endl << endl;
		cout << right << setw(8) << "Ticker" << setw(15) << " | Shares Price" << setw(9) << " | Change" << endl; //print table header
		cout << "==================================" << endl;
		for (int i = 0; i < companyCount; i++) {
			companies.getNode(i)->company.display(); //Display each company
		}
		cout << "==================================" << endl << endl;

		cout << "[1] Simulate Market " << endl //Option to simulate market changes
			<< "[0] Exit " << endl;
		cin >> choice;
		while (true) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (choice > 1 || choice < 0) {
				cout << "Invalid entry, please re-enter an option: ";
				cin >> choice;
			}
			if (!cin.fail() && choice >= 0 && choice <= 2)
				break;
		}
		system("CLS");
		if (choice == 1)
			updateStockMarket(companies, companyCount);
	} while (choice != 0);

}
void viewPortfolio(Account *account, CompanyList companies) {
	int shares;
	double netWorth = 0;
	char title[] = "Stock Portfolio";
	centerstring(title);
	cout << endl << endl << setprecision(2) << fixed;
	cout << "Logged in as: " << account->getUserName() << endl
		<< "Account Balance: $" << account->getBalance() << endl;
	cout << right << setw(14) << "Company" << setw(19) << "Shares Owned" << setw(18) << "Total Value" << endl; //Print table header
	cout << "==========================================================" << endl;
	for (int i = 0; i < account->getNumberOfCompanies(); i++) { //Display share count for each company
		shares = account->getNumShares(i);
		netWorth += shares * companies.get(i).getShareValue();
		cout << setprecision(0) << fixed;
		cout << right << setw(12) << companies.get(i).getTicker() << setw(19) << shares 
			<< setprecision(2) << fixed << setw(19) << (shares*companies.get(i).getShareValue()) << endl;
	}
	cout << "==========================================================" << endl; //Display Net Worth
	cout << right << setw(37) << "Networth: " << setw(13) << (netWorth + account->getBalance()) << endl;
}
void manageBalance(Account *account) {
	int choice;
	double userIn;
	char title[] = "Account Balance Management";
	do {
		centerstring(title);
		cout << endl << endl << setprecision(2) << fixed; 
		cout << "Logged in as: " << account->getUserName() << endl
			<< "Account Balance: $" << account->getBalance() << endl; //Display active account balance and name
		cout << "Please select an option: \n"		//Choose to deposit, withdraw or exit function
			<< "[1] Deposit \n"
			<< "[2] Withdraw \n"
			<< "[0] Return to Menu \n";
		cin >> choice;
		while (true) {
			if (cin.fail()) {
				cin.clear();
				cin.ignore(256, '\n');
			}
			if (choice > 2 || choice < 0) {
				cout << "Invalid entry, please re-enter an option: ";
				cin >> choice;
			}
			if (!cin.fail() && choice >= 0 && choice <= 2)
				break;
		}

		switch (choice) { //Preform desired action and check for errors
			case 1: {
				cout << "Enter the ammount you would like to deposit: $";
				cin >> userIn;
				account->deposit(userIn);
				cout << "Deposit Successful." << endl;
				break;
			}
			case 2: {
				cout << "Enter the ammount you would like to Withdraw: $";
				cin >> userIn;
				if (userIn <= account->getBalance()) {
					account->withdraw(userIn);
					cout << "Withdrawal Successful." << endl;
				}
				else
					cout << "Insufficient Funds! No withdrawal made" << endl;
				break;
			}
			case 0: {
				break;
			}

		}
		system("pause");
		system("CLS");
	} while (choice != 0);
	


}
void buyShares(Account *a, int shares, Node *c) {
	Account account = *a;
	if (shares > c->company.getNumberOfShares()) {
		cout << "Not enough shares. Buying all available shares" << endl;
		shares = c->company.getNumberOfShares();
	}
	if (account.buyShares(c->company.getIndex(), shares, c->company.getShareValue())) { //If purchase is successfull
		c->company.setNumberOfShares(c->company.getNumberOfShares() - shares);  //company reduces number of shares available
		cout << "Successfully purchased " << shares << " shares for $" << setprecision(2) << fixed << (shares*c->company.getShareValue()) << "." << endl;
	}
	else {
		cout << "Insufficient balance for that many shares." << endl;
	}
	*a = account;
}
void sellShares(Account *a, int shares, Node *c) {
	Account account = *a;
	if (shares > account.getNumShares(c->company.getIndex())) {				//Check that account has sufficeint shares
		cout << "Not enough shares. Selling all owned shares." << endl;
		shares = account.getNumShares(c->company.getIndex());				//If not sell all owned shares
	}
	account.sellShares(c->company.getIndex(), shares, c->company.getShareValue());	//Sell shares to company(index) at share value
	c->company.setNumberOfShares(c->company.getNumberOfShares() + shares);		//Company(index) recieves shares
	cout << "Successfully sold " << shares << " shares for $" << setprecision(2) << fixed << (shares*c->company.getShareValue()) << "." << endl;
	*a = account;
}
void centerstring(char* s)
{//Centers a c string on the console output
	int l = strlen(s);
	int pos = (int)((80 - l) / 2);
	for (int i = 0; i < pos; i++)
		cout << " ";
	cout << s;
}
Account *logInAccount(Account *accounts, int accountCount) {
	string userIn = "0";
	int activeAccount = -1;
	int attempts = 3;
	char title[] = "Account Log In";
	//Choose account by user name
	while (userIn != "0000") {
		centerstring(title);
		cout << endl << endl;
		cout << "Enter '0000' To Cancel Login" << endl;
		cout << "Enter Account User Name: "; 
		cin >> userIn;
		if (userIn == "0000") {
			cout << "Cancelling Login." << endl;
			break;
		}
		for (int i = 0; i < accountCount; i++) {
			if (userIn == accounts[i].getUserName()) { //Found Username
				activeAccount = i;
				break;
			}
		}
		if (activeAccount == -1) { //If UserName Is Not Found
			if (attempts == 0) {  //If out of Attempts - Failed Login, return false
				cout << "No Attempts Remaining. Cancelling Login." << endl;
				return false;
			}
			cout << "User Name Not Found. " << attempts-- << " Attempts Remaining." << endl;
			system("pause");
			system("CLS");
		}
		else {
			cout << "User Found: " << accounts[activeAccount].getUserName() << endl;
			break;
		}
	}
	//password validation with 3 attempts
	attempts = 3;
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;

	while (userIn != "0000") {
		GetConsoleMode(hStdin, &mode);
		SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT)); //Hide User input
		cout << "Enter Account Password: "; cin >> userIn;
		if (userIn == accounts[activeAccount].getPassword()) { //If correct Password
				//Successful Log in - Set active account and return pointer
			cout << "Login Successful!" << endl;
			SetConsoleMode(hStdin, mode); //Restore User input Mode
			return &accounts[activeAccount];
		}
		else {
			if (attempts == 0) { 	//Failed Log in attempt return nullptr
				cout << "Password Incorrect. No Remaining Attemps. Cancelling Login." << endl;
				system("pause");
				system("CLS");
				SetConsoleMode(hStdin, mode); //Restore User input Mode
				return nullptr;
			}
			cout << "Password Incorrect. " << attempts-- << " Attempts Remaining." << endl;
		}
	}
	return false;
}
bool signUpAccount(Account *accounts, int *accountCount, int companyCount) {
	int const MIN_US = 5, MAX_US = 15, MIN_P = 5, MAX_P = 15;
	string userIn, userName, password;
	char passCheck[50];
	double balance = 0.00;
	bool invalidEntry = true, hasUpper = false, hasLower = false, hasDigit = false;
	int totalAccounts = *accountCount;
	char title[] = "Account Creation";
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	while (invalidEntry) { //Get User Name
		centerstring(title);
		cout << endl << endl;
		cout << "Enter '0000' To Cancel Sign up" << endl;
		cout << "User name must be 5 to 15 characters long" << endl;
		cout << "Enter Desired User Name: ";
		cin >> userIn;
		if (userIn == "0000") { //Canceled Sign up - return false
			cout << "Cancelling Sign Up." << endl;
			system("pause");
			system("CLS");
			return false;
		}

		if (userIn.length() > MAX_US) { //UserName length invalid
			cout << "User name is too long. Maximum " << MAX_US << " characters." << endl;
		}
		else if(userIn.length() < MIN_US){
			cout << "User name is too short. Minimum " << MIN_US << " characters" << endl;
		}
		else {
			for (int i = 0; i < totalAccounts; i++) { //Check for existing user with same name
				if (userIn == accounts[i].getUserName()) {
					cout << "That user name is already in use" << endl;
					invalidEntry = true;
					break;
				}
				else
					invalidEntry = false;
			}
		}
		system("pause");
		system("CLS");
	}
	userName = userIn; //Valid Username entered
	cout << "User Name: " << userName;
	invalidEntry = true;
	while (invalidEntry) { // Get new Account Password
		centerstring(title);
		cout << endl << endl;
		cout << "Enter '0000' To Cancel Sign up" << endl;
		cout << "Password must be 5 to 15 characters long" << endl;
		cout << "Password must include a number and a letter" << endl;
		GetConsoleMode(hStdin, &mode);
		SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT)); //Hide User input
		cout << "Enter Desired Password: ";
		cin >> passCheck;
		SetConsoleMode(hStdin, mode); //Restore User input Mode
		cin.ignore(100, '\n');
		passCheck[49] = '\0';

		if ((string)passCheck == "0000") { //Canceled Sign up - return false
			cout << "Cancelling Sign Up." << endl;
			system("pause");
			system("CLS");
			return false;
		}
		//Check Validity of password
		if (strlen(passCheck) > MAX_P) { //Check length
			cout << "Password is too long. Maximum " << MAX_P << " characters" << endl;
		}
		else if(strlen(passCheck) < MIN_P){
			cout << "Password is too short. Minimum " << MIN_P << " characters" << endl;
		}
		else { //Check for number and letter
			for (int i = 0; i < strlen(passCheck); i++) {
				if (isupper(passCheck[i]))
					hasUpper = true;
				if (islower(passCheck[i]))
					hasLower = true;
				if (isdigit(passCheck[i]))
					hasDigit = true;
			}
			if (hasUpper && hasLower && hasDigit) {	//if valid password
				cout << "Password is valid." << endl;
				invalidEntry = false;
			}
			else {
				cout << "The password entered does not include the required character types." << endl;
			}
		}
		system("pause");
		system("CLS");
	}

	password = passCheck;
	accounts[totalAccounts] = Account(userName, password, balance);

	//Initialize share for all companies to zero
	for (int i = 0; i < companyCount; i++) {
		accounts[totalAccounts].initializeNumberShares(0);
	}

	*accountCount = totalAccounts + 1;
	return true;
}