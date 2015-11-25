#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QRadioButton>
#include <QComboBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QMessageBox>
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
#include <cmath>
#include <queue>

class MainWindow : public QWidget
{
	Q_OBJECT
public:
	MainWindow(DataFunction& ds, User* u);
	~MainWindow();

private slots:
	void andRadio();
	void orRadio();
	void addSearch();
	void alphaSort();
	void rateSort();
	void addtoCart();
	void viewtheCart();
	void buytheCart();
	void deletefromCart();
	void index(int row);
	void viewReview(QListWidgetItem* item);
	void addReview();
	void addHelper();
	void quit();
	void saveFile();
private:
	void sortList();
	void sortReviews();
	void showLogin();

	// // UI

	// Layouts
	QHBoxLayout* overallLayout;

	// Titles
	QLabel* resultsLabel;
	QLabel* reviewsLabel;

	// Form for new search
	QFormLayout* formLayout;
	QLineEdit* searchInput;
	QRadioButton* andButton;
	QRadioButton* orButton;
	QPushButton* searchButton;
	

	// Form for users
	QFormLayout* form1Layout;

	// List of search results
	QListWidget* searchListWidget;

	// Review Layout
	QFormLayout* form2Layout;
	QListWidget* reviewListWidget;

	// Add Review Layout and Buttons
	QWidget* revWin;
	QPushButton* addRev;
	QFormLayout* addReviewLayout;
	QLabel* revTitle;
	QLineEdit* ratingInput;
	QLineEdit* dateInput;
	QLineEdit* textInput;
	QPushButton* addReviewButton;

	// View Cart Layout and Buttons
	QWidget* cartWin;
	QLabel* listTitle;
	QFormLayout* cartLayout;
	QListWidget* cart_list;
	QPushButton* buyCart;
	QPushButton* deleteItem;

	// Save to Database
	QLineEdit* filenameInput;
	QPushButton* saveButton;

	// Cart Buttons
	QPushButton* addCart;
	QPushButton* viewCart;

	// Sorting  
	QPushButton* alphaButton;
	QPushButton* rateButton;

	QPushButton* quitButton;

	// // Data
	int delt;
	// List of Products matching User's search terms
	std::vector<Product*> hits;
	std::vector<Review*> revs;
	std::map<User*, std::deque<Product*> > cartMap;
	// Datastore object
	DataFunction df;
	// ID to find out whether the user selected AND or OR
	int id;
	std::string product_text;
	// Counter to see if this is the first or greater click
	int count;
	int count1;

	User* user;
};