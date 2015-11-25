#ifndef REVIEW_H
#define REVIEW_H
#include <string>

/**
 * Models a review for the product identified by prodName
 */
struct Review
{
  Review() :  prodName(), rating(), user(), date(), reviewText()
  { }
  Review(std::string& prod, 
	 int& rate,
   std::string& u,
	 std::string& d, 
	 std::string& review_text) :
    prodName(prod), 
    rating(rate),
    user(u),
    date(d), 
    reviewText(review_text)
  { }

  std::string prodName;
  int rating;
  int average;
  std::string user;
  std::string date;
  int dates;
  std::string reviewText;
};

#endif
