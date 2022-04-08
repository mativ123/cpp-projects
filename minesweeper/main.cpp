#include <iostream>
#include <vector>
#include <random>
#include <ctime> // for std::time

#include "inc.h"

std::vector<std::vector<int>> sourroundMines(std::vector<std::vector<int>> boardVector, int width, int height)
{
    for(int i{0}; i<boardVector.size(); ++i)
    {
        for(int j{0}; j<boardVector[i].size(); ++j)
        {
            int mineCount{0};
            if(i==0 || i==width-1 || j==0 || j==height-1 || boardVector[i][j]==9)
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

                boardVector[i][j]=mineCount;
            }
        }
    }

    return boardVector;
}

int main()
{
    std::vector<std::vector<int>> balls {};

    std::cout << "width: ";
    int height{};
    std::cin >> height; 

    std::cout << "height: ";
    int width{};
    std::cin >> width; 

    std::cout << "mines: ";
    int mines{};
    std::cin >> mines; 
    while (mines > width*height || mines < 0)
    {
        std::cout << "Error: invalid number of mines-> " << mines << "\nmines: ";
        std::cin >> mines;
    }
    
    std::cout << width << 'x' << height << '\n';

    for (int x{0}; x<width; ++x)
    {
        balls.push_back(std::vector<int>());
        for (int y{0}; y<height; ++y)
        {
            balls[x].push_back(0);
        }
    }

    {
        std::vector<std::vector<int>> taken{};
        
        // Initialize our mersenne twister with a random seed based on the clock
        std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

        // Create a reusable random number generator that generates uniform numbers between 1 and 6
        std::uniform_int_distribution heightChance{ 0, height-1};
        std::uniform_int_distribution widthChance{ 0, width-1};

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

    std::cout << "mines: " << (static_cast<float>(mines)/(static_cast<float>(width)*static_cast<float>(height)))*100 << "%\n";

    balls = sourroundMines(balls, width, height);

    for (int i{0}; i<balls.size(); ++i)
    {
        for (int j{0}; j<balls[i].size(); ++j)
        {
            if(balls[i][j] == 9)
            {
                std::cout << "x, \t";
            } else
            {
                std::cout << balls[i][j] << ", \t";
            }
        }
        std::cout << '\n';
    }

    return 0;
}
