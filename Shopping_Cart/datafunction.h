#ifndef DATAFUNCTION_H
#define DATAFUNCTION_H
#include "datastore.h"
#include "util.h"
#include "user.h"
#include "splay_tree.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>

class DataFunction : public DataStore
{
public:
	DataFunction();
	~DataFunction();
	DataFunction(const DataFunction& other);
	void addProduct(Product* p);
	void addUser(User* u);
	std::vector<User*> getUser();
	std::vector<Product*> getProduct();
	std::vector<Review*> getReview();
	void addReview(Review* r);
	std::vector<Product*> search(std::vector<std::string>& terms, int type);
	void dump(std::ostream& ofile);
	void addCart(std::string username, std::string product_text, std::map<User*, std::deque<Product*> >& carMap);
	User* viewCart(std::string username);
	void buyCart(std:: string username, std::map<User*, std::deque<Product*> >& cartMap);
	void deleteItem(std::string user_name, std::string prod_name);
	void db_dump(std:: string db_name);
	std::set<Product*> union_helper(std::set<Product*> uni_set, std::vector<std::string>& terms);
private:
	// Container to store the products
	SplayTree<std::string, std::set<Product*> > ProductMap;
	// Container to store the users
	std::map<std::string, User*> UserMap;
	// Queue to maintain User's cart
	std::deque<Product*> cart;
	// Storage
	std::vector<Product*> storage;
	// Keywords string set (for deallocation purposes)
	std::set<std::string> keywords;
	// Store reviews
	std::vector<Review*> reviews;
};
#endif