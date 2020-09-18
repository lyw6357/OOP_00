#include <iostream>
class SetValue {
public:
	int x, y;
	void setX(int a);
	void setY(int b);
	int getX();
	int getY();
};

void SetValue::setX(int a) {
	x = a;
}

void SetValue::setY(int b) {
	y = b;
}

int SetValue::getX() {
	return x;
}

int SetValue::getY() {
	return y;
}

int main() {
	class SetValue obj;
	obj.setX(33);
	obj.setY(44);

	std::cout << "X = " << obj.getX() << " , Y = " << obj.getY() << std::endl;
	system("pause");
	return 0;
}