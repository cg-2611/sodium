use std::path::Path;
use std::process::Command;
use std::time::{SystemTime, UNIX_EPOCH};
use std::{fs, process};

use tempfile::Builder;

use crate::llvm::{Module, TargetMachine};
use crate::target::TargetGen;

impl<'ctx> TargetGen<'ctx> {
    pub fn link_executable(&self, module: &'ctx Module, target_machine: &TargetMachine) {
        let tempdir = Builder::new().prefix("nac_").tempdir().unwrap();
        let tempdir_path = tempdir.path().to_str().unwrap();

        let timestamp = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_nanos();
        let process_id = process::id();

        let filename = format!("{}_{}_main.o", process_id, timestamp);

        fs::create_dir_all(tempdir_path).unwrap();

        let path = Path::new(tempdir_path).join(filename);
        let path = path.to_str().unwrap();

        target_machine
            .write_to_file(module, path)
            .expect("failed to write object file");

        Command::new("clang")
            .arg(path)
            .arg("-o")
            .arg("./main")
            .status()
            .unwrap();
    }
}
