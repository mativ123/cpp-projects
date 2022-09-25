#include <ncurses.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    initscr();
    raw();
	keypad(stdscr, TRUE);
	noecho();

    bool running { true };
    std::string screenText { };
    while(running)
    {
        int ch = getch();
        if(ch == KEY_BACKSPACE && screenText.size() > 0)
        {
            screenText.pop_back();

            clear();
            printw(screenText.c_str());
            refresh();
        } else if(ch != 113)
        {
            screenText += ch;

            clear();
            printw(screenText.c_str());
            refresh();
        } else
            running = false;
    }
    endwin();

    return 0;
}
