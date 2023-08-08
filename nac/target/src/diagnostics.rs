use crate::TargetGen;
use errors::{Diagnostic, ErrorOccurred};

pub type TargetGenError<'ctx> = Diagnostic<'ctx, ErrorOccurred>;
pub type TargetGenResult<'ctx, T> = Result<T, TargetGenError<'ctx>>;

impl<'ctx> TargetGen<'ctx> {
    pub fn target_gen_error(&self, message: String) -> TargetGenError<'ctx> {
        self.context.session().create_error(message)
    }

    pub fn error_create_directories(&self, error: std::io::Error) -> TargetGenError<'ctx> {
        let message = format!("failed to create directories: {:?}", error);
        self.target_gen_error(message)
    }

    pub fn error_create_temp_dir(&self, error: std::io::Error) -> TargetGenError<'ctx> {
        let message = format!("failed to create tempdir: {:?}", error);
        self.target_gen_error(message)
    }

    pub fn error_create_path(&self) -> TargetGenError<'ctx> {
        self.target_gen_error("failed to create path string".to_string())
    }

    pub fn error_get_current_time(
        &self,
        error: std::time::SystemTimeError,
    ) -> TargetGenError<'ctx> {
        let message = format!("failed to get current time for temp file name: {:?}", error);
        self.target_gen_error(message)
    }

    pub fn error_invoking_linker(&self, error: std::io::Error) -> TargetGenError<'ctx> {
        let message = format!("failed to invoke linker: {:?}", error);
        self.target_gen_error(message)
    }

    pub fn error_linker_exit_failure(&self) -> TargetGenError<'ctx> {
        let message = format!("linker failed");
        self.target_gen_error(message)
    }
}
