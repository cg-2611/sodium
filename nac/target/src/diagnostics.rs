use crate::TargetGen;
use errors::{Diagnostic, ErrorOccurred};

pub type TargetGenError<'a> = Diagnostic<'a, ErrorOccurred>;
pub type TargetGenResult<'a, T> = Result<T, TargetGenError<'a>>;

impl<'a> TargetGen<'a> {
    pub fn target_gen_error(&self, message: String) -> TargetGenError<'a> {
        self.sess.create_error(message)
    }

    pub fn error_create_directories(&self, error: std::io::Error) -> TargetGenError<'a> {
        let message = format!("failed to create directories: {:?}", error);
        self.target_gen_error(message)
    }

    pub fn error_create_temp_dir(&self, error: std::io::Error) -> TargetGenError<'a> {
        let message = format!("failed to create tempdir: {:?}", error);
        self.target_gen_error(message)
    }

    pub fn error_create_path(&self) -> TargetGenError<'a> {
        self.target_gen_error("failed to create path string".to_string())
    }

    pub fn error_get_current_time(&self, error: std::time::SystemTimeError) -> TargetGenError<'a> {
        let message = format!("failed to get current time for temp file name: {:?}", error);
        self.target_gen_error(message)
    }

    pub fn error_invoking_linker(&self, error: std::io::Error) -> TargetGenError<'a> {
        let message = format!("failed to invoke linker: {:?}", error);
        self.target_gen_error(message)
    }

    pub fn error_linker_exit_failure(&self) -> TargetGenError<'a> {
        self.target_gen_error("linker failed".to_string())
    }
}
