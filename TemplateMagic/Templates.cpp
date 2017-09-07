#include <functional>
#include <tuple>
#include <iostream>
#include <cassert>
//#include <variant> not supported in VS 2015

struct call_by
{
	void l_value() &
	{
		std::cout << "L-value\n";
	}

	void r_value() &&
	{
		std::cout << "R-value\n";
	}
};

template <class... T>
void foo(T&&... t)
{
	//    t.print()...; // we cannot do it, so instead we do what's in the line below
	[[maybe_unused]] int x[] = { (std::forward<T>(t).print(), 0)... };
}

//-----------------------------------------------------------------------------
// type traits


template<typename T, typename U, typename V>
struct some_trait : std::false_type {};

template<typename T, typename U>
struct some_trait<T, U, U> : std::true_type {};

template<typename T, typename V>
struct some_trait<T, T, V> : std::true_type {};

template<typename T>
void foo(const T&);

//template<typename T, typename U> void foo<std::variant<T, U>>(const std::variant<T, U> &); 
template<> void foo<int>(const int &);
template<typename T> void foo(T &&);
//void foo<int>(const int &);
// template<typename T, typename U> void foo(const std::variant<T, U> &); // NO C++17!

//-----------------------------------------------------------------------------
// "Is same" implementation

template<class T, class U>
struct is_same : std::false_type {};

template<class T>
struct is_same<T, T> : std::true_type {};

template <class A, class B>
constexpr bool isSame(A, B)
{
	//std::cout << "they are NOT same" << std::endl;
	return false;
}

template <class A>
constexpr bool isSame(A, A)
{
	//std::cout << "they are same" << std::endl;
	return true;
}

struct X
{
	int x;
};

struct Y
{
	int y;
};

struct Z : Y
{
	int z;
};

//-----------------------------------------------------------------------------

int main()
{
	X x{};
	Y y{};
	std::cout << "Is same: " << std::boolalpha << is_same<decltype(x), decltype(y)>() << std::endl;
	static_assert(is_same<decltype(x), decltype(x)>() , "GTFO!");

	bool same = isSame(x, y);// is_same<typeid(x).hash_code(),typeid(x).hash_code()>();
	//static_assert(isSame(x, x), "");
	std::cout << std::boolalpha << same << std::endl;

	/*C c {};

	std::apply([](auto&&... args)
	{
	return f(std::forward<decltype(args)>(args)...);
	}, std::make_tuple(C{}, std::ref(c), c));
	*/
	/*
	std::cout << std::boolalpha << std::is_same<std::remove_const_t<const int &>, int>::value << std::endl;
	std::cout << std::boolalpha <<std::is_same<std::decay_t<const int &>, int>::value << std::endl;
	std::cout << std::boolalpha <<    std::is_same<std::remove_cv_t<const int>, int>::value << std::endl;
	std::cout << std::boolalpha <<  std::is_same<std::remove_const_t<int>, int>::value << std::endl;
	std::cout << std::boolalpha <<    std::is_same<std::remove_reference_t<std::remove_cv_t<const int &>>, int>::value << std::endl;
	*/
}