fn main()
{
    let mut board = [[-1; 9]; 9];
    board[0][0] = 1;
    board[1][0] = 2;
    board[2][0] = 3;
    board[0][1] = 4;
    board[1][1] = 5;
    board[2][1] = 6;
    board[0][2] = 7;
    board[1][2] = 8;
    board[2][2] = 9;
    let valid = is_valid(&board, (2, 2), (0, 0));
    let valid_str = if valid { "True" } else {"False"};
    println!("Valid? {}", valid_str);
}

fn is_valid(board : &[[i8; 9]; 9],
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
