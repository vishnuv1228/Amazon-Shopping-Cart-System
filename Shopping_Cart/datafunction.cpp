#include "datafunction.h"
#include "product.h"
#include "datastore.h"
#include "util.h"
#include "db_parser.h"
#include "review.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <map>


using namespace std;

DataFunction::DataFunction()
{

}
DataFunction::~DataFunction()
{
	// Delete product*
	for (unsigned int i = 0; i < storage.size(); i++)
	{
		Product * p1 = storage[i];
		delete p1;
	}
	// Delete review*
	for (unsigned int i = 0; i < reviews.size(); i++)
	{
		Review* r = reviews[i];
		delete r;
	}
}
void DataFunction::addProduct(Product* p)
{
	// Declare a set of keywords containing all keywords for the products
	keywords = p->keywords();
	set<string>::iterator it;
	for (it = keywords.begin(); it != keywords.end(); ++it)
	{
		// If the product is not found, add the set of product* to the map
		if (ProductMap.find(*it) == ProductMap.end())
		{
			// Create a set of product
			set<Product*> product_set;
			product_set.insert(p);
			// Insert the keyword with the product to the ProductMap
			ProductMap.insert(std::make_pair(*it, product_set));
		}
		else
		{
			// If it is found, add it to the set of product*
			SplayTree<string, set<Product*> >::iterator it1 = ProductMap.find(*it);
			it1->second.insert(p);
		}
	}
	storage.push_back(p);
}
vector<Product*> DataFunction::getProduct()
{
	return storage;
}
void DataFunction::addUser(User *u)
{
	// Find the name of the user and store it in the map with the user pointer
	string user_name = u->getName();
	UserMap.insert(std::make_pair(user_name, u));

}
vector<User*> DataFunction::getUser()
{
	vector<User*> users;
	map<string, User*>::iterator it3;
	for (it3 = UserMap.begin(); it3 != UserMap.end(); ++it3)
	{
		users.push_back(it3->second);
	}
	return users;
}

