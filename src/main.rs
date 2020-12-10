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
    let ping_req = icmp::Request::new()
        .msg("No one is home")
        .ttl(19)
        .timeout(10);

    let res = ping_req.send(&ping_dest);
    match res {
        Ok(icmp_reply) => println!(
            "Reply: {:?}\n{}",
            icmp_reply,
            pretty_hex::pretty_hex(&icmp_reply.data())
        ),
        Err(ping_err) => println!("Ping failed! {}", ping_err),
    }
}
