#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"
#include <iostream>
using namespace std;
std::string convToLower(std::string src) 
{
  std::transform(src.begin(), src.end(), src.begin(), ::tolower);
  return src;
}

/** Complete the code to convert a string containing a rawWord 
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWord) 
{ 
	string newWord = convToLower(rawWord);
	stringstream iss;
	iss << newWord;
	string parse;
	set<string> convertWord;
	while(iss >> parse)
	{
		if(iss.fail())
		{
			break;
		}
		for (unsigned int i = 0; i < parse.size(); i++)
		{

			if(ispunct(parse[i]) && parse.size() < 15)
			{
				string s3 = parse.substr(0, i);
				// Erase the original string from the beginning up to the punctuation
				parse = parse.erase(0, s3.length()+1);
				// Insert strings into the set if it has more than 1 letter
				if (parse.size() > 1)
				{
					convertWord.insert(parse);
				}	
				if (s3.size() > 1)
				{
					convertWord.insert(s3);
				}	
			}
		}
		if (parse.size() > 1)
		{
			convertWord.insert(parse);
		}
		
	}
	return convertWord;
}