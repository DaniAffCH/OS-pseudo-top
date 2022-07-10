#include "ncurses_handler.h"
#include <stdlib.h>
int main()
{	
    nc_init();

    init_pair(1, COLOR_BLACK, COLOR_MAGENTA);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    attron(COLOR_PAIR(1));

	print_centered(0, "PSEUDO TOP");

    attroff(COLOR_PAIR(1));

	getch();			/* Wait for user input */
	endwin();			/* End curses mode */

	return 0;
}