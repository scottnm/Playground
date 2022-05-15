// This module is setup to simplify cmdline arg parsing
// This could have been done without new code by using one of the many arg parsing crates such as
// structopt, but I decided that since my parsing needs were so small, I didn't want to pull in the
// dependency which, overtime, could lead to bloated binaries and build times. It also felt like a
// good exercise to exercise my rust muscles.

#[derive(Debug)]
pub struct Opts {
    pub output: String,
    pub meta_output: String,
    pub aa_sample_size: u8,
    pub width: usize,
    pub height: usize,
}

impl Opts {
    pub fn from_args() -> Opts {
        fn from_args_internal() -> Result<Opts, String> {
            fn get_arg_value(arg_key: &str, args: &[String]) -> Option<String> {
                for i in 0..args.len() - 1 {
                    if arg_key == args[i] {
                        return Some(args[i + 1].clone());
                    }
                }
                None
            }

            fn get_arg_string(arg_key: &str, args: &[String]) -> Result<String, String> {
                match get_arg_value(arg_key, &args) {
                    Some(arg) => Ok(arg),
                    None => Err(format!("No {} arg", arg_key)),
                }
            }

            fn get_arg_num<T: std::str::FromStr>(
                arg_key: &str,
                args: &[String],
            ) -> Result<T, String> {
                match get_arg_value(arg_key, &args) {
                    Some(arg) => match arg.parse::<T>() {
                        Ok(value) => Ok(value),
                        _ => Err(format!("{} arg \"{}\" not valid", arg_key, arg)),
                    },
                    None => Err(format!("Missing {} arg", arg_key)),
                }
            }

            let args: Vec<String> = std::env::args().collect();
            let output = get_arg_string("--output", &args)?;
            let meta_output = get_arg_string("--meta", &args)?;
            let aa_sample_size = get_arg_num::<u8>("--antialiasing", &args)?;
            let width = get_arg_num::<usize>("--width", &args)?;
            let height = get_arg_num::<usize>("--height", &args)?;

            Ok(Opts { output, meta_output, aa_sample_size, width, height })
        }

        match from_args_internal() {
            Ok(opts) => opts,
            Err(err_msg) => {
                println!("Err: {}", err_msg);
                println!(
                    "Usage: raytracer_rs.exe --output <output> --antialiasing <aa sample size>"
                );
                std::process::exit(1)
            }
        }
    }
}
