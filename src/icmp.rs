use crate::dll;
use crate::ipv4;

pub struct Request {
    // TODO:
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

pub fn ping(addr: &ipv4::Addr) -> Result<Reply, String> {
    // TODO: cleanup liberal use of unwrap
    let iphlp = dll::Library::new("IPHLPAPI.dll").expect("IPHLPAPI.dll not found");

    #[allow(non_snake_case)]
    let IcmpCreateFile: FnIcmpCreateFile = iphlp
        .get_proc("IcmpCreateFile")
        .expect("IcmpCreateFile not found");

    #[allow(non_snake_case)]
    let IcmpSendEcho: FnIcmpSendEcho = iphlp
        .get_proc("IcmpSendEcho")
        .expect("IcmpSendEcho not found");

    let icmp_file = IcmpCreateFile();

    let echo_msg = "can you hear me?";
    let mut reply_buffer = vec![0u8; std::mem::size_of::<IcmpEchoReply>() + 8 + echo_msg.len()];

    let echo_options = WinIpOptionInformation {
        ttl: 128,
        tos: 0,
        flags: 0,
        options_data32: 0,
        options_size: 0,
    };

    let echo_result = IcmpSendEcho(
        icmp_file,
        *addr,
        echo_msg.as_ptr(),
        echo_msg.len() as u16,
        Some(&echo_options),
        reply_buffer.as_mut_ptr(),
        reply_buffer.len() as u32,
        4000,
    );

    match echo_result {
        0 => Err(String::from("IcmpSendEcho failed! No replies")),
        1 => {
            let reply_ref = unsafe { std::mem::transmute::<&u8, &IcmpEchoReply>(&reply_buffer[0]) };
            Ok(Reply {
                icmpr: reply_ref,
                _buffer: reply_buffer,
            })
        }
        _ => Err(std::format!("Unexpected reply count! {}", echo_result)),
    }
}

// Private Windows implementation
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

type FnIcmpCreateFile = extern "stdcall" fn() -> WinHandle;

type FnIcmpSendEcho = extern "stdcall" fn(
    handle: WinHandle,
    dest: ipv4::Addr,
    request_data: *const u8,
    request_size: u16,
    request_options: Option<&WinIpOptionInformation>,
    reply_buffer: *mut u8,
    reply_size: u32,
    timeout: u32,
) -> u32;

/* TODO: amos defined these but do I really want them?
fn IcmpCreateFile() -> WinHandle {
    let iphlp = dll::Library::new("IPHLPAPI.dll").expect("IPHLPAPI.dll not found");

    let IcmpCreateFileProc: FnIcmpCreateFile = iphlp
        .get_proc("IcmpCreateFile")
        .expect("IcmpCreateFile not found");

    IcmpCreateFileProc()
}

fn IcmpSendEcho(
    handle: WinHandle,
    dest: ipv4::Addr,
    request_data: *const u8,
    request_size: u16,
    request_options: Option<&WinIpOptionInformation>,
    reply_buffer: *mut u8,
    reply_size: u32,
    timeout: u32,
) -> u32 {
    let iphlp = dll::Library::new("IPHLPAPI.dll").expect("IPHLPAPI.dll not found");

    let IcmpSendEchoProc: FnIcmpSendEcho = iphlp
        .get_proc("IcmpSendEcho")
        .expect("IcmpSendEcho not found");

    IcmpSendEchoProc(
        handle,
        dest,
        request_data,
        request_size,
        request_options,
        reply_buffer,
        reply_size,
        timeout,
    )
}
*/
