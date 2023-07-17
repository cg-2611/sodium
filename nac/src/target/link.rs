use std::path::Path;
use std::process::Command;
use std::time::{SystemTime, UNIX_EPOCH};

use tempfile::{Builder, TempDir};

use crate::errors::Result;
use crate::target::diagnostics::{
    failed_to_create_directories, failed_to_create_path, failed_to_create_tempdir,
    failed_to_get_current_time, failed_to_invoke_linker,
};
use crate::target::TargetGen;

impl<'s, 'ctx> TargetGen<'s, 'ctx> {
    pub fn link(&self) -> Result<()> {
        let tempdir = Builder::new()
            .prefix("nac_")
            .tempdir()
            .map_err(failed_to_create_tempdir)?;

        let path = Self::get_object_file_path(&tempdir)?;
        self.write_object_file(path.as_str())?;

        Self::link_executable(path.as_str())?;

        Ok(())
    }

    fn get_object_file_path(tempdir: &TempDir) -> Result<String> {
        let tempdir_path = tempdir.path().to_str().ok_or(failed_to_create_path())?;

        let timestamp = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .map_err(failed_to_get_current_time)?
            .as_nanos();

        let process_id = std::process::id();
        let filename = format!("{}_{}_main.o", process_id, timestamp);

        std::fs::create_dir_all(tempdir_path).map_err(failed_to_create_directories)?;

        let path = Path::new(tempdir_path).join(filename);
        let path = path
            .into_os_string()
            .into_string()
            .map_err(|_| failed_to_create_path())?;

        Ok(path)
    }

    pub fn write_object_file(&self, path: &str) -> Result<()> {
        self.target_machine
            .write_to_file(self.module, path)
            .map_err(|message| message.to_fatal())
    }

    pub fn link_executable(path: &str) -> Result<()> {
        Command::new("clang")
            .arg(path)
            .arg("-o")
            .arg("./main")
            .status()
            .map_err(failed_to_invoke_linker)?;

        Ok(())
    }
}
