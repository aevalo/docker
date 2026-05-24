extern crate stdint;

use std::ffi::c_char;
use stdint::int32_t;

#[no_mangle]
/// Print a greeting to given name.
pub extern "C" fn rust_function(name: *const c_char) {
    let name = unsafe { std::ffi::CStr::from_ptr(name).to_str().unwrap() };
    println!("Hello, {}! I'm Rust!", name);
}

#[no_mangle]
/// Add given parameters together, and return the result.
pub extern "C" fn add(left: int32_t, right: int32_t) -> int32_t {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
