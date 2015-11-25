#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QErrorMessage>
#include "main_window.h"
#include "product.h"
#include "datastore.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "review.h"
#include "datafunction.h"
#include "msort.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

class LoginWindow : public QWidget
{
	Q_OBJECT
public:
	LoginWindow(char* filen);
	~LoginWindow();

private slots:
	void showMainWindow();
	void quitWindow();
	void showInfoWindow();
	void submitUser();
private:
	// Login
	QFormLayout* loginLayout;
	QLineEdit* usernameInput;
	QLineEdit* pwInput;
	QPushButton* loginButton;
	QPushButton* quit1Button;
	QPushButton* newUserButton;

	// User Information Dialog
	QWidget* userInfoWindow;
	QFormLayout* userLayout;
	QLineEdit* ageInput;
	QLineEdit* creditInput;
	QLineEdit* passInput;
	QLineEdit* usernInput;
	QPushButton* submitButton;

	// Show mainWindow
	MainWindow* mainwindow;

	// Function to hash passwords
	std::string hashPassword(std::string password);
	unsigned int* convert(long long n, int b);

	User* u;
	DataFunction df;

};