#ifndef DBPARSER_H
#define DBPARSER_H
#include <iostream>
#include <string>
#include <map>
#include "datastore.h"
#include "product.h"
#include "user.h"
#include "product_parser.h"

class DBParser {
 public:
  DBParser();
  virtual ~DBParser();
  void addProductParser(ProductParser* p);
  bool parse(std::string db_filename, DataStore& ds);

 protected:
  enum PState { INIT, PRODUCTS, FIND_USERS, READ_USERS, FIND_REVIEWS, READ_REVIEWS, DONE };

  Product* parseProduct(const std::string& category, 
			std::istream& is);



  virtual User* parseUser(std::istream& is);

  virtual Review* parseReview(std::istream& is, 
			      std::string prodname); 

  virtual Review* makeReview(std::string& prodname, 
			     int& rating, 
           std::string& user,
			     std::string& date, 
			     std::string& review_text);
 private:
  int lineno_;
  std::string errorMsg_;
  bool error_;
  std::map<std::string, ProductParser*> parsers_;

};

#endif
