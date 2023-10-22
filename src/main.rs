use serialport::SerialPort;
use std::time::Duration;

fn main() {
    println!("Hello, world!");

    let ports = serialport::available_ports().expect("No ports found!");
    for p in ports {
        println!("{}", p.port_name);
    }

    let mut port = serialport::new("COM6", 9600)
    .timeout(Duration::from_millis(100))
        .open().expect("Failed to open port");


    let mut serial_buf: Vec<u8> = vec![0; 32];

    let mut info = ReadInfo {
        line: "".into(),
        remaining: "".into(),
        buffer: vec![0; 1024],
    };


    loop {
        read_to_newline(&mut port, &mut info);
    }

}


struct ReadInfo {
    line: String,
    remaining: String,
    buffer: Vec::<u8>
}


fn read_to_newline(port: &mut Box<dyn SerialPort>, info: &mut ReadInfo) {
    let read_bytes = port.read(info.buffer.as_mut_slice()).expect("Found no data!");

    let s = String::from_utf8_lossy(&info.buffer[0..read_bytes]);

    let mut parts: Vec::<&str> = s.split("\n").collect();

    let len = parts.len();

    if len == 0 {
        return;
    }

    // first will print remainder and part
    info.line = info.remaining.clone() + parts[0];
    info.remaining = "".into();
    println!("{:?}", info.line);


    // print remaining parts that ended on newline
    for i in 1..len - 1 {
        println!("{:?}", parts[i]);
    }
    // put last part into remaining
    info.remaining = parts[len - 1].into();

}
