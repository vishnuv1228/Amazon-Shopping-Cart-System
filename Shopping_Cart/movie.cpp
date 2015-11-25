#include "movie.h"
#include "book.h"
#include "clothing.h"
#include "util.h"
#include <string>
#include <set>
#include <sstream>
#include <iomanip>
using namespace std;

Movie::Movie(const std::string name, const std::string genre, const std::string rating,
	std::string category, int qty, double price) : Product(category,name, price, qty)
{
	genre_ = genre;
	rating_ = rating;
}
set<string> Movie::keywords() const
{
	// Returns appropriate keywords: ISBN, Author, and title that product is associated with
	
	set<string> result = parseStringToWords(name_);
	set<string> result1 = parseStringToWords(genre_);
	set<string> result2 = parseStringToWords(rating_);
	set<string> uni = setUnion(result, result1);
	set<string> uni2 = setUnion(uni, result2);

	return uni2;
}
string Movie::displayString() const
{
	stringstream s;
	s << std::setprecision(6) << price_;
	string price1;
	s >> price1;
	string qty1 = to_string(qty_);
	// String to display relevant information about the product
	string output = name_+ "\n" + "Genre: " + genre_ + " " + "Rating: " +
	rating_ + "\n" + price1 + " " + qty1 + " left";
	return output;
}
void Movie::dump(ostream& os) const
{
	// Outputs information in database form
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << genre_ << "\n" 
	<< rating_ << "\n"; 
}