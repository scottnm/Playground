fn main()
{
    let mut board = [[0; 9]; 9];
    fill_with_solvable_board(&mut board);
    println!("Start board!");
    print_board(&board);
    println!("");

    let solvable = sudoku_solver(&mut board, (0, 0));
    if solvable
    {
        println!("Solved!\n");
        println!("Solved board:");
        print_board(&board);
    }
    else
    {
        println!("Unsolvable!");
        print_board(&board);
    }
}

fn sudoku_solver (board : &mut[[i8; 9]; 9],
                  cell : (usize, usize)) -> bool
{
    let next_cell = get_next_cell(board, cell);
    for cell_value in 1..10
    {
        board[cell.0][cell.1] = cell_value;
        if is_valid_board(board, cell, (cell.0 / 3, cell.1 / 3))
        {
            if is_valid_cell(board, next_cell)
            {
                if sudoku_solver(board, next_cell)
                {
                    return true;
                }
            }
            else
            {
                return true;
            }
        }
    }
    board[cell.0][cell.1] = 0;
    false
}

fn is_valid_cell (board : &[[i8; 9]; 9],
                  cell : (usize, usize)) -> bool
{
    !(cell.0 >= board.len() || cell.1 >= board[0].len())
}

fn get_next_cell (board : &[[i8; 9]; 9],
                  curr_cell : (usize, usize)) -> (usize, usize)
{
    for col in (curr_cell.1 + 1) .. 9
    {
        if board[curr_cell.0][col] == 0
        {
            return (curr_cell.0, col);
        }
    }
    
    for row in (curr_cell.0 + 1) .. 9
    {
        for col in 0 .. 9
        {
            if board[row][col] == 0
            {
                return (row, col);
            }
        }
    }

    return (9, 9);
}

fn print_board (board : &[[i8; 9]; 9])
{
    for row in 0..9
    {
        for col in 0..9
        {
            print!("{}", board[row][col]);
            if (col + 1) % 3 == 0 && col != 8
            {
                print!("|");
            }
        }
        println!("");
        
        if (row + 1) % 3 == 0 && row != 8
        {
            println!("---+---+---");
        }
    }
}

fn is_valid_board (board : &[[i8; 9]; 9],
                   cell : (usize, usize),
                   square : (usize, usize)) -> bool
{
    let cell_value : i8 = board[cell.0][cell.1];
    let sub_grid_start : (usize, usize) = (square.0 * 3, square.1 * 3);

    for row in sub_grid_start.0 .. (sub_grid_start.0 + 3)
    {
        for col in sub_grid_start.1 .. (sub_grid_start.1 + 3)
        {
            if board[row][col] == cell_value &&
                !(row == cell.0 && col == cell.1)
            {
                return false;
            }
        }
    }

    for row in 0 .. cell.0
    {
        if  board[row][cell.1] == cell_value
        {
            return false;
        }
    }
    for row in cell.0+1 .. 9
    {
        if  board[row][cell.1] == cell_value
        {
            return false;
        }
    }

    for col in 0 .. cell.1
    {
        if board[cell.0][col] == cell_value
        {
            return false;
        }
    }
    for col in cell.1+1 .. 9
    {
        if board[cell.0][col] == cell_value
        {
            return false;
        }
    }

    true
}

fn fill_with_solvable_board (board : &mut[[i8; 9]; 9])
{
    board[1][0] = 6;
    board[1][1] = 8;
    board[2][0] = 1;
    board[2][1] = 9;

    board[0][3] = 2;
    board[0][4] = 6;
    board[1][4] = 7;
    board[2][5] = 4;

    board[0][6] = 7;
    board[0][8] = 1;
    board[1][7] = 9;
    board[2][6] = 5;

    board[3][0] = 8;
    board[3][1] = 2;
    board[4][2] = 4;
    board[5][1] = 5;

    board[3][3] = 1;
    board[4][3] = 6;
    board[4][5] = 2;
    board[5][5] = 3;

    board[3][7] = 4;
    board[4][6] = 9;
    board[5][7] = 2;
    board[5][8] = 8;

    board[6][2] = 9;
    board[7][1] = 4;
    board[8][0] = 7;
    board[8][2] = 3;

    board[6][3] = 3;
    board[7][4] = 5;
    board[8][4] = 1;
    board[8][5] = 8;

    board[6][7] = 7;
    board[6][8] = 4;
    board[7][7] = 3;
    board[7][8] = 6;
}
