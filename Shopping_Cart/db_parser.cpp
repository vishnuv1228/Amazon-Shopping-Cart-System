#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "db_parser.h"
#include "review.h"

using namespace std;


DBParser::DBParser()
{
  lineno_ = 1;
  error_ = false;

}
DBParser::~DBParser()
{
  for(map<string, ProductParser*>::iterator it = parsers_.begin();
      it != parsers_.end();
      ++it){
    delete it->second;
  }
}

void DBParser::addProductParser(ProductParser* p)
{
  parsers_.insert(make_pair(p->categoryID(), p));
}

bool DBParser::parse(string db_filename, DataStore& ds)
{
  int numProds = 0;
  int numUsers = 0;
  int numReviews = 0;
#ifdef DEBUG
  cout << "Starting parsing" << endl;
#endif
  ifstream ifile(db_filename.c_str());
  if(ifile.fail()){
    return true;
  }
  lineno_ = 1;
  string line;
  PState state = INIT;
  while(!ifile.fail() && !error_){
    getline(ifile, line);
#ifdef DEBUG
    cout << "Line: " << line << endl;
#endif
    stringstream ss(line);
    string token;
    if(state == INIT){
      if( ss >> token ){
	if(token == "<products>"){
#ifdef DEBUG
	  cout << "Found product" << endl;
#endif
	  state = PRODUCTS;
	}
      }
    }
    else if( state == PRODUCTS ){
      if( ss >> token) {
	if( token == "</products>" ){
#ifdef DEBUG
	  cout << "Found /product" << endl;
#endif
	  state = FIND_USERS;
	}
	else {
	  Product* p = parseProduct(token, ifile);
	  if(!error_ && p != NULL){
	    ds.addProduct(p);
	    numProds++;
	  }
	}
      }
    }
    else if( state == FIND_USERS){
      if( ss >> token ){
	if(token == "<users>"){
	  state = READ_USERS;
#ifdef DEBUG
	  cout << "Found users" << endl;
#endif
	}
      }
    }
    else if( state == READ_USERS){
      if( ss >> token) {
	if( token == "</users>" ){
#ifdef DEBUG
	  cout << "Found /users" << endl;
#endif
	  state = FIND_REVIEWS;
	}
	else {
	  ss.clear();
	  ss.str(line);
	  User* u = parseUser(ss);
	  if(u){
	    ds.addUser(u);
	    numUsers++;
	  }
	}
      }
    }
    else if( state == FIND_REVIEWS){
      if( ss >> token ){
	if(token == "<reviews>"){
	  state = READ_REVIEWS;
#ifdef DEBUG
	  cout << "Found reviews" << endl;
#endif
	}
      }
    }
    else if( state == READ_REVIEWS){
      if( ss >> token) {
	if( token == "</reviews>" ){
#ifdef DEBUG
	  cout << "Found /reviews" << endl;
#endif
	  state = DONE;
	}
	else {
	  string prodName = token;
	  Review* r = parseReview(ifile, line);
	  if(r){
	    ds.addReview(r);
	    numReviews++;
	  }
	}
      }
    }
    
    if(error_){
      cerr << "Parse error on line " << lineno_ << ": " << errorMsg_ << endl;
    }
    lineno_++;
  }
  if(!error_){
    cout << "Successfully parsed " << numProds << " products, " <<
      numUsers << " users, and " << numReviews << " reviews." << endl;
  }
  return error_;
}



Product* DBParser::parseProduct(const string& category, 
			    istream& is)
{
  map<string,ProductParser*>::iterator it = parsers_.find(category);
  if(it != parsers_.end()){
    return it->second->parse(category, is, error_, errorMsg_, lineno_);
  }
  else {
    error_ = true;
    string msg = "No product parser available for category: ";
    errorMsg_ =  msg + category;
    return NULL;
  }
}

User* DBParser::parseUser(istream& is)
{
  string username;
  int age;
  double balance;
  int type;
  string password;
  is >> username;
  if( is.fail() ){
    error_ = true;
    errorMsg_ = "Unable to read username";
  }
  is >> age;
  if( is.fail() ){
    error_ = true;
    errorMsg_ = "Unable to read age";
  }
  is >> balance;
  if( is.fail() ){
    error_ = true;
    errorMsg_ = "Unable to read balance";
  }
  is >> type;
  if( is.fail() ){
    error_ = true;
    errorMsg_ = "Unable to read type";
  }
  is >> password;
  if (is.fail() ){
    error_ = true;
    errorMsg_ = "Unable to read password";
  }
  if(error_){
    return NULL;
  }
  else {
    return new User(username, age, balance, type, password);
  }
}

Review* DBParser::parseReview(istream& is, string prodname)
{
  int rating;
  string date;
  string user;
  string review_text;
  string line;
  is >> rating;
  if( is.fail() || (rating < 1 || rating > 5) ){
    error_ = true;
    errorMsg_ = "Unable to read rating";
  }
  is >> user;
  if( is.fail() ){
    error_ = true;
    errorMsg_ = "Unable to read user";
  }
  is >> date;
  if (is.fail()) {
    error_ = true;
    errorMsg_ = "Unable to read date";
  }
  // skip over whitespace but not newlines
  char c = is.peek();
  while(c == ' ' || c == '\t'){
    is.get();
    c = is.peek();
  }
  // get the review text
  getline(is, review_text);
  if( is.fail() ){
    error_ = true;
    errorMsg_ = "Unable to read review text";
  }
  lineno_++;
  if(error_){
    return NULL;
  }
  else {
    return makeReview(prodname, rating, user, date, review_text);
  }

}

Review* DBParser::makeReview(string& prodname, int& rating, string& user, string& date, string& review_text)
{
  return new Review(prodname, rating, user, date, review_text);
}


