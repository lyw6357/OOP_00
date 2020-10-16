#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class AnyString {
	std::string anyString;

public:
	AnyString(const std::string& anyString) : anyString(anyString) {}
	std::string getAnyString() {
		return "Stored String :: " + anyString;
	}
	friend ostream& operator<<(ostream& os, const AnyString& as);
};

ostream& operator<<(ostream& os, const AnyString& a) {
	os << "Stored String :: " << a.anyString;
	return os;
}

int main() {
	std::ofstream out("testOveroding.txt");
	AnyString a("Hello, this is operator overloading test!!!");
	cout << a;

	return 0;
}