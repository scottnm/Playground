use std::mem::transmute;

type HModule = *const std::ffi::c_void;
type FarProc = *const std::ffi::c_void;
type Fn_MessageBoxA = extern "stdcall" fn(*const std::ffi::c_void, *const u8, *const u8, u32);

extern "stdcall" {
    fn LoadLibraryA(name: *const u8) -> HModule;
    fn GetProcAddress(module: HModule, name: *const u8) -> FarProc;
}

fn main() {
    let h: HModule = unsafe { LoadLibraryA("IPHLPAPI.dll\0".as_ptr()) };
    let user32h: HModule = unsafe { LoadLibraryA("USER32.dll\0".as_ptr()) };
    let MessageBoxFnPtr: FarProc = unsafe { GetProcAddress(user32h, "MessageBoxA\0".as_ptr()) };
    let MessageBoxA: Fn_MessageBoxA = unsafe { transmute(MessageBoxFnPtr) };

    MessageBoxA(
        std::ptr::null(),
        "Hello from Rust!\0".as_ptr(),
        std::ptr::null(),
        0,
    );

    println!(
        "h = {:?}, user32h = {:?}, MessageBoxA = {:?}",
        h, user32h, MessageBoxFnPtr
    );
}
