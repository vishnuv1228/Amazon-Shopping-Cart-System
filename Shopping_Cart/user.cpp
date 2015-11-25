#include "user.h"
using namespace std;

User::User() : name_("unknown"), age_(0), balance_(0.0), type_(1), password_("unknown")
{

}
User::User(std::string name, int age, double balance, int type, std::string password) : 
  name_(name), age_(age), balance_(balance), type_(type), password_(password)
{

}

User::~User()
{

}


int User::getAge() const
{
  return age_;
}

std::string User::getName() const
{
  return name_;
}

std::string User::getPassword() const
{
	return password_;
}

double User::getBalance() const
{
  return balance_;
}

void User::deductAmount(double amt)
{
  balance_ -= amt;
}

void User::dump(std::ostream& os)
{
  os << name_ << " " << age_ << " " << balance_ << " " << type_ <<  " " << password_ << endl;
}
