use crate::errors::{Diagnostic, DiagnosticLevel};
use crate::llvm::LLVMString;

pub fn target_machine_error() -> Diagnostic {
    Diagnostic::new(
        DiagnosticLevel::Fatal,
        String::from("error creating target machine for target generation"),
    )
}

pub fn target_triple_error(message: LLVMString) -> Diagnostic {
    Diagnostic::new(
        DiagnosticLevel::Fatal,
        format!(
            "error getting target triple for target generation: {:?}",
            message.as_string()
        ),
    )
}

pub fn failed_to_create_directories(error: std::io::Error) -> Diagnostic {
    Diagnostic::new(
        DiagnosticLevel::Fatal,
        format!("failed to create directories: {:?}", error),
    )
}

pub fn failed_to_create_tempdir(error: std::io::Error) -> Diagnostic {
    Diagnostic::new(
        DiagnosticLevel::Fatal,
        format!("failed to create tempdir: {:?}", error),
    )
}

pub fn failed_to_create_path() -> Diagnostic {
    Diagnostic::new(
        DiagnosticLevel::Fatal,
        String::from("failed to create path string"),
    )
}

pub fn failed_to_get_current_time(error: std::time::SystemTimeError) -> Diagnostic {
    Diagnostic::new(
        DiagnosticLevel::Fatal,
        format!("failed to get current time for temp file name: {:?}", error),
    )
}

pub fn failed_to_invoke_linker(error: std::io::Error) -> Diagnostic {
    Diagnostic::new(
        DiagnosticLevel::Fatal,
        format!("failed to invoke linker: {:?}", error),
    )
}
