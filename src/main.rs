mod board;
mod piece;
mod vec2;
mod game;
mod test;
mod command;
mod setting;

use std::io;
fn get_line() -> Option<String> {
    let mut buffer = String::new();
    let stdin = io::stdin(); // We get `Stdin` here.
    stdin.read_line(&mut buffer).ok()?;
    Some(buffer)
}

fn main() {
    loop {
        break;
    }

    use setting::Setting;
    use setting::SettingType;
    use setting::SettingData;
    use colored::CustomColor as Colour;
    let all_settings = [
        Setting {
            name: "board-theme",
            data: SettingData::new(SettingType::Str("emoji")),
            is_valid_value: None,
        },
        Setting {
            name: "board-colour",
            data: SettingData::new(SettingType::Colour(Colour {
                r: 255,
                g: 255,
                b: 255,
            })),
            is_valid_value: None,
        },
    ];

    let input = get_line();
    
    println!("You inputted: {}", input.unwrap());
}
