#include "book.h"
#include "util.h"
#include <string>
#include <set>
#include <sstream>
#include <iomanip>

using namespace std;

Book::Book(const std::string name, const std::string isbn, const std::string author_name,
	std::string category, int qty, double price) : Product(category,name, price, qty)
{
	isbn_ = isbn;
	author_name_ = author_name;
}
set<string> Book::keywords() const
{
	// Returns appropriate keywords: ISBN, Author, and title that product is associated with
	
	set<string> result = parseStringToWords(name_);
	
	set<string> result1 = parseStringToWords(author_name_);
	set<string> result2 = parseStringToWords(isbn_);
	set<string> uni = setUnion(result, result1);
	set<string> uni2 = setUnion(uni, result2);

	return uni2;
}
string Book::displayString() const
{
	stringstream s;
	s << std::setprecision(6) << price_;
	string price1;
	s >> price1;
	string qty1 = to_string(qty_);
	// String to display relevant information about the product
	string output = name_+ "\n" + "Author: " + author_name_ + " " + "ISBN: " +
	isbn_ + "\n" + price1 + " " + qty1 + " left";
	return output;
}
void Book::dump(ostream& os) const
{
	// Outputs information in database form
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << isbn_ << "\n" 
	<< author_name_ << "\n"; 
}