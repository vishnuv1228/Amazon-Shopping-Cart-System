#ifndef MSORT_H
#define MSORT_H
#include <vector>
#include <iostream>
#include <string>

template <class T, class Comparator>
void mergeSort(std::vector<T>& myArray, Comparator comp)
{
	if (myArray.size() < 2)
	{
		return;
	}
	std::vector<T> first, second;
	for (unsigned int i = 0; i < myArray.size() / 2; i++)
	{
		first.push_back(myArray[i]);
	}
	for (unsigned int i = myArray.size() / 2; i < myArray.size(); i++)
	{
		second.push_back(myArray[i]);
	}
	mergeSort(first, comp);
	mergeSort(second, comp);
	msort(first, second, myArray, comp);
}
template <class T, class Comparator>
void msort(std::vector<T>& first, std::vector<T>& second, std::vector<T>& combo, Comparator comp)
{
	combo.clear();
	combo.reserve(first.size() + second.size());
	unsigned int firstPos = 0, secondPos = 0;
	while (firstPos < first.size() && secondPos < second.size())
	{
		if (comp(first[firstPos], second[secondPos]) == 1)
		{
			combo.push_back(first[firstPos]);
			++firstPos;
		}
		else
		{
			combo.push_back(second[secondPos]);
			++secondPos;
		}
	}
	while (firstPos < first.size())
	{
		combo.push_back(first[firstPos]);
		++firstPos;
	}
	while (secondPos < second.size())
	{
		combo.push_back(second[secondPos]);
		++secondPos;
	}
};

#endif