#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>

using namespace std;

int main()
{
	
	vector<int> vec{ 8, 3, 2, 4, 5, 6, 1, 7, 9 };
	
	int multi = 4;
	auto exp_by = [=](int& n) { n *= multi; };
	auto b = std::for_each(vec.begin(), vec.end(), exp_by);

	for(const auto& v : vec)
		std::cout << v << std::endl;

	//	std::transform example
	std::transform(vec.begin(), vec.end(), vec.begin(), 
		[](int n) -> int { return std::min(n, 20); });

	for (const auto& v : vec)
		std::cout << v << std::endl;

	std::string text{ "Welcome to modern c++" };
	std::transform(text.begin(), text.end(), text.begin(), 
		[](unsigned char c) -> unsigned char { return std::toupper(c); });
	std::cout << text << std::endl;
	
	return 0;
}