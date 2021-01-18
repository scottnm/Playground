fn main() {
    let window = pancurses::initscr();
    pancurses::noecho();
    pancurses::cbreak();
    pancurses::curs_set(0);
    pancurses::set_title("test");

    while let Some(input) = window.getch() {
        dbg!(input);

        if input == pancurses::Input::Character('q') {
            break;
        }
    }
}
