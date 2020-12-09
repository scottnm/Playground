mod dll;

use std::ffi::c_void;
use std::fmt;

type WinHandle = *const c_void; // HANDLE

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
struct IcmpEchoReply {
    addr: IPAddr,
    status: u32,
    rtt: u32,
    _data_size: u16,
    _reserved: u16,
    _data: *mut u8,
    options: WinIpOptionInformation,
}

impl IcmpEchoReply {
    fn data(&self) -> &[u8] {
        unsafe { std::slice::from_raw_parts(self._data, self._data_size as usize) }
    }
}

#[repr(C)]
struct IPAddr([u8; 4]);

impl fmt::Debug for IPAddr {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}.{}.{}.{}", self.0[0], self.0[1], self.0[2], self.0[3])
    }
}

type FnIcmpCreateFile = extern "stdcall" fn() -> WinHandle;

type FnIcmpSendEcho = extern "stdcall" fn(
    handle: WinHandle,
    dest: IPAddr,
    request_data: *const u8,
    request_size: u16,
    request_options: Option<&WinIpOptionInformation>,
    reply_buffer: *mut u8,
    reply_size: u32,
    timeout: u32,
) -> u32;

fn main() {
    // TODO: cleanup liberal use of unwrap
    let iphlapi_lib = dll::Library::new("IPHLPAPI.dll").expect("IPHLPAPI.dll not found");

    #[allow(non_snake_case)]
    let IcmpCreateFile: FnIcmpCreateFile = iphlapi_lib
        .get_proc("IcmpCreateFile")
        .expect("IcmpCreateFile not found");

    #[allow(non_snake_case)]
    let IcmpSendEcho: FnIcmpSendEcho = iphlapi_lib
        .get_proc("IcmpSendEcho")
        .expect("IcmpSendEcho not found");

    let icmp_file = IcmpCreateFile();

    let echo_msg = "can you hear me?";
    let mut reply_buffer = vec![0u8; std::mem::size_of::<IcmpEchoReply>() + 8 + echo_msg.len()];

    let reply: &IcmpEchoReply = {
        let echo_options = WinIpOptionInformation {
            ttl: 128,
            tos: 0,
            flags: 0,
            options_data32: 0,
            options_size: 0,
        };

        let echo_result = IcmpSendEcho(
            icmp_file,
            IPAddr([8, 8, 8, 8]),
            echo_msg.as_ptr(),
            echo_msg.len() as u16,
            Some(&echo_options),
            reply_buffer.as_mut_ptr(),
            reply_buffer.len() as u32,
            4000,
        );

        match echo_result {
            0 => panic!("IcmpSendEcho failed! No replies"),
            1 => unsafe { std::mem::transmute::<&u8, &IcmpEchoReply>(&reply_buffer[0]) },
            _ => panic!("Unexpected reply count! {}", echo_result),
        }
    };

    use pretty_hex::*;
    println!("Reply:{:?}\n{}", reply, pretty_hex(&reply.data()));
}
