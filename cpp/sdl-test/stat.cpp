#include <iostream>
#include <string>
#include <random>
#include <ctime> // for std::time
#include <algorithm>
#include <iterator>
#include <array>
#include <vector>
#include <sstream>

bool isCommand(std::string x, std::vector<std::string> commands);
void printHelp();

int CoinFlip(int argc, char* argv[])
{
    if(argc > 3)
    {
        printHelp();
        return 0;
    } else if(argc <= 1)
    {
        printHelp();
        return 0;
    }

    std::vector<std::string> commands { "--help", "--tails", "--heads" };
    std::vector<std::string> argsString;

    bool tails{};
    int percent{};

    for(int i{1}; i<argc; ++i)
    {
        argsString.push_back(argv[i]);
    }

    if(isCommand(argsString[0], commands))
    {
        if(argsString[0] == "--help")
        {
            printHelp();
            return 0;
        } else if(argsString[0] == "--tails")
        {
            tails = true;
        } else if(argsString[0] == "--heads")
        {
            tails = false;
        } else
        {
            printHelp();
        }

        std::stringstream convert{ argsString[1] };
        if(!(convert >> percent))
        {
            std::cout << "error: " << argsString[1] << " is invalid percantage\n";
            return 0;
        }
        if(percent < 0 || percent > 100)
        {
            std::cout << "error: " << argsString[1] << " is invalid percantage\n";
            return 0;
        }
    } else
    {
        printHelp();
        return 0;
    }

    if(tails)
    {
        std::cout << "tails percantage is: " << percent << '\n';
    } else
    {
        std::cout << "heads percantage is: " << percent << '\n';
    }
    
    // Initialize our mersenne twister with a random seed based on the clock
    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    // Create a reusable random number generator that generates uniform numbers between 1 and 6
    std::uniform_int_distribution tailChance{ 1, 100};

    int outcome = tailChance(mersenne);
    if(outcome > percent && tails)
    {
        std::cout << "heads won!\n";
    } else if(tails)
    {
        std::cout << "tails won!\n";
    }

    if(outcome > percent && !tails)
    {
        std::cout << "tails won!\n";
    } else if(!tails)
    {
        std::cout << "heads won!\n";
    }
    return 0;
}

bool isCommand(std::string x, std::vector<std::string> commands)
{
    for(std::string i : commands)
    {
        if(i == x)
        {
            return true;
        }
    }
    return false;
}

void printHelp()
{
    std::string helpString { "Play Heads or Tails with predefined chances\n\n --help \t\tPrint this text\n --heads n \t\tChance of hitting heads in percent\n --tails n \t\tChance of hitting tails in percent\n\nOnly input one" };

    std::cout << helpString << '\n';
}
