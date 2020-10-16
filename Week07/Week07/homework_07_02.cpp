#include <iostream>

bool funcA() {
    int c;
    std::cin >> c;
    if (c < 10) //Let's assume this is exception
        return false;
    return true;
}
int main() {
    if (funcA()){

    }
    else {
        std::cout << "Exception : Invalid Input!!" << std::endl;
    }
    return 0;
}
