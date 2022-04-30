#include <iostream>
#include <vector>
#include <random>
#include <ctime> // for std::time

#include "inc.h"

std::vector<int> convertToList(std::string s, std::string delimiter)
{
    std::vector<int> result;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(stoi(token));
        s.erase(0, pos + delimiter.length());
    }
    result.push_back(stoi(s));

    return result;
}

std::vector<std::vector<int>> sourroundMines(std::vector<std::vector<int>> boardVector, int width, int height)
{
    for(int i{0}; i<boardVector.size(); ++i)
    {
        for(int j{0}; j<boardVector[i].size(); ++j)
        {
            int mineCount{0};
            if(i==0 || i==width || j==0 || j==height || boardVector[i][j]==9)
            {
                continue;
            } else
            {
                if(boardVector[i-1][j-1]==9)
                {
                    ++mineCount;
                }
                if(boardVector[i-1][j]==9)
                {
                    ++mineCount;
                }
                if(boardVector[i-1][j+1]==9)
                {
                    ++mineCount;
                }
                if(boardVector[i][j-1]==9)
                {
                    ++mineCount;
                }
                if(boardVector[i][j+1]==9)
                {
                    ++mineCount;
                }
                if(boardVector[i+1][j-1]==9)
                {
                    ++mineCount;
                }
                if(boardVector[i+1][j]==9)
                {
                    ++mineCount;
                }
                if(boardVector[i+1][j+1]==9)
                {
                    ++mineCount;
                }
            }
            boardVector[i][j]=mineCount;
        }
    }

    return boardVector;
}

void printBoard(std::vector<std::vector<int>> balls, int mines, int width, int height, std::vector<std::vector<bool>> revealed)
{
    std::cout << "mines: " << (static_cast<float>(mines)/(static_cast<float>(width)*static_cast<float>(height)))*100 << "%\n";

    balls = sourroundMines(balls, width, height);

    for(int i{0}; i<height; ++i)
    {
        if(i==0)
        {
            std::cout << "\t";
        }
        std::cout << i << ", \t";
        if(i==height-1)
        {
            std::cout << '\n';
        }
    }

    for (int i{0}; i<balls.size(); ++i)
    {
        std::cout << i << ", \t";
        for (int j{0}; j<balls[i].size(); ++j)
        {
            //if(revealed[i][j])
            //{
                if(balls[i][j] == 9)
                {
                    std::cout << "\033[31mx, \033[0m\t";
                } else if(balls[i][j] == 0)
                {
                    std::cout << "\033[32m" << balls[i][j] << ", \033[0m\t";
                } else if(balls[i][j] == 1)
                {
                    std::cout << "\033[33m" << balls[i][j] << ", \033[0m\t";
                } else if(balls[i][j] == 2)
                {
                    std::cout << "\033[34m" << balls[i][j] << ", \033[0m\t";
                } else if(balls[i][j] == 3)
                {
                    std::cout << "\033[35m" << balls[i][j] << ", \033[0m\t";
                } else if(balls[i][j] == 4)
                {
                    std::cout << "\033[35m" << balls[i][j] << ", \033[0m\t";
                } else if(balls[i][j] == 5)
                {
                    std::cout << "\033[36m" << balls[i][j] << ", \033[0m\t";
                } else if(balls[i][j] == 6)
                {
                    std::cout << "\033[93m" << balls[i][j] << ", \033[0m\t";
                } else if(balls[i][j] == 7)
                {
                    std::cout << "\033[95m" << balls[i][j] << ", \033[0m\t";
                } else if(balls[i][j] == 8)
                {
                    std::cout << "\033[31m" << balls[i][j] << ", \033[0m\t";
                }
            //} else
            //{
                //std::cout << "H, \t";
            //}
        }
        std::cout << '\n';
    }
}

std::vector<std::vector<bool>> openFree(std::vector<std::vector<int>> balls, std::vector<std::vector<bool>> revealed, std::vector<int> revealPos)
{
    std::cout << "balls";

    return revealed;
}

int main()
{
    int height{};
    int width{};
    int mines{};
    std::vector<std::vector<int>> balls {};
    std::vector<std::vector<bool>> revealed {};

    std::cout << "[1]Small: 8x8, 10 mines | [2]Medium: 16x16, 40 mines | [3]Big: 30x16, 99 mines | [4]Custom\nChoose board: ";
    int choice{0};
    while(true)
    {
        std::cin >> choice;
        switch(choice)
        {
            case(1):
                height = 8;
                width = 8;
                mines = 10;
                break;
            case(2):
                height = 16;
                width = 16;
                mines = 40;
                break;
            case(3):
                height = 30;
                width = 16;
                mines = 99;
                break;
            case(4):
                std::cout << "width: ";
                std::cin >> height; 

                std::cout << "height: ";
                std::cin >> width; 

                std::cout << "mines: ";
                std::cin >> mines; 
                break;
            default:
                continue;
        }
        break;
    }
    while (mines > width*height || mines < 0)
    {
        std::cout << "Error: invalid number of mines-> " << mines << "\nmines: ";
        std::cin >> mines;
    }
    
    std::cout << width << 'x' << height << '\n';

    for (int x{0}; x<width+1; ++x)
    {
        balls.push_back(std::vector<int>());
        revealed.push_back(std::vector<bool>());
        for (int y{0}; y<height+1; ++y)
        {
            balls[x].push_back(0);
            revealed[x].push_back(false);
        }
    }

    {
        std::vector<std::vector<int>> taken{};
        
        // Initialize our mersenne twister with a random seed based on the clock
        std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

        // Create a reusable random number generator that generates uniform numbers between 1 and 6
        std::uniform_int_distribution heightChance{ 1, height-1};
        std::uniform_int_distribution widthChance{ 1, width-1};

        for (int i{0}; i<mines; ++i)
        {

            int x{};
            int y{};
            bool already{true};
            while (already)
            {
                x = widthChance(mersenne);
                y = heightChance(mersenne);
                if(taken.size()==0)
                {
                    already = false;
                }
                for (int c{0}; c<taken.size(); ++c)
                {
                    if(x==taken[c][0] && y==taken[c][1])
                    {
                        already = true;
                        break;
                    } else
                    {
                        already = false;

                    }
                }
            }

            balls[x][y] = 9;
            taken.push_back(std::vector<int>());
            taken[i].push_back(x);
            taken[i].push_back(y);
        }
    }

    printBoard(balls, mines, width, height, revealed);

    bool running{true};
    while(running)
    {
        std::cout << "Format: x,y\nPosition to be revealed: ";
        std::string revealPosInput{};
        std::getline(std::cin >> std::ws, revealPosInput);
        std::string delimiter = ",";
        std::vector<int> revealPos = convertToList(revealPosInput, delimiter);
        revealed[revealPos[0]][revealPos[1]] = true;

        if(balls[revealPos[0]][revealPos[1]] == 9)
        {
            std::cout << "You picked a mine and died!!!!\n";
            for(int x = 0; x<balls.size(); ++x)
            {
                for(int y = 0; y<balls[x].size(); ++y)
                {
                    revealed[x][y] = true;
                }
            }
            running = false;
        }
        printBoard(balls, mines, width, height, revealed);
    }

    return 0;
}

