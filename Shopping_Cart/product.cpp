#include <sstream>
#include <iomanip>
#include "product.h"

using namespace std;

Product::Product(const std::string category, const std::string name, double price, int qty) : 
  name_(name),
  price_(price),
  qty_(qty),
  category_(category)
{
  
}

Product::~Product()
{
  
}


double Product::getPrice() const
{
  return price_;
}

std::string Product::getName() const
{
  return name_;
}

void Product::subtractQty(int num)
{
  qty_ -= num;
}

int Product::getQty() const
{
  return qty_;
}
double Product::getRating()
{
  if (std::isnan(avg))
  {
    avg = 0;
  }
  return avg;
}
void Product::getRatings(vector<Review*> revs) 
{
  // If the product has no reviews
  vector<double> ratings;
  double sum = 0;
  if (revs.size() == 0)
  {
    avg = 0;
  }
  else
  {
    for (unsigned int i = 0; i < revs.size(); i++)
    {
      if (this->getName() == revs[i]->prodName)
      {
        ratings.push_back(revs[i]->rating);
      }
    }
    for (unsigned int i = 0; i < ratings.size(); i++)
    {
      sum += ratings[i];
    }
    avg = sum/ratings.size();
  }


}

/**
 * default implementation...can be overriden in a future
 * assignment
 */ 


void Product::dump(std::ostream& os) const
{
  os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ << endl;
}



