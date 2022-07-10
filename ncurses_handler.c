#include "ncurses_handler.h"

void nc_init(){
    initscr(); // Start curses mode
    raw(); // disable line buffering (get as soon as possible keyboard input)
    noecho(); // disable echoing of input
    keypad(stdscr, TRUE); // enable arrows
    start_color(); // allow colors

}

void print_centered(int row, const char* text){
    WINDOW * win = stdscr;
    size_t halfWin = win->_maxx/2;
    size_t halfText = strlen(text)/2;

    mvprintw(row, halfWin-halfText, text);
    refresh();
}