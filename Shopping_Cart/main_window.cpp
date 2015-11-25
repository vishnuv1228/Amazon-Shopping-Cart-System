#include "main_window.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(DataFunction& ds, User* u)
{
	df = ds;
	user = u;
	// Title
	setWindowTitle("Amazon");

	// Overall Layout
	overallLayout = new QHBoxLayout();

	// // Form to search for products
	// Layout
	formLayout = new QFormLayout();
	overallLayout->addLayout(formLayout);

	// Search terms input
	searchInput = new QLineEdit();
	formLayout->addRow("Search", searchInput);

	// AND button - change this to radio button 
	andButton = new QRadioButton("AND");
	formLayout->addRow(andButton);
	connect(andButton, SIGNAL(clicked()), this, SLOT(andRadio()));

	// OR button - change this to radio button 
	orButton = new QRadioButton("OR");
	formLayout->addRow(orButton);
	connect(orButton, SIGNAL(clicked()), this, SLOT(orRadio()));

	// Search Button
	searchButton = new QPushButton("Search");
	connect(searchButton, SIGNAL(clicked()), this, SLOT(addSearch()));
	formLayout->addRow(searchButton);
	count = 0;

	// Alphabetical Sorting Button
	alphaButton = new QPushButton("Sort Alphabetically");
	connect(alphaButton, SIGNAL(clicked()), this, SLOT(alphaSort()));
	formLayout->addRow(alphaButton);

	// Average Rating Sorting Button
	rateButton = new QPushButton("Sort by Average Rating");
	connect(rateButton, SIGNAL(clicked()), this, SLOT(rateSort()));
	formLayout->addRow(rateButton);

	// // Form to select User and cart
	// Layout
	form1Layout = new QFormLayout();
	overallLayout->addLayout(form1Layout);

	// Search Results Label
	resultsLabel = new QLabel("Results:");
	form1Layout->addRow(resultsLabel);

	// List of products
	searchListWidget = new QListWidget();
	form1Layout->addRow(searchListWidget);


	// Add Cart
	addCart = new QPushButton("Add to Cart");
	connect(addCart, SIGNAL(clicked()), this, SLOT(addtoCart()));
	form1Layout->addRow(addCart);

	// View Cart
	viewCart = new QPushButton("View Cart");
	connect(viewCart, SIGNAL(clicked()), this, SLOT(viewtheCart()));
	form1Layout->addRow(viewCart);

	// // Form to view Reviews
	// Layout
	form2Layout = new QFormLayout();
	overallLayout->addLayout(form2Layout);
	// Reviews Label
	reviewsLabel = new QLabel("Reviews:");
	form2Layout->addRow(reviewsLabel);

	// List of Reviews
	reviewListWidget = new QListWidget();
	form2Layout->addRow(reviewListWidget);
	count1 = 0;

	// Add Review Button
	addRev = new QPushButton("Add Review");
	connect(addRev, SIGNAL(clicked()), this, SLOT(addReview()));
	form2Layout->addRow(addRev);

	// Quit to database
	filenameInput = new QLineEdit();
	formLayout->addRow("Enter Filename: ", filenameInput);

	// Save button
	saveButton = new QPushButton("Save");
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveFile()));
	formLayout->addRow(saveButton);

	// Quit Button
	quitButton = new QPushButton("Quit");
	connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
	form2Layout->addRow(quitButton);

	// Set overall layout
	setLayout(overallLayout);
	vector<Review*> reviews = df.getReview();
	vector<Product*> prods1 = df.getProduct();
	vector<User*> users = df.getUser();
	set<Review*> set1;
	set<Review*> set2;
	set<Review*> reviewv;
	map<string, map<string, double> > user_sim;
	double sim1 = 0;
	double sum = 0;
	int counter = 0;
	for (unsigned int i = 0; i < users.size(); i++)
	{
		for (unsigned int j = 0; j < users.size(); j++)
		{
			if (i == j)
			{
				continue;
			}
			counter = 0;
			sum = 0;
			set1.clear();
			set2.clear();
			for (unsigned int k = 0; k < reviews.size(); k++)
			{
					if (users[i]->getName() == reviews[k]->user)
					{
						// Set of all products reviewed for user 1
						set1.insert(reviews[k]);
					}
					if (users[j]->getName() == reviews[k]->user)
					{
						// Set of all products reviewed for user 2
						set2.insert(reviews[k]);
					}
				
			}
			for (set<Review*>::iterator it = set1.begin(); it != set1.end(); ++it)
			{
				for (set<Review*>::iterator it1 = set2.begin(); it1 != set2.end(); ++it1)
				{
					// Set intersection
					if ((*it)->prodName == (*it1)->prodName)
					{
						// Make sure they don't share the same name
							counter++;
							// Calculate basic similarities
							sim1 = abs ((*it)->rating - (*it1)->rating);
							sim1 = sim1/4;
							sum += sim1;
					}
				}
			}
			sum = sum/counter;
			if (counter == 0)
			{
				sum = 1;
			}
			map<string, map<string, double> >::iterator it1 = user_sim.find(users[i]->getName());
			if (it1 == user_sim.end())
			{
				map<string, double> str;
				str.insert(make_pair(users[j]->getName(), sum));
				user_sim.insert(make_pair(users[i]->getName(), str));
			}
			else
			{
				// grab internal map
				map<string, double> temp = it1->second;
				// insert into that map
				temp.insert(make_pair(users[j]->getName(), sum));
				user_sim[users[i]->getName()] = temp;
			}
		}
	}

	// Comparator object of a pair of string, double represented as user and its associated similarity

	struct doubleGreater
	{
		bool operator()(const pair<string, double> lhs, const pair<string, double> rhs)
		{
			return lhs.second > rhs.second;
		}
	};
		
	// Dijsktra's algorithm 
	
	priority_queue<pair<string, double>, vector<pair<string, double> >, doubleGreater> mypq;
	map<string, double> user_distance;
	// User that's logged in - mark it as 0 in the adjacency_list
	user_distance.insert(make_pair(user->getName(), 0));
	// Set of visted nodes
	set<string> visited;
	// Find the logged in user
	map<string, map<string, double> >::iterator iet = user_sim.find(user->getName());
	// Initial push
	pair<string, double> firstUser = make_pair(user->getName(), 0);
	mypq.push(firstUser);
	// Iterate through logged in user's vertices
	for (map<string, double>::iterator ift = iet->second.begin(); ift != iet->second.end(); ++ift)
	{
		user_distance.insert(make_pair(ift->first, 2));
		// Make a pair of other users and their corresponding similarities
		pair<string, double> otherUsers = make_pair(ift->first, ift->second);
		// Push the pair into the priority queue
		mypq.push(otherUsers);
	}
	while(!mypq.empty())
	{
		pair<string, double> min = mypq.top();
		// Pop min similarity
		mypq.pop();
		// Find the current user's similariity
		visited.insert(min.first);
		map<string, map<string, double> >::iterator user_curr = user_sim.find(min.first);
		for(map<string, double>::iterator it2 = user_curr->second.begin(); it2 != user_curr->second.end(); ++it2)
		{	
			set<string>::iterator it = visited.find(it2->first);
			map<string, double>::iterator it1 = user_distance.find(it2->first);
			// If the sum of v's distance (min) and the edge weight (it4->second) is less than its neighbor, u's distance
			if (((min.second + it2->second) < it1->second) &&  it == visited.end())
			{
				double dist = min.second + it2->second;
				// Decrease key - update the priority queue
				set<pair<string, double> > temp1;
                while (!mypq.empty()) 
                {
                    temp1.insert(mypq.top());
                    mypq.pop();
                }
                set<pair<string, double> >::iterator it;

                for (it = temp1.begin(); it != temp1.end(); ++it) 
                {
                    pair<string, double> n; 
                    n = make_pair((*it).first, (*it).second);
                    mypq.push(n); 
                    map<string, double>::iterator it_t1 = user_distance.find(it1->first);
                    it_t1->second = dist;
                }
			}
		}
	}

	unsigned int num_user_revs = 0;
	// Check if user has reviewed all products
	for (unsigned int i = 0; i < reviews.size(); i++)
	{
		if (user->getName() == reviews[i]->user)
		{
			num_user_revs++;
		}
	}
	/* Let's say that the logged-in user is A, and we calculate the "interestingness" of product P to A.
Let S be the set of all other users who have rated P. (We only look at products P that A has not yet rated.)
For each user B in S, let r(B,P) be the rating that B gave P, and s(B,A) the similarity between B and A.
Let R(P) be the sum of (1-s(B,A))*r(B,P), over all users B in S.
Let W be the sum of (1-s(B,A)) over all users B in S.
The interestingness of P to A is now R(P)/W. If W=0, then also R(P)=0, and we define the interestingness as 0/0 := 0.
*/
	map<string, vector<pair<double, double> > > interestingness;
	set<Review*> other_reviews;
	double rp = 0;
	double w = 0;
	for (unsigned int i = 0; i < reviews.size(); i++)
	{
		// For all reviews that the logged in user has not reviewed
		if(user->getName() != reviews[i]->user)
		{
			other_reviews.insert(reviews[i]);
		}
	}
	set<Review*>::iterator ivt;
	double rating, simi;
	for (ivt = other_reviews.begin(); ivt != other_reviews.end(); ++ivt)
	{
		for (map<string, double>::iterator ibt = user_distance.begin(); ibt != user_distance.end(); ++ibt)
		{
			if (ibt->first == (*ivt)->user)
			{
				rating = (*ivt)->rating;
				simi = ibt->second;
				rp = (1 - simi) * rating;
				w = 1 - simi;
				pair<double, double> rpw = make_pair(rp, w);
				map<string, vector<pair<double, double> > >::iterator fi = interestingness.find((*ivt)->prodName);
				if (fi == interestingness.end())
				{
					vector<pair<double, double> > temp;
					temp.push_back(rpw);
					interestingness.insert(make_pair((*ivt)->prodName, temp));
				}
				else
				{
					fi->second.push_back(rpw);
				}

			}
		}
	}
	// Need to check for products that the user has not reviewed!
	double sum1 = 0;
	double sum2 = 0;
	double interest = 0;
	map<string, double> fat;
	for (map<string, vector<pair<double, double> > >::iterator it3 = interestingness.begin(); it3 != interestingness.end(); ++it3)
	{
		sum1 = 0;
		sum2 = 0;
		for (unsigned int i = 0; i < it3->second.size(); i++)
		{
			// rp
			sum1 += it3->second[i].first;
			// w
			sum2 += it3->second[i].second;
		}
		if (sum1 == 0 || sum2 == 0)
		{
			interest = 0;
		}
		else
		{
			interest = sum1/sum2;
		}
		fat.insert(make_pair(it3->first, interest));
	}
	vector<string> copy;
	// Take out products the user hasn't reviewed
	for (map<string, double>::iterator f = fat.begin(); f != fat.end(); ++f)
	{
		for (unsigned int i = 0; i < reviews.size(); i++)
		{
			if (reviews[i]->user == user->getName() && reviews[i]->prodName == f->first)
			{
				copy.push_back(f->first);
			}
		}
	}
	for (unsigned int i = 0; i < copy.size(); i++)
	{
		map<string, double>::iterator del = fat.find(copy[i]);
		fat.erase(del);
	}
	// Find max value in map with corresponding product

	typedef std::pair<std::string, double> MyPairType;
	struct CompareSecond
	{
	    bool operator()(const MyPairType& left, const MyPairType& right) const
	    {
	        return left.second < right.second;
	    }
	};
	std::pair<std::string, double> max = *max_element(fat.begin(), fat.end(), CompareSecond());
	
	// Recommend Product
	for (unsigned int i = 0; i < prods1.size(); i++)
	{
		if (num_user_revs == reviews.size() || num_user_revs == 0)
		{
			// Pop-up saying no more products to recommends
			QMessageBox* rec = new QMessageBox();
			rec->setWindowTitle("Recommendation");
			rec->setText("No more products to recommend!");
			rec->show();
		}
		else if (max.first == prods1[i]->getName())
		{
			string info = prods1[i]->displayString();
			QString qstr = QString::fromStdString(info);
			QMessageBox* rec = new QMessageBox();
			rec->setWindowTitle("Recommendation");
			rec->setText(qstr);
			rec->show();
		}
	}
}
MainWindow::~MainWindow()
{
	// Delete user*
	map<User*, deque<Product*> >::iterator iter1;
	for(iter1 = cartMap.begin(); iter1 != cartMap.end(); ++iter1)
	{
		User* u1 = iter1->first;
		delete u1;
	}
}
void MainWindow::andRadio()
{
	id = 0;
}
void MainWindow::orRadio()
{
	id = 1;
}
void MainWindow::saveFile()
{
	std::string filename = filenameInput->text().toStdString();
	df.db_dump(filename);
}
void MainWindow::addSearch()
{
	count++;
	// Do nothing if user left at least one input blank

	if(searchInput->text().isEmpty())
	{
		return;
	}

	// // Get form values
	// Search terms

	std::vector<std::string> terms;
	std::stringstream ss(searchInput->text().toStdString());
	std::string term;
	while(ss >> term){
			term = convToLower(term);
			terms.push_back(term);
	}
	// AND search
	if (id == 0)
	{
		hits = df.search(terms, id);
		
	}
	// OR search
	else 
	{
		hits = df.search(terms, id);
	}
	
	QString productText;

	for(std::vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it){
		productText += QString::fromStdString((*it)->getName());
		productText += "\n";
		productText += QString::number((*it)->getPrice());
		productText += "\n"; 	
		searchListWidget->addItem(productText);
		productText.clear();
	}

	if (count > 1)
	{
		sortList();
	}
	connect(searchListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(viewReview(QListWidgetItem*)));   
    // Clear form input

    searchInput->setText("");
    
}
void MainWindow::addtoCart()
{
	df.addCart(user->getName(), product_text, cartMap);

}
void MainWindow::viewtheCart()
{
	cartWin = new QWidget();
	cartWin->setWindowTitle("Cart");

	// Layout
	cartLayout = new QFormLayout();

	// List title
	listTitle = new QLabel("Items in your Cart");
	cartLayout->addRow(listTitle);

	// Cart List
	cart_list = new QListWidget();
	cartLayout->addRow(cart_list);
	User* u = df.viewCart(user->getName());
	map<User*, deque<Product*> >::iterator it1;
	it1 = cartMap.find(u);
	QString cart_items;
	if (it1 != cartMap.end())
	{
		for(deque<Product*>::iterator it2 = it1->second.begin(); it2
			!= it1->second.end(); ++it2)
		{
			cart_items += QString::fromStdString((*it2)->displayString());
			cart_list->addItem(cart_items);
			cart_items.clear();
		}
	}
	delt = cart_list->currentRow();
	connect(cart_list, SIGNAL(currentRowChanged(int)), this, SLOT(index(int)));
	

	// Delete Item Button
	deleteItem = new QPushButton("Delete");
	connect(deleteItem, SIGNAL(clicked()), this, SLOT(deletefromCart()));
	cartLayout->addRow(deleteItem);

	// Buy Cart Button
	buyCart = new QPushButton("Buy");
	connect(buyCart, SIGNAL(clicked()), this, SLOT(buytheCart()));
	cartLayout->addRow(buyCart);

	cartWin->setLayout(cartLayout);
	
	cartWin->show();
}
void MainWindow::buytheCart()
{
	df.buyCart(user->getName(), cartMap);
	cart_list->clear();
	User* u = df.viewCart(user->getName());
	map<User*, deque<Product*> >::iterator it1;
	it1 = cartMap.find(u);
	QString cart_items;
	if (it1 != cartMap.end())
	{
		for(deque<Product*>::iterator it2 = it1->second.begin(); it2
			!= it1->second.end(); ++it2)
		{
			cart_items += QString::fromStdString((*it2)->displayString());
			cart_list->addItem(cart_items);
			cart_items.clear();
		}
	}

}
void MainWindow::index(int row){
	delt = row;
}
void MainWindow::deletefromCart()
{
	deque<Product*> temp;
	map<User*, deque<Product*> >::iterator it3;
	User* u1 = df.viewCart(user->getName());
	it3 = cartMap.find(u1);
	temp = it3->second;

	deque<Product*>::iterator it5 = temp.begin();
	it5 += delt;
	temp.erase(it5);
	it3->second = temp;	

	cart_list->takeItem(cart_list->row(cart_list->currentItem()));

}
void MainWindow::addReview()
{
	revWin = new QWidget;
	revWin->setWindowTitle("Add Review");
	// Layout
	addReviewLayout = new QFormLayout;
	// Rating input
	ratingInput = new QLineEdit();
	addReviewLayout->addRow("Rating(1-5): ", ratingInput);

	// Date input
	dateInput = new QLineEdit();
	addReviewLayout->addRow("Date(YYYY-MM-DD): ", dateInput);

	// Text input
	textInput = new QLineEdit();
	addReviewLayout->addRow("Review Text: ", textInput);

	// Add Review Button
	addReviewButton = new QPushButton("Add Review and Return");
	connect(addReviewButton, SIGNAL(clicked()), this, SLOT(addHelper()));
	addReviewLayout->addRow(addReviewButton);

	revWin->setLayout(addReviewLayout);
	revWin->show();

}
void MainWindow::addHelper()
{
	if(ratingInput->text().isEmpty() || dateInput->text().isEmpty()
		|| textInput->text().isEmpty())
	{
		return;
	}

	// Grab rating
	int rate;

	rate = ratingInput->text().toInt();

	// Grab date
	std::string date;

	date = dateInput->text().toStdString();

	// Grab review text
	std::string rev_text;
	rev_text = textInput->text().toStdString();

	Review* r;

	string user1 = user->getName();
	r = new Review(product_text, rate, user1, date, rev_text);
	// Pass the datafunction to the mainwindow
	df.addReview(r);
	revs = df.getReview();
	sortReviews();
	revWin->close();
	
}
void MainWindow::viewReview(QListWidgetItem* item)
{
	reviewListWidget->clear();
	count1++;
	product_text = item->text().toStdString();
	// Amend the string so that only the product's name is visible
	for (unsigned int i = 0; i < product_text.size(); i++)
	{
		if(isdigit(product_text[i]))
		{
			product_text.erase(i--,product_text.size());
		}
	}
	product_text.erase(product_text.size()-1, 1);
	
	revs = df.getReview();
	
	std::vector<string> copy;
	// Convert date into integer
	for (unsigned int i = 0; i < revs.size(); i++)
	{
		copy.push_back(revs[i]->date);
	}
	for (unsigned int i = 0; i < copy.size(); i++)
	{
		std::string::iterator end_pos = std::remove(copy[i].begin(),
		copy[i].end(), '-');
		copy[i].erase(end_pos, copy[i].end());
	}
	std::vector<int> num_date;
	for (unsigned int i = 0; i < copy.size(); i++)
	{
		num_date.push_back(std::stoi(copy[i],nullptr,0));
	}
	// Set date int value in review to be the integers inside num_date
	for (unsigned int i = 0; i < revs.size(); i++)
	{
		revs[i]->dates = num_date[i];
	}
	struct DateComp {
	    bool operator()(Review* lhs, Review* rhs) 
	    { // Compare using alphanumerical
	      return lhs->dates > rhs->dates; 
	    }
  	};

  	DateComp comp;
  	mergeSort(revs, comp);
	QString review; 
	if (count == 1)
	{
		for (unsigned int i = 0; i < revs.size(); i++)
		{
			if (revs[i]->prodName == product_text)
			{
				review += "Rating: ";
				review += QString::number(revs[i]->rating);
				review += "\n" ;
				review += "User: ";
				review += QString::fromStdString(revs[i]->user);
				review += "\n";
				review += "Date: ";
				review += QString::fromStdString(revs[i]->date);
				review += "\n" ;
				review += "Review-Text: ";
				review += QString::fromStdString(revs[i]->reviewText);
				reviewListWidget->addItem(review);
				review.clear();
			}
		}
	}
	else
	{
		sortReviews();
	}

	
}
void MainWindow::sortReviews()
{
	reviewListWidget->clear();
	QString review;
	for (unsigned int i = 0; i < revs.size(); i++)
	{
		if (revs[i]->prodName == product_text)
		{
			review += "Rating: ";
			review += QString::number(revs[i]->rating);
			review += "\n" ;
			review += "User: ";
			review += QString::fromStdString(revs[i]->user);
			review += "\n";
			review += "Date: ";
			review += QString::fromStdString(revs[i]->date);
			review += "\n" ;
			review += "Review-Text: ";
			review += QString::fromStdString(revs[i]->reviewText);
			reviewListWidget->addItem(review);
			review.clear();		
		}
	}
}
void MainWindow::alphaSort()
{
	struct AlphaStrComp {
	    bool operator()(const Product* lhs, const Product* rhs) 
	    { // Compare using alphanumerical
	      return lhs->getName() < rhs->getName(); 
	    }
  	};
  	AlphaStrComp comp;
  	mergeSort(hits, comp);
  	sortList();
}

void MainWindow::sortList()
{
	searchListWidget->clear();
	QString productText;
	for(std::vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it){
		productText += QString::fromStdString((*it)->getName());
		productText += "\n";
		productText += QString::number((*it)->getPrice());
		productText += "\n"; 	
		searchListWidget->addItem(productText);
		productText.clear();
	
	}
}
void MainWindow::rateSort()
{
	// Iterate through the reviews for the particular product
	std::vector<Review*> revs1;
	revs1 = df.getReview();
	std::vector<Product*> all_prods;
	all_prods = df.getProduct();
	for (unsigned int i = 0; i < all_prods.size(); i++)
	{
		all_prods[i]->getRatings(revs1);
	}
	struct AvgRateComp {
		bool operator()(Product* lhs, Product* rhs)
		{
			return lhs->getRating() > rhs->getRating();
		}
	};
	AvgRateComp comp;
	mergeSort(hits, comp);
	sortList();
}
void MainWindow::quit()
{
	close();
}