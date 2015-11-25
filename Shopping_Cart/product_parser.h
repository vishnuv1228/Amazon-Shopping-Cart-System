#ifndef PRODUCT_PARSER_H
#define PRODUCT_PARSER_H
#include <string>
#include <iostream>
#include "product.h"

class ProductParser
{
 public:
  ProductParser();

  virtual ~ProductParser();
  

  Product* parse(std::string category,
		 std::istream& is,
		 bool& error,
		 std::string& errorMsg,
		 int& lineno);

  void parseCommonProduct(std::istream& is, 
			  bool& error,
			  std::string& errorMsg,
			  int& lineno);

  virtual Product* parseSpecificProduct(std::string category,
					std::istream& is,
					bool& error,
					std::string& errorMsg,
					int& lineno) = 0;
  
  virtual std::string categoryID() = 0;

 protected:
  virtual Product* makeProduct() = 0;
  
  std::string prodName_;
  double price_;
  int qty_;
  
};


class ProductBookParser : public ProductParser
{
 public:
  ProductBookParser();
  Product* parseSpecificProduct(std::string category,
				std::istream& is,
				bool& error,
				std::string& errorMsg,
				int& lineno);
  
  std::string categoryID();

 protected:
  Product* makeProduct();
  
 private:
  std::string isbn_;
  std::string author_;
  Product * new_books;
};


class ProductClothingParser : public ProductParser
{
 public:
  ProductClothingParser();
  Product* parseSpecificProduct(std::string category,
				std::istream& is,
				bool& error,
				std::string& errorMsg,
				int& lineno);
  
  std::string categoryID();

 protected:
  Product* makeProduct();
  
 private:
  std::string size_;
  std::string brand_;
  Product * new_clothes;
};


class ProductMovieParser : public ProductParser
{
 public:
  ProductMovieParser();
  Product* parseSpecificProduct(std::string category,
				std::istream& is,
				bool& error,
				std::string& errorMsg,
				int& lineno);
  
  std::string categoryID();

 protected:
  Product* makeProduct();
  
 private:
  std::string genre_;
  std::string rating_;
  Product * new_movie;
};

#endif
