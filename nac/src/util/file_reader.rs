use std::fs::read_to_string;

pub fn read_file(path: &str) -> Option<String> {
    let contents = read_to_string(path);
    match contents {
        Ok(string) => Some(string),
        Err(_) => None,
    }
}
