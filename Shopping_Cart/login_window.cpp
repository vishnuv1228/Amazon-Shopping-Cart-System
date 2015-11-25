#include "login_window.h"
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

LoginWindow::LoginWindow(char* filen)
{

  // Instantiate the parser
  DBParser parser;

  // Instantiate the individual product parsers we want
  parser.addProductParser(new ProductBookParser);
  parser.addProductParser(new ProductClothingParser);
  parser.addProductParser(new ProductMovieParser);

  // Now parse the database to populate the DataStore
  if( parser.parse(filen, df) ){
    cerr << "Error parsing!" << endl;
    return;
  }

  	setWindowTitle("Login");

	// Layout
	loginLayout = new QFormLayout();

	// Username and Password fields
	usernameInput = new QLineEdit();
	loginLayout->addRow("Username", usernameInput);

	pwInput = new QLineEdit();
	pwInput->setMaxLength(8);
	pwInput->setEchoMode(QLineEdit::Password);
	loginLayout->addRow("Password", pwInput);

	// Buttons

	// Login button
	loginButton = new QPushButton("Login");
	loginLayout->addRow(loginButton);
	connect(loginButton, SIGNAL(clicked()), this, SLOT(showMainWindow()));

	// Quit Button
	quit1Button = new QPushButton("Quit");
	loginLayout->addRow(quit1Button);
	connect(quit1Button, SIGNAL(clicked()), this, SLOT(quitWindow()));

	// New User Button
	newUserButton = new QPushButton("New User");
	loginLayout->addRow(newUserButton);
	connect(newUserButton, SIGNAL(clicked()), this, SLOT(showInfoWindow()));

	setLayout(loginLayout);
}
LoginWindow::~LoginWindow()
{
	
}
void LoginWindow::showMainWindow()
{
	if(usernameInput->text().isEmpty())
	{
		QErrorMessage* error = new QErrorMessage(usernameInput);
		error->showMessage("Please enter a valid username");
		return;
	}
	if ( pwInput->text().isEmpty())
	{
		QErrorMessage* error1 = new QErrorMessage(pwInput);
		error1->showMessage("Please enter a valid password");
		return;
	}

	// Check for whitespace
	for (int i = 0; i < pwInput->text().length(); i++)
	{
		if (pwInput->text()[i].isSpace())
		{
			QErrorMessage* error1 = new QErrorMessage(pwInput);
			error1->showMessage("Password must not have spaces");
			return;
		}
	}

	// Verify username and password with existing database 
	vector<User*> users = df.getUser();
	string username1 = usernameInput->text().toStdString();
	string password1 = pwInput->text().toStdString();
	string hash = hashPassword(password1);
	for (unsigned int i = 0; i < users.size(); i++)
	{
		
		if(users[i]->getName() == username1)
		{
			if (users[i]->getPassword() == hash)
			{
				mainwindow = new MainWindow(df, users[i]);
				mainwindow->show();
				close();
				return;
			}
			else
			{
				QErrorMessage* error1 = new QErrorMessage(loginButton);
				error1->showMessage("Incorrect Password");
				return;
			}
		}
	}
	QErrorMessage* error1 = new QErrorMessage(loginButton);
	error1->showMessage("Incorrect Username");
	
}
void LoginWindow::showInfoWindow()
{
	userInfoWindow = new QWidget();
	userInfoWindow->setWindowTitle("New User Information");

	// Layout
	userLayout = new QFormLayout();

	// Text Fields

	// Username
	usernInput = new QLineEdit();
	userLayout->addRow("Username", usernInput);

	// Password
	passInput = new QLineEdit();
	passInput->setMaxLength(8);
	passInput->setEchoMode(QLineEdit::Password);
	userLayout->addRow("Password", passInput);

	// Age 
	ageInput = new QLineEdit();
	userLayout->addRow("Age", ageInput);

	// Credit
	creditInput = new QLineEdit();
	userLayout->addRow("Credit", creditInput);

	// Buttons
	submitButton = new QPushButton("Submit");
	userLayout->addRow(submitButton);
	connect(submitButton, SIGNAL(clicked()), this, SLOT(submitUser()));

	// Show

	userInfoWindow->setLayout(userLayout);
	
	userInfoWindow->show();
}
void LoginWindow::submitUser()
{
	// Error checks
	if(usernInput->text().isEmpty() || passInput->text().isEmpty() || ageInput->text().isEmpty() || creditInput->text().isEmpty())
	{
		QErrorMessage* error1 = new QErrorMessage(pwInput);
		error1->showMessage("Please enter a valid password");
		return;
	}
	// Check for whitespace
	for (int i = 0; i < passInput->text().length(); i++)
	{
		if (passInput->text()[i].isSpace())
		{
			QErrorMessage* error1 = new QErrorMessage(pwInput);
			error1->showMessage("Password must not have spaces");
			return;
		}
	}

	// Convert QStrings to ints and strings
	string username, password;
	int age, balance, type;

	age = ageInput->text().toInt();
	balance = creditInput->text().toInt();
	username = usernInput->text().toStdString();
	password = hashPassword(passInput->text().toStdString());
	type = 0;
	// Create a new user using user input
	u = new User(username, age, balance, type, password);

	df.addUser(u);
	userInfoWindow->close();
}
string LoginWindow::hashPassword(string password)
{
	// Convert string to ASCII
	vector<long long> temp;
	int counter = password.length();
	for (unsigned int i = 0; i < password.length(); i++)
    {
       counter--;
       char x = password.at(i);
       long long value = pow(128, counter) * int(x);
       temp.push_back(value);
    }
    long long sum = 0;
    for (unsigned int i = 0; i < temp.size(); i++)
    {
    	sum += temp[i];
    }
    unsigned int* digits = convert(sum, 65521);
    int encrypt = (45912 * digits[3] + 35511 * digits[2] + 65169 * digits[1] + 4625 * digits[0]) % 65521;
    string encrypt_pw = to_string(encrypt);
    return encrypt_pw;
    

}
unsigned int* LoginWindow::convert(long long n, int b)
{
	unsigned int* temp = new unsigned int[4];
	for (int i = 0; i < 4; i++)
	{
		temp[i] = 0;
	}
	long long q = n;
	int k = 0;
	while (q != 0)
	{
		temp[k] = q % b;
		q = q/b;
		k = k + 1;
	}
	return temp;

}
void LoginWindow::quitWindow()
{
	close();
}