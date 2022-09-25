#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char *argv[])
{
    std::ifstream file;
    file.open("obelLectio.jpg");

    char str[256];

    if(file.is_open())
    {
        file.getline(str, 50);

        std::cout << str << '\n';
    }
    else
        std::cout << "din mor\n";

    
    return 0;
}
