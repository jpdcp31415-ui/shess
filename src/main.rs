mod board;
mod piece;
mod vec2;
mod game;
mod test;
mod command;

use std::io;
fn get_line() -> Option<String> {
    let mut buffer = String::new();
    let stdin = io::stdin(); // We get `Stdin` here.
    stdin.read_line(&mut buffer).ok()?;
    Some(buffer)
}

fn main() {
    use game::Game;
    let mut curr_game = Game::start_pos();

    let input = get_line();
    
    println!("You inputted: {}", input.unwrap());
}
