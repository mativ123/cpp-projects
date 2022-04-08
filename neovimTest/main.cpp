#include <string>
#include <iostream>
#include "print.cpp"

void print();

int main()
{
	std::cout << "im shaking and crying right now\n";
    std::cout << "ok bro\n";
    print();
    return 0;
}

void simpleAdder(int x, int y)
{
    std::cout << x << " + " << y << " = " << x+y << '\n';
}

void printText()
{
    std::string x{};
    std::cin >> x;

    std::cout << x;
}
