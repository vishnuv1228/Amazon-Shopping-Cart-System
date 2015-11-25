#include <iostream>
#include <sstream>
#include "product_parser.h"
/** Add any other desired #includes below */
#include "book.h"
#include "clothing.h"
#include "movie.h"


using namespace std;

static std::string &trim(std::string &s);

ProductParser::ProductParser()
{

}


ProductParser::~ProductParser()
{
}


Product* ProductParser::parse(string category,
			      istream& is,
			      bool& error,
			      string& errorMsg,
			      int& lineno)
{
  (void) category;
  parseCommonProduct(is, error, errorMsg, lineno);
  if(error) return NULL;
  return parseSpecificProduct(category, is, error, errorMsg, lineno);
}

void ProductParser::parseCommonProduct(std::istream& is, 
				       bool& error,
				       string& errorMsg,
				       int& lineno)
  
{
  lineno++;
  string myline;
  getline(is, myline);
  myline = trim(myline);
  if(myline.size() == 0){
    error = true;
    errorMsg = "Unable to find a product name";
    return;
  }
  prodName_ = myline;
  
  lineno++;
  getline(is, myline);
  if(is.fail()){
    error = true;
    errorMsg = "Expected another line with the price";
    return;    
  }
  stringstream ss1(myline);
  ss1 >> price_;
  if( ss1.fail() ){
    error = true;
    errorMsg = "Unable to read price";
    return;
  }

  lineno++;
  getline(is, myline);
  if(is.fail()){
    error = true;
    errorMsg = "Expected another line with the quantity";
    return;    
  }
  stringstream ss2(myline);
  ss2 >> qty_;
  if( ss2.fail() ){
    error = true;
    errorMsg = "Unable to read quantity";
  }
}

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}


ProductBookParser::ProductBookParser() : ProductParser()
{
}

Product* ProductBookParser::parseSpecificProduct(std::string category,
						 std::istream& is,
						 bool& error,
						 std::string& errorMsg,
						 int& lineno)
{
  (void) category;
  lineno++;
  string myline;
  getline(is, myline);
  stringstream ss3(myline);
  ss3 >> isbn_;
  if(ss3.fail()){
    error = true;
    errorMsg = "Unable to read ISBN";
  }
  
  lineno++;
  getline(is, author_);
  if(is.fail()){
    error = true;
    errorMsg = "Unable to read author";
  }
#ifdef DEBUG
  cout << "Making product " << prodName_ << endl;
#endif
  if(error){
    return NULL;
  }
  return makeProduct();
    
}

std::string ProductBookParser::categoryID()
{
  return "book";
}


/**
 * Your job to fill in the code to create a new book product
 * using the data members in this class and the parent ProductParser class
 */
Product* ProductBookParser::makeProduct()
{
  string category_ = categoryID();
  new_books = new Book(prodName_, isbn_, author_, category_, qty_, price_);
  return new_books;
}




ProductClothingParser::ProductClothingParser()
{
}

Product* ProductClothingParser::parseSpecificProduct(std::string category,
						 std::istream& is,
						 bool& error,
						 std::string& errorMsg,
						 int& lineno)
{
  (void) category;
  lineno++;
  string myline;
  getline(is, myline);
  stringstream ss3(myline);
  ss3 >> size_;
  if(ss3.fail()){
    error = true;
    errorMsg = "Unable to read size";
  }
  
  lineno++;
  getline(is, brand_);
  if(is.fail()){
    error = true;
    errorMsg = "Unable to read brand";
  }
#ifdef DEBUG
  cout << "Making product " << prodName_ << endl;
#endif
  if(error){
    return NULL;
  }
  return makeProduct();
    
}

std::string ProductClothingParser::categoryID()
{
  return "clothing";
}


/**
 * Your job to fill in the code to create a new clothing product
 * using the data members in this class and the parent ProductParser class
 */
Product* ProductClothingParser::makeProduct()
{
  string category_ = categoryID();
  new_clothes = new Clothing(prodName_, brand_, size_, category_, qty_, price_);
  return new_clothes;
}



ProductMovieParser::ProductMovieParser()
{
}

Product* ProductMovieParser::parseSpecificProduct(std::string category,
						 std::istream& is,
						 bool& error,
						 std::string& errorMsg,
						 int& lineno)
{
  (void) category;
  lineno++;
  string myline;
  getline(is, myline);
  stringstream ss3(myline);
  ss3 >> genre_;
  if(ss3.fail()){
    error = true;
    errorMsg = "Unable to read genre";
  }
  
  lineno++;
  getline(is, myline);
  stringstream ss4(myline);
  ss4 >> rating_;
  if(ss4.fail()){
    error = true;
    errorMsg = "Unable to read rating";
  }
#ifdef DEBUG
  cout << "Making product " << prodName_ << endl;
#endif
  if(error){
    return NULL;
  }
  return makeProduct();
    
}

std::string ProductMovieParser::categoryID()
{
  return "movie";
}


/**
 * Your job to fill in the code to create a new movie product
 * using the data members in this class and the parent ProductParser class
 */
Product* ProductMovieParser::makeProduct()
{
  string category_ = categoryID();
  new_movie = new Movie(prodName_, genre_, rating_, category_, qty_, price_);
  return new_movie;
}
