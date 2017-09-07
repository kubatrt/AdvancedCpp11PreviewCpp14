/*	C++14
	* Function return type deduction.
	* `decltype(auto)`.
	* Relaxed constexpr restrictions.
	* Variable templates.
	* Generic lambdas.


*/

#include <iostream>
#include <algorithm>
#include <tuple>
#include <functional>
#include <vector>
#include <string>

auto satan()
{
	return 666;
}

// template<typename T>   constexpr T pi {3.1415926535897932385};	// NO VARIABLE TEMPLATES IN VC++


template <class T>
T circural_area(T r)
{
	return 3.14 * r * r;
}

template<>
float circural_area(float r)
{
	return 3.14f * r * r;
}

// Predicate
template<typename T1, typename T2>
class Predicate
{
public:
	bool operator()(T1& t1, T2& t2) { return t1 < t2; }
};


template <typename T>
void print(const std::vector<T>& vec)
{
	for (const T& v : vec)
	{
		std::cout << v << ", ";
	}
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	static_assert(std::is_same<decltype(satan()), int>(), "this function does not return int");

	// std::function<bool(int, int)> pred = Procedure<int, int>();
	std::function<bool(int, int)> pred2 = [](int a, int b) { return a > b; };

	auto lambda = [](auto x) { return x*x; };
	static_assert(std::is_same<decltype(lambda(1.f)), float>(), "this is bullshit");

	std::vector<int> array1 = { 6, 3, 8, 6, 1 };
	std::vector<float> array2{ {9, 5, 3 } };
	std::vector<std::string> array3{ "this", "can", "be", "useful" };

	std::vector<int> vec(100); //vec.resize(100);
	std::fill(vec.begin(), vec.end(), 0);
	for (unsigned int i = 0; i < vec.size(); ++i)
	{
		vec[i] = i;
	}

	std::vector<int> evens;
	for_each(vec.begin(), vec.end(), 
		[&evens](int v) { if (v % 2 == 0) evens.push_back(v); }
	);

	print(evens);

	std::sort(std::begin(array1), std::end(array1), pred2);
	std::sort(std::begin(array2), std::end(array2), Predicate<float,float>());
	std::sort(std::begin(array3), std::end(array3), Predicate<std::string, std::string>());

	print(array1);
	print(array2);
	print(array3);

	return 0;
}