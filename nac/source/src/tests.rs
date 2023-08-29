use tempfile::Builder;

use crate::file::SourceFileReader;

macro_rules! initialise_test_file {
    ($tempdir: expr, $name: expr, $contents: expr) => {{
        let path = $tempdir
            .path()
            .to_str()
            .expect("failed to convert test tempdir path to str");

        let file_path = std::path::Path::new(path).join($name);

        std::fs::write(&file_path, $contents).expect("failed to write to temporary test file");

        file_path
            .into_os_string()
            .into_string()
            .expect("failed to convert test file path to string")
    }};
}

macro_rules! emit_diagnostic {
    ($result: expr) => {
        let _ = $result.map_err(|mut error| error.emit());
    };
}

const TEST_FILE_CONTENTS: &str = r#"this is a
        file that is
    used for
    testing"#;

#[test]
fn source_file_reader_reads_file() {
    let sess = session::Session::new();
    let tempdir = Builder::new()
        .tempdir()
        .expect("failed to create tempdir for temporary test file");

    let file_name = "test_file.na";
    let file_path = initialise_test_file!(tempdir, file_name, TEST_FILE_CONTENTS);

    let source_file_result = SourceFileReader::source_file_from_path(&sess, file_path.as_str());

    assert!(source_file_result.is_ok());

    let source_file = source_file_result.unwrap();
    assert_eq!(source_file.path(), file_path.as_str());
    assert_eq!(source_file.name(), file_name);
    assert_eq!(source_file.contents(), TEST_FILE_CONTENTS);

    assert!(sess.has_errors().is_ok());
    assert_eq!(sess.error_count(), 0);
}

#[test]
fn source_file_reader_rejects_files_with_incorrect_extension() {
    let sess = session::Session::new();
    let tempdir = Builder::new()
        .tempdir()
        .expect("failed to create tempdir for temporary test file");

    let file_name = "test_file.rs";
    let file_path = initialise_test_file!(tempdir, file_name, TEST_FILE_CONTENTS);

    let source_file_result = SourceFileReader::source_file_from_path(&sess, file_path.as_str());

    assert!(source_file_result.is_err());
    emit_diagnostic!(source_file_result);

    assert!(sess.has_errors().is_err());
    assert_eq!(sess.error_count(), 1);
}

#[test]
fn source_file_reader_rejects_files_with_missing_extension() {
    let sess = session::Session::new();
    let tempdir = Builder::new()
        .tempdir()
        .expect("failed to create tempdir for temporary test file");

    let file_name = "test_file";
    let file_path = initialise_test_file!(tempdir, file_name, TEST_FILE_CONTENTS);

    let source_file_result = SourceFileReader::source_file_from_path(&sess, file_path.as_str());

    assert!(source_file_result.is_err());
    emit_diagnostic!(source_file_result);

    assert!(sess.has_errors().is_err());
    assert_eq!(sess.error_count(), 1);
}

#[test]
fn source_file_reader_rejects_file_that_does_not_exist() {
    let sess = session::Session::new();
    let tempdir = Builder::new()
        .tempdir()
        .expect("failed to create tempdir for temporary test file");

    let _ = initialise_test_file!(tempdir, "test_file.na", TEST_FILE_CONTENTS);

    let source_file_result = SourceFileReader::source_file_from_path(&sess, "does_not_exist.na");

    assert!(source_file_result.is_err());
    emit_diagnostic!(source_file_result);

    assert!(sess.has_errors().is_err());
    assert_eq!(sess.error_count(), 1);
}
