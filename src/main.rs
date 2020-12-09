mod dll;
mod icmp;
mod ipv4;

fn main() {
    // TODO: cleanup liberal use of unwrap
    use pretty_hex::*;
    let res = icmp::ping(&ipv4::Addr([8, 8, 8, 8]));
    match res {
        Ok(icmp_reply) => println!(
            "Reply: {:?}\n{}",
            icmp_reply,
            pretty_hex(&icmp_reply.data())
        ),
        Err(ping_err) => println!("Ping failed! {}", ping_err),
    }
}
