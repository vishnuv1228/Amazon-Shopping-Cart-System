#include "clothing.h"
#include "util.h"
#include <string>
#include <set>
#include <sstream>
#include <iomanip>
using namespace std;

Clothing::Clothing(const std::string name, const std::string brand, const std::string size,
	std::string category, int qty, double price) : Product(category,name, price, qty)
{
	brand_ = brand;
	size_ = size;
}
set<string> Clothing::keywords() const
{
	// Returns appropriate keywords: ISBN, Author, and title that product is associated with
	set<string> result = parseStringToWords(name_);
	set<string> result1 = parseStringToWords(brand_);
	set<string> result2 = parseStringToWords(size_);
	set<string> uni = setUnion(result, result1);
	set<string> uni2 = setUnion(uni, result2);

	return uni2;
}
string Clothing::displayString() const
{
	stringstream s;
	s << std::setprecision(6) << price_;
	string price1;
	s >> price1;
	string qty1 = to_string(qty_);
	// String to display relevant information about the product
	string output = name_+ "\n" + "Brand: " + brand_ + " " + "Size: " +
	size_ + "\n" + price1 + " " + qty1 + " left";
	return output;
}
void Clothing::dump(ostream& os) const
{
	// Outputs information in database form
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" << size_ << "\n" 
	<< brand_ << "\n"; 
}