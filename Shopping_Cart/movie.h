#ifndef MOVIE_H
#define MOVIE_H

#include "product.h"
#include "util.h"


class Movie : public Product {
public:
	Movie(const std::string name, const std::string genre, const std::string rating,
		const std::string category, int qty, double price);
	std::set<std::string> keywords() const;
	std::string displayString() const;
	void dump(std::ostream& os) const;
private:
	std::string genre_;
	std::string rating_;
};
#endif