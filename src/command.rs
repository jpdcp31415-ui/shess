use std::io;

fn get_line() -> Option<String> {
    let mut buffer = String::new();
    let stdin = io::stdin();
    stdin.read_line(&mut buffer).ok()?;
    Some(buffer)
}

struct CommandRunArgs<'a> {
    usage: &'a str,
    input: &'a str,
}

enum CommandRunType {
    TakesNoArgs(fn()),
    TakesArgs(fn(&CommandRunArgs) -> Result<(), String>),
}

struct CommandUsage {
    format: &'static str,
    text: &'static str,
}

struct Command {
    name: &'static str,
    run: CommandRunType,
    usage: Option<CommandUsage>,
    help_text: &'static str,
}

fn moveCmd(args: &CommandRunArgs) -> Result<(), String> {
    Ok(())
}

static ALL_CMDS: [Command; 1] = [
    Command {
        name: "mv",
        run: CommandRunType::TakesArgs(moveCmd),
        usage: Some(CommandUsage {
            format: "{usize} {usize} {i32} {i32}",
            text: "pos-x pos-y mov-x mov-y",
        }),
        help_text: "A command for moving pieces around",
    }
];

impl Command {
    fn find(name: &str) -> Option<&Command> {
        for i in 0..(ALL_CMDS.len()) {
            if ALL_CMDS[i].name == name {
                return Some(&ALL_CMDS[i]);
            }
        }
        None
    }

    fn get_usage(name: &str) -> Option<&CommandUsage> {
        Self::find(name).unwrap().usage.as_ref()
    }

    fn run(name: &str, input: &str) -> Result<(), String> {
        let command = Self::find(name);

        if command.is_none() {
            return Err(format!("No command {name} found"));
        }

        match command.unwrap().run {
            CommandRunType::TakesNoArgs(func) => func(),
            CommandRunType::TakesArgs(func) => {
                let res = func(&CommandRunArgs {
                    input,
                    usage: command.unwrap().usage.as_ref().unwrap().format,
                });
                match res {
                    Err(error_msg) => println!("Error: {error_msg}"),
                    _ => (),
                };
            },
        }

        return Err(format!("No command {name} found"));
    }
}
