"""main runner module"""

import time
import unicurses


def main():
    """main function"""

    # MUST ASSIGN TO A VARIABLE NAME STDSCR
    stdscr = unicurses.initscr()
    unicurses.cbreak()
    unicurses.noecho()
    unicurses.curs_set(0)
    unicurses.keypad(stdscr, True)
    unicurses.addstr("Test 1 2 3")
    unicurses.refresh()
    time.sleep(3)
    unicurses.endwin()


if __name__ == "__main__":
    main()
