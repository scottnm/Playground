extern crate anyhow;
extern crate rawsock;
extern crate serde;
extern crate wmi;

fn main() -> anyhow::Result<()> {
    let wmi_com = wmi::COMLibrary::new()?;
    let wmi_connection = wmi::WMIConnection::new(wmi_com.into())?;

    #[derive(serde::Deserialize, Debug)]
    #[allow(non_camel_case_types, non_snake_case)]
    struct Win32_IP4RouteTable {
        InterfaceIndex: i64,
    }

    let default_nic_index_query: Vec<Win32_IP4RouteTable> = wmi_connection
        .raw_query("SELECT * FROM Win32_IP4RouteTable Where Destination='0.0.0.0'")?;
    let default_nic_index = default_nic_index_query[0].InterfaceIndex;
    println!("Default nic index: {}", default_nic_index);

    #[derive(serde::Deserialize, Debug)]
    #[allow(non_camel_case_types, non_snake_case)]
    struct Win32_NetworkAdapter {
        guid: String,
    }

    let default_nic_query_str = std::format!(
        "SELECT * FROM Win32_NetworkAdapter Where InterfaceIndex={}",
        default_nic_index,
    );
    let default_nic_query: Vec<Win32_NetworkAdapter> =
        wmi_connection.raw_query(&default_nic_query_str)?;
    let default_nic_guid_str = &default_nic_query[0].guid;
    println!("Default nic guid: {}", default_nic_guid_str);

    let default_nic_name = std::format!(r#"\Device\NPF_{}"#, default_nic_guid_str);
    println!("NIC name: {}", default_nic_name);

    let lib = rawsock::open_best_library()?;

    let nics = lib.all_interfaces()?;
    let nic = nics.iter().find(|n| n.name == default_nic_name).unwrap();

    println!("Default nic: {:?}", nic);
    Ok(())
}
