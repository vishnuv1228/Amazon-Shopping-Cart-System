#ifndef CLOTHING_H
#define CLOTHING_H

#include "product.h"
#include "util.h"


class Clothing : public Product {
public:
	Clothing(const std::string name, const std::string brand, const std::string size,
		const std::string category, int qty, double price);
	std::set<std::string> keywords() const;
	std::string displayString() const;
	void dump(std::ostream& os) const;
private:
	std::string brand_;
	std::string size_;
};
#endif