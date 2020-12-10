type WinHModule = std::ptr::NonNull<std::ffi::c_void>; // HMODULE (non-null void*)
type WinFarProc = std::ptr::NonNull<std::ffi::c_void>; // FARPROC (non-null void*)

extern "stdcall" {
    fn LoadLibraryA(name: *const i8) -> Option<WinHModule>;
    fn GetProcAddress(module: WinHModule, name: *const i8) -> Option<WinFarProc>;
}

pub struct Library {
    handle: WinHModule,
}

impl Library {
    pub fn new(name: &str) -> Option<Self> {
        // TODO: handle invalid string name errors
        let c_name = std::ffi::CString::new(name.as_bytes()).expect("invalid library name!");
        let maybe_lib = unsafe { LoadLibraryA(c_name.as_ptr()) };
        maybe_lib.map(|lib| Library { handle: lib })
    }

    pub fn get_proc<T>(&self, name: &str) -> Option<T> {
        let c_name = std::ffi::CString::new(name.as_bytes()).expect("invalid proc name!");
        let maybe_proc = unsafe { GetProcAddress(self.handle, c_name.as_ptr()) };
        // TODO: anyway to use transmute instead of transmute_copy? we need to find some way
        // to let the compiler understand the sizing expectations
        maybe_proc.map(|proc| unsafe { std::mem::transmute_copy(&proc) })
    }
}

/*
#[macro_export]
macro_rules! bind {
    ($struct_name:ident $struct_once_name:ident $dll_name:ident $(fn $fn_name:ident($($arg:ident: $type:ty),*) -> $ret:ty;)*) => {
        struct $struct_name {
            $($fn_name: extern "stdcall" fn ($($arg: $type),*) -> $ret),*
        }

        use once_cell::sync::Lazy;
        static $struct_once_name: Lazy<Functions> = Lazy::new(|| {
            let dll_name = format!("{}.dll", stringify!($dll_name));
            let dll = dll::Library::new(&dll_name).unwrap();

            // TODO: cleanup liberal use of expect/unwrap
            Functions {
                $($fn_name: dll.get_proc(stringify!($fn_name)).unwrap(),),*
            }
        });
    };
}

*/
