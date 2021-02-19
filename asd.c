#define DELAY 5

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ncurses.h>
#include <time.h>

int main() {

    int child_status = -1;
    char symbol;
    pid_t pid = fork();

    noecho();

    initscr();
    curs_set(0);
    halfdelay(DELAY);

    switch(pid) {

        case -1: {

            printw("Child creating ERROR\n");
            getch();
            exit(0);
        }
        case 0: {

            WINDOW* childWindow = newwin(100, 100, 0, 0);
            wrefresh(childWindow);

            while(symbol != '\n') {
            
                time_t seconds = time(NULL);
                seconds += 60;
                struct tm* timeinfo = localtime(&seconds);
                
                wmove(childWindow, 7, 0);
                wprintw(childWindow, "Child (+1 minute): %s", asctime(timeinfo));
                wrefresh(childWindow);

                flushinp();
                symbol = getch();
            }

            delwin(childWindow);

            exit(0);
            break;
        }
        default: {
            while(true) {
            
                time_t seconds = time(NULL);
                struct tm* timeinfo = localtime(&seconds);

                move(5, 0);
                printw("Perant (+0 minute): %s", asctime(timeinfo));
                refresh();

                if(waitpid(pid, &child_status, WNOHANG) > 0)
                    break;

                napms(DELAY);
            }

            if (WIFEXITED(child_status)) {
                printw("Press key...");
            }
            else {
                printw("Chiled exiting ERROR");
            }

            refresh();
            flushinp();
            getch();

            break;
        }
    }

   endwin();

   return 0;
}