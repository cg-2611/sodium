pub use self::cursor::Cursor;
pub use self::file::SourceFile;
pub use self::range::{Location, Range};

#[cfg(test)]
mod tests;

pub mod cursor;
pub mod file;
pub mod range;
