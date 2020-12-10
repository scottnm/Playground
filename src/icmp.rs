use crate::dll;
use crate::ipv4;

pub struct Request {
    // TODO:
    _msg: String,
    _ttl: u8,
    _timeout: u32,
}

pub struct Reply<'a> {
    icmpr: &'a IcmpEchoReply,
    _buffer: Vec<u8>,
}

impl Reply<'_> {
    pub fn data(&self) -> &[u8] {
        unsafe { std::slice::from_raw_parts(self.icmpr._data, self.icmpr._data_size as usize) }
    }
}

impl std::fmt::Debug for Reply<'_> {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "{:?}", self.icmpr)
    }
}

impl Request {
    pub fn new() -> Self {
        Self {
            _msg: String::from("Hello! Anybody there?"),
            _ttl: 128,
            _timeout: 4000,
        }
    }
    pub fn msg<S>(mut self, request: S) -> Self
    where
        S: AsRef<str>,
    {
        self._msg = String::from(request.as_ref());
        self
    }

    pub fn ttl(mut self, ttl: u8) -> Self {
        self._ttl = ttl;
        self
    }

    pub fn timeout(mut self, timeout: u32) -> Self {
        self._timeout = timeout;
        self
    }

    pub fn send(self, addr: &ipv4::Addr) -> Result<Reply, String> {
        let mut reply_buffer =
            vec![0u8; std::mem::size_of::<IcmpEchoReply>() + 8 + self._msg.len()];

        let echo_options = WinIpOptionInformation {
            ttl: self._ttl,
            tos: 0,
            flags: 0,
            options_data32: 0,
            options_size: 0,
        };

        // FIXME: RAII-ify
        let echo_result = {
            let icmp_file = (IP_HLP_API.IcmpCreateFile)();

            let echo_result = (IP_HLP_API.IcmpSendEcho)(
                icmp_file,
                *addr,
                self._msg.as_ptr(),
                self._msg.len() as u16,
                Some(&echo_options),
                reply_buffer.as_mut_ptr(),
                reply_buffer.len() as u32,
                self._timeout,
            );

            (IP_HLP_API.IcmpCloseHandle)(icmp_file);

            echo_result
        };

        match echo_result {
            0 => Err(String::from("IcmpSendEcho failed! No replies")),
            1 => {
                let reply_ref =
                    unsafe { std::mem::transmute::<&u8, &IcmpEchoReply>(&reply_buffer[0]) };
                Ok(Reply {
                    icmpr: reply_ref,
                    _buffer: reply_buffer,
                })
            }
            _ => Err(std::format!("Unexpected reply count! {}", echo_result)),
        }
    }
}

// Private Windows definitions and implementations
type WinHandle = *const std::ffi::c_void; // HANDLE (const void*)

#[repr(C)]
#[derive(Debug)]
struct WinIpOptionInformation {
    ttl: u8,
    tos: u8,
    flags: u8,
    options_size: u8,
    options_data32: u32,
}

#[repr(C)]
#[derive(Debug)]
pub struct IcmpEchoReply {
    pub addr: ipv4::Addr,
    pub status: u32,
    pub rtt: u32,
    _data_size: u16,
    _reserved: u16,
    _data: *mut u8,
    _options: WinIpOptionInformation,
}

#[allow(non_snake_case)]
struct IpHlpFn {
    pub IcmpCreateFile: extern "stdcall" fn() -> WinHandle,

    pub IcmpSendEcho: extern "stdcall" fn(
        handle: WinHandle,
        dest: ipv4::Addr,
        request_data: *const u8,
        request_size: u16,
        request_options: Option<&WinIpOptionInformation>,
        reply_buffer: *mut u8,
        reply_size: u32,
        timeout: u32,
    ) -> u32,

    pub IcmpCloseHandle: extern "stdcall" fn(handle: WinHandle),
}

use once_cell::sync::Lazy;
static IP_HLP_API: Lazy<IpHlpFn> = Lazy::new(|| {
    let iphlp = dll::Library::new("IPHLPAPI.dll").expect("IPHLPAPI.dll not found");

    // TODO: cleanup liberal use of expect/unwrap
    IpHlpFn {
        IcmpCreateFile: iphlp
            .get_proc("IcmpCreateFile")
            .expect("IcmpCreateFile not found"),
        IcmpSendEcho: iphlp
            .get_proc("IcmpSendEcho")
            .expect("IcmpSendEcho not found"),
        IcmpCloseHandle: iphlp
            .get_proc("IcmpCloseHandle")
            .expect("IcmpCloseHandle not found"),
    }
});
