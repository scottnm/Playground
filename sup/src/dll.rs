type HModule = usize; // HMODULE (non-null void*)
type FarProc = std::ptr::NonNull<std::ffi::c_void>; // FARPROC (non-null void*)

extern "stdcall" {
    fn LoadLibraryA(name: *const i8) -> HModule;
    fn GetProcAddress(module: HModule, name: *const i8) -> Option<FarProc>;
    fn FreeLibrary(module: HModule) -> bool;
}

pub struct Library {
    handle: HModule,
}

impl Library {
    pub fn new(name: &str) -> Option<Self> {
        // TODO: handle invalid string name errors
        let c_name = std::ffi::CString::new(name.as_bytes()).expect("invalid library name!");
        let maybe_lib = unsafe { LoadLibraryA(c_name.as_ptr()) };
        match maybe_lib {
            0 => None,
            _ => Some(Library { handle: maybe_lib }),
        }
    }

    pub fn get_proc<T>(&self, name: &str) -> Option<T> {
        let c_name = std::ffi::CString::new(name.as_bytes()).expect("invalid proc name!");
        let maybe_proc = unsafe { GetProcAddress(self.handle, c_name.as_ptr()) };
        // TODO: anyway to use transmute instead of transmute_copy? we need to find some way
        // to let the compiler understand the sizing expectations
        maybe_proc.map(|proc| unsafe { std::mem::transmute_copy(&proc) })
    }
}

impl Drop for Library {
    fn drop(&mut self) {
        let succeeded: bool = unsafe { FreeLibrary(self.handle) };
        assert!(succeeded);
        println!("dropping");
    }
}

macro_rules! bind {
    ($dll:literal $(fn $fn_name:ident($($arg:ident: $type:ty),*) -> $ret:ty;)*) => {

        #[allow(non_snake_case)]
        struct FNS {
            $($fn_name: extern "stdcall" fn ($($arg: $type),*) -> $ret),*,
            _dll: dll::Library,
        }

        use once_cell::sync::Lazy;
        static FNS : Lazy<FNS> = Lazy::new(|| {
            let dll = dll::Library::new($dll).unwrap();

            // TODO: cleanup liberal use of expect/unwrap
            FNS {
                $($fn_name: dll.get_proc(stringify!($fn_name)).unwrap(),)*
                _dll: dll,
            }
        });

        $(
            #[allow(non_snake_case)]
            #[inline(always)]
            pub fn $fn_name($($arg: $type),*) -> $ret {
                (FNS.$fn_name)($($arg),*)
            }
        )*
    };
}
