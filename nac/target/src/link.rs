use std::path::Path;
use std::process::Command;
use std::time::{SystemTime, UNIX_EPOCH};

use tempfile::{Builder, TempDir};

use crate::{TargetGen, TargetGenResult};

impl<'ctx> TargetGen<'ctx> {
    pub fn link(&self) -> TargetGenResult<'ctx, ()> {
        let tempdir = Builder::new()
            .prefix("nac_")
            .tempdir()
            .map_err(|error| self.error_create_temp_dir(error))?;

        let path = self.get_object_file_path(&tempdir)?;
        self.write_object_file(path.as_str())?;

        self.link_executable(path.as_str())?;

        Ok(())
    }

    fn get_object_file_path(&self, tempdir: &TempDir) -> TargetGenResult<'ctx, String> {
        let tempdir_path = tempdir
            .path()
            .to_str()
            .ok_or_else(|| self.error_create_path())?;

        let timestamp = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .map_err(|error| self.error_get_current_time(error))?
            .as_nanos();

        let process_id = std::process::id();
        let filename = format!("{}_{}_main.o", process_id, timestamp);

        std::fs::create_dir_all(tempdir_path)
            .map_err(|error| self.error_create_directories(error))?;

        let path = Path::new(tempdir_path).join(filename);
        let path = path
            .into_os_string()
            .into_string()
            .map_err(|_| self.error_create_path())?;

        Ok(path)
    }

    pub fn write_object_file(&self, path: &str) -> TargetGenResult<'ctx, ()> {
        self.target_machine
            .write_to_file(self.module, path)
            .map_err(|message| self.target_gen_error(message.as_string()))
    }

    pub fn link_executable(&self, path: &str) -> TargetGenResult<'ctx, ()> {
        let linker_status = Command::new("clang")
            .arg(path)
            .arg("-o")
            .arg("./main")
            .status()
            .map_err(|error| self.error_invoking_linker(error))?;

        if !linker_status.success() {
            return Err(self.error_linker_exit_failure());
        }

        Ok(())
    }
}
