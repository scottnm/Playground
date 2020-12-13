extern crate anyhow;
extern crate rawsock;
extern crate serde;
extern crate wmi;

fn wmic(args: &[&str]) -> anyhow::Result<String> {
    let cmd_output = std::process::Command::new("wmic").args(args).output()?;

    let stdout = std::str::from_utf8(&cmd_output.stdout)?;
    let stderr = std::str::from_utf8(&cmd_output.stderr)?;

    match cmd_output.status.code() {
        Some(0) => (), // all good
        Some(n) => panic!("wmic failure ({})!\n{}", n, stderr),
        _ => panic!("wmic failure!\n{}", stderr),
    }

    let response = stdout
        .split("\n")
        .map(|s| s.trim())
        .filter(|s| !s.is_empty())
        .last()
        .unwrap();

    Ok(response.to_owned())
}

fn main() -> anyhow::Result<()> {
    let interface_index_str = wmic(&[
        "Path",
        "Win32_IP4RouteTable",
        "Where",
        "Destination='0.0.0.0'",
        "Get",
        "InterfaceIndex",
    ])?;

    println!("Default interface index: {}", interface_index_str);

    let interface_query = std::format!("InterfaceIndex={}", interface_index_str);
    let nic_guid_str = wmic(&[
        "Path",
        "Win32_NetworkAdapter",
        "Where",
        &interface_query,
        "Get",
        "Guid",
    ])?;

    println!("NIC guid: {}", nic_guid_str);

    let nic_name = std::format!(r#"\Device\NPF_{}"#, nic_guid_str);
    println!("NIC name: {}", nic_name);

    let lib = rawsock::open_best_library()?;

    let nics = lib.all_interfaces()?;
    let nic = nics.iter().find(|n| n.name == nic_name).unwrap();

    println!("Default interface: {:?}", nic);
    Ok(())
}
