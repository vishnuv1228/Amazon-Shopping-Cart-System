#ifndef BOOK_H
#define BOOK_H

#include "product.h"
#include "util.h"


class Book : public Product {
public:
	Book(const std::string name, const std::string isbn, const std::string author_name,
		const std::string category, int qty, double price);
	std::set<std::string> keywords() const;
	std::string displayString() const;
	void dump(std::ostream& os) const;
private:
	std::string author_name_;
	std::string isbn_;
};
#endif