#include<iostream>
#include<string>

template<typename T>
auto sum(T x) {
	return x;
}

template<typename T, typename... Args>
auto sum(T x, Args... args) {
	return x+sum(args...);
}

int main() {
	auto x = sum(42.5, 10, 11.1f, 1234);
	std::cout << x;

	getchar();
}