void DataFunction::addReview(Review *r)
{
	for(unsigned int i = 0; i < storage.size(); i++)
	{
		if (r->prodName == storage[i]->getName())
		{
			reviews.push_back(r);
		}
	}	
	
}
vector<Review*> DataFunction::getReview()
{
	return reviews;
}
vector<Product*> DataFunction::search(vector<string>& terms, int type)
{
	// Declare vector for return
	vector<Product*> product;
	// Iterate through the vector of input strings
	for (unsigned int i = 0; i < terms.size();i++)
	{
		SplayTree<string, set<Product*> >::iterator ite = ProductMap.find(terms[i]);
		// If the input is found in the product map
			
			// If the input contained "AND" or "OR", but had only 1 input
			if ((type == 0 && terms.size() == 1) || (type == 1 && terms.size() == 1))
			{				
				if(ite != ProductMap.end())
				{
					// Create a new vector to copy the contents of the set
					vector<Product*> output(ite->second.begin(), ite->second.end()); 
					// Append the output vector to the product vector
					product.insert(product.end(), output.begin(), output.end());
				}
			}
			else if (type == 0 && terms.size() > 1)
			{
				if(ite == ProductMap.end())
				{
					break;
				}
				int counter = i;
				std::set<Product*> inter_set;
				if (counter == 0)
				{
					SplayTree<string, set<Product*> >::iterator its = ProductMap.find(terms[counter + 1]);
					// Intersection of terms
					// find the intersection between found terms
					inter_set = setIntersection(ite->second, its->second);
					vector<Product*> output(inter_set.begin(), inter_set.end()); 
					product.insert(product.end(), output.begin(), output.end());
					counter++;
				}
				else
				{
					// Copy the intersection of the first 2 sets over
					set<Product*> new_set = inter_set;
					// Make a new set for finding the intersection
					set<Product*> result;
					// While the counter is not equal to 1 less than the size of the input
					// vector, see if the next term is in the map 
					int size = terms.size()-1;
					while (counter != size)
					{
						SplayTree<string, set<Product*> >::iterator its1 = ProductMap.find(terms[counter+1]);
						if (its1 != ProductMap.end())
						{
							result = setIntersection(new_set,its1->second);
							new_set.clear();
							new_set = result;
							counter++;
						}
						else
						{
							// Return an empty vector
							vector<Product*> temp;
							return temp;
						}
					}
					vector<Product*> output1(result.begin(), result.end()); 
					product.insert(product.end(), output1.begin(), output1.end());
				}
			}
			else if (type == 1 && terms.size() > 1)
			{
				int counter = i;
				// Make a new set for finding the union
				std::set<Product*> uni_set;
				if (ite != ProductMap.end())
				{
					if (counter == 0)
					{
						SplayTree<string, set<Product*> >::iterator its = ProductMap.find(terms[counter + 1]);
						if (its != ProductMap.end())
						{
							// find the union between found terms
							uni_set = setUnion(ite->second, its->second);
							if (terms.size() > 2)
							{
								set<Product*> result1 = union_helper(uni_set, terms);
								vector<Product*> output1(result1.begin(), result1.end()); 
								product.insert(product.end(), output1.begin(), output1.end());
							}
							else
							{
								vector<Product*> output1(uni_set.begin(), uni_set.end()); 
								product.insert(product.end(), output1.begin(), output1.end());
							}
							
						}
						else
						{
							set<Product*> empty;
							uni_set = setUnion(ite->second, empty);
							if (terms.size() > 2)
							{
								set<Product*> result1 = union_helper(uni_set, terms);
								vector<Product*> output1(result1.begin(), result1.end()); 
								product.insert(product.end(), output1.begin(), output1.end());
							}
							else
							{
								vector<Product*> output1(uni_set.begin(), uni_set.end()); 
								product.insert(product.end(), output1.begin(), output1.end());
							}

						}
					}
				}
				else
				{
					if (counter == 0)
					{
						SplayTree<string, set<Product*> >::iterator its = ProductMap.find(terms[counter + 1]);
						if (its != ProductMap.end())
						{
							set<Product*> empty3;
							// find the union between found terms
							uni_set = setUnion(empty3, its->second);
							if (terms.size() > 2)
							{
								set<Product*> result1 = union_helper(uni_set, terms);
								vector<Product*> output1(result1.begin(), result1.end()); 
								product.insert(product.end(), output1.begin(), output1.end());
							}
							else
							{
								vector<Product*> output1(uni_set.begin(), uni_set.end()); 
								product.insert(product.end(), output1.begin(), output1.end());
							}
							
						}
					}

				}
			}
	
		}
	return product;
}
set<Product*> DataFunction::union_helper(set<Product*> uni_set, vector<string>& terms)
{
	int size = terms.size()-1;
	int counter = 1;
	set<Product*> result1;
	while (counter != size)
	{
		SplayTree<string, set<Product*> >::iterator its1 = ProductMap.find(terms[counter+1]);
		if (its1 != ProductMap.end())
		{
			// If the product* of the two sets are the same, only output one
			result1= setUnion(uni_set,its1->second);
			uni_set.clear();
			uni_set = result1;
			counter++;
		}
		else
		{
			set<Product*> empty2;
			result1 = setUnion(uni_set, empty2);
			uni_set = result1;
			counter++;
		}
	}
	return result1;
}
void DataFunction::addCart(string username, string product_text, map<User*, deque<Product*> >& carMap)
{
	// This function should update the user's cart with the index of the product
	// It also keeps track of the product information associated with the index

	// Find the user object using the username
	std::map<std::string, User*>::iterator it;
	it = UserMap.find(username);
	// Update the CartMap to correlate username to hit
	// If the user is found
	if (it != UserMap.end())
	{
		map<User*, deque<Product*> >::iterator it1;
		it1 = carMap.find(it->second);
		// If the User is not found in the map
		if(it1 == carMap.end())
		{
			// If the user's cart is empty, add the item to the cart
			for (unsigned int i = 0; i < storage.size(); i++)
			{
				// If the hit's name is the same as the selected product
				if (storage[i]->getName() == product_text)
				{
					cart.push_back(storage[i]);
					carMap.insert(std::make_pair(it->second, cart));
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < storage.size(); i++)
			{
				if (storage[i]->getName() == product_text)
				{
					// Update the cart's hit number if the user already exists in the UserCartMap
					it1->second.push_back(storage[i]);
				}
			}
		}
	}
	cart.clear();
}
void DataFunction::buyCart(string username, map<User*, deque<Product*> >& cartMap)
{
	// Find the user object using the username
	std::map<std::string, User*>::iterator it;
	it = UserMap.find(username);
	// Find the user's cart
	std::map<User*, deque<Product*> >::iterator it1;
	it1 = cartMap.find(it->second);

	if (it1 == cartMap.end())
	{
		return;
	}

	deque<Product*>::iterator it2;

	// Create a copy of the deque
	deque<Product*> copy;
	
	for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
	{
		if ((*it2)->getQty() > 0 && (*it2)->getPrice() <= it->second->getBalance())
		{
			// Subtract the quantity - ISSUE!
			Product * p = *it2;
			p->subtractQty(1);	
			
			// Deduct the amount from the user's balance
			it->second->deductAmount((*it2)->getPrice());
		}
		else
		{
			// Insert the products the user cannot buy into the copy deque
			copy.push_back(*it2);
		}
	}
	// Overwrite it with the copy's contents
	it1->second = copy;

		
}
User* DataFunction::viewCart(string username)
{
	/// Find the user object using the username
	std::map<std::string, User*>::iterator it;
	it = UserMap.find(username);
	
	return it->second; 
	
		
}
void DataFunction::db_dump(string db_name)
{
	// Call dump in this function
	
	// Open the file
	ofstream ofile(db_name.c_str());
	// Call dump
	dump(ofile);
	// Close the file
	ofile.close();

}
void DataFunction::dump(std::ostream& ofile)
{
	// Write updated contents in database form

	// Call product-dump 
	ofile << "Products: " << endl;
	for (unsigned int i = 0; i < storage.size(); i++)
	{
		storage[i]-> dump(ofile);
	}
	ofile << endl;
	// Output user contents
	map<string, User*>::iterator iter;
	ofile << "Users: " << endl;
	for (iter = UserMap.begin(); iter != UserMap.end(); ++iter)
	{
		iter->second->dump(ofile);
	}
	// Output reviews
	ofile << "Reviews:" << endl;

	for (unsigned int j = 0; j < reviews.size(); j++)
	{
		ofile << reviews[j]->prodName << endl;
		ofile << reviews[j]->rating << " " << reviews[j]->date << endl;
		ofile << reviews[j]->reviewText << endl;
	}
}