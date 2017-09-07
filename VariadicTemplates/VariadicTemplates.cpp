#include <algorithm>
#include <iostream>
#include <string>
#include <functional>
#include <vector>

// Variadic templates
// http://eli.thegreenplace.net/2014/variadic-templates-in-c/
// typename... Args is called a template parameter pack, 
// and Args... args is called a function parameter pack
// it's all about recurrence


template<typename T>
T adder(T value) {
	return value;
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
	return first + adder(args...);	// <- recursion
}

void TEST_Adder()
{
	long sum = adder(1, 2, 3, 4, 5, 6);
	std::string s1{ "this" }, s2{ " is" }, s3{ " bullshit" };
	std::string ssum = adder(s1, s2, s3);
}

//-----------------------------------------------------------------------------
// pattern matching:

template<typename T>
bool pair_comparer(T a, T b) {	// only even number of args
	return a == b;
}

template<typename T, typename... Args>
bool pair_comparer(T a, T b, Args... args) {
	return a == b && pair_comparer(args...);
}

template <typename T>
bool triple_comparer(T a)
{
	return false;
}

/*template <typename T>
bool triple_comparer(T a, T b)
{
	return false;
}
*/
template <typename T>
bool triple_comparer(T a, T b, T c)
{
	return a == b == c;
}

template <typename T, typename... Args>
bool triple_comparer(T a, T b, T c, Args... args)	// doesnt work as expected !?
{
	return (a == b == c) && triple_comparer( args...);
}

void TEST_PairComparer()
{
	//std::cout << __PRETTY_FUNCTION__ << std::endl;	// gcc extension

	bool result = pair_comparer(1, 1, 2, 2, 3, 3);
	bool result2 = pair_comparer(1, 1, 2, 3, 4, 5);
	bool result3 = triple_comparer(1, 1, 1, 2, 2, 2);
	std::cout << "Pairs 1 comparision: " << std::boolalpha << result << std::endl;
	std::cout << "Pairs 2 comparision: " << std::boolalpha << result2 << std::endl;
	std::cout << "Pairs 3 comparision: " << std::boolalpha << result3 << std::endl;
}

//-----------------------------------------------------------------------------
// https://en.wikipedia.org/wiki/Fibonacci_number

using uint = unsigned int;

template<typename T>
T fib(T n)
{
	return n;
}

template<typename T, typename... Args>
T fibonnaci(T n, Args... args)
{
	return n + fib(n, args...);
}

template<uint n>
struct Fibonacci
{
	enum // why enum!?
	{
		value = Fibonacci<n-1>::value + Fibonacci<n-2>::value
	};
	static void add_values(std::vector<uint>& v) {
		Fibonacci<n - 1>::add_values(v);
		v.push_back(value);
	}
};

// TODO
void TEST_Fibonacci()
{
	int n = 72;
	int f = fibonnaci(3);

	std::cout << "Fibonacci: " << f << std::endl;
}

//-----------------------------------------------------------------------------
// Variadic tuple

template <typename... Ts> 
struct tuple {};

template <typename T, typename... Ts>
struct tuple<T, Ts...> : tuple<Ts...> 
{
	tuple(T t, Ts... ts) 
		: tuple<Ts...>(ts...)
		, tail(t) 
	{}

	T tail;
};

template <class T, class... Ts>
struct elem_type_holder<0, tuple<T, Ts...>> {
	typedef T type;
};

template <size_t k, class T, class... Ts>
struct elem_type_holder<k, tuple<T, Ts...>> {
	typedef typename elem_type_holder<k - 1, tuple<Ts...>>::type type;
};

// k == 0, enable_if used for two cases
template <size_t k, class... Ts>
typename std::enable_if<
	k == 0, typename elem_type_holder<0, tuple<Ts...>>::type&>::type
get(tuple<Ts...>& t) 
{
	return t.tail;
}

// general case
template <size_t k, class T, class... Ts>
typename std::enable_if<
	k != 0, typename elem_type_holder<k, tuple<T, Ts...>>::type&>::type
get(tuple<T, Ts...>& t) 
{
	tuple<Ts...>& base = t;
	return get<k - 1>(base);
}

// get<1>(tuple) = 13.4f;
void TEST_Tuple()
{
	tuple<int, float, double, const char*> t1(7, 1.34, 43.23, "c");
	get<0> = 1;
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

void TEST_Adjacent()
{
	std::vector<int> vec{ 1, 2, 5, 5, 4, 5, 6, 7, 7 };
	auto r = std::adjacent_find(vec.begin(), vec.end()/*, [](const int a, const int b) { return a == b; }*/);
	std::cout << "Adjacent find: " << *r << " at: " << vec.at(*r) << std::endl;
	r = std::adjacent_find(vec.begin() + vec.at(*r), vec.end());
	std::cout << "Adjacent find: " << *r << " at: " << vec.at(*r) << std::endl;

}

int main()
{
	auto lambda = [val = 0]() mutable { return val++; };

	TEST_Adder();
	TEST_PairComparer();
	TEST_Fibonacci();
	TEST_Adjacent();

	return lambda();
}

