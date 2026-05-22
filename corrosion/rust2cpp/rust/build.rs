extern crate cbindgen;

use std::env;

fn main() {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let target_dir = match env::var("CARGO_TARGET_DIR") {
        Ok(target) => target,
        Err(_error) => String::from("target")
    };
    let header_file = format!("{}/include/rust_lib.h", target_dir);

    cbindgen::Builder::new()
      .with_crate(crate_dir)
      .with_language(cbindgen::Language::C)
      .with_include_guard("RUST_LIB_H_INCLUDED")
      .with_no_includes()
      .with_sys_include("stdint.h")
      .with_cpp_compat(true)
      .generate()
      .expect("Unable to generate bindings")
      .write_to_file(header_file);
}
