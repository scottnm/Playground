#[macro_use]
mod dll;
mod icmp;
mod iphlp;
mod ipv4;

fn main() {
    let dest_arg = std::env::args().nth(1).unwrap_or_else(|| {
        println!("Usage: sup DEST");
        std::process::exit(1);
    });

    let ping_dest: ipv4::Addr = dest_arg.parse().unwrap();
    let ping_msg = "No one is home";

    println!(
        "Pinging {:?} with {} bytes of data:",
        ping_dest,
        ping_msg.len()
    );

    for _ in 0..4 {
        let ping_req = icmp::Request::new().msg(ping_msg).ttl(19).timeout(10);
        let res = ping_req.send(&ping_dest);
        match res {
            Ok(icmp_reply) => println!(
                "Reply from {:?}: bytes={} time={:?} TTL={:?}",
                ping_dest,
                icmp_reply.data().len(),
                icmp_reply.rtt(),
                icmp_reply.ttl()
            ),
            Err(ping_err) => println!("Ping failed! {}", ping_err),
        }

        std::thread::sleep(std::time::Duration::from_secs(1));
    }
}
