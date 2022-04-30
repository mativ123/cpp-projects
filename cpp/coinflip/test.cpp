#include <iostream>
#include <iterator>

int main()
{
    int array[] { 1, 2, 3 };
    int arrSize{ static_cast<int>(std::size(array)) };

    std::cout << arrSize;

    return 0;
}
