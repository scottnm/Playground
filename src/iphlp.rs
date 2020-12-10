use crate::dll;
use crate::ipv4;

type Handle = *const std::ffi::c_void; // HANDLE (const void*)

#[repr(C)]
#[derive(Debug)]
pub struct IpOptionInformation {
    pub ttl: u8,
    pub tos: u8,
    pub flags: u8,
    pub options_size: u8,
    pub options_data32: u32,
}

#[repr(C)]
#[derive(Debug)]
pub struct IcmpEchoReply {
    pub addr: ipv4::Addr,
    pub status: u32,
    pub rtt: u32,
    data_size: u16,
    reserved: u16,
    data: *mut u8,
    options: IpOptionInformation,
}

impl IcmpEchoReply {
    pub fn data(&self) -> &[u8] {
        unsafe { std::slice::from_raw_parts(self.data, self.data_size as usize) }
    }
}

bind! {
    "IPHLPAPI.dll"
    fn IcmpCreateFile() -> Handle;
    fn IcmpSendEcho(handle: Handle, dest: ipv4::Addr, requestdata: *const u8, request_size: u16, requestoptions: Option<&IpOptionInformation>, reply_buffer: *mut u8, reply_size: u32, timeout: u32) -> u32;
    fn IcmpCloseHandle(handle: Handle) -> ();
}
