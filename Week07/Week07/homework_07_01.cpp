#include <fstream>
#include <iostream>
#include <string>

class AnyString {
	std::string anyString;

public:
	AnyString(const std::string& anyString) : anyString(anyString) {}
	std::string getAnyString() {
		return "Stored String :: " + anyString;
	}
};

std::ostream& operator<<(std::ostream& os, AnyString& a) {
	os << a.getAnyString();
	return os;
}

int main() {
	std::ofstream out("testOveroding.txt");
	AnyString a("Hello, this is operator overloading test!!!");
	out << a << std::endl;

	return 0;
}