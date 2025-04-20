# DirectShare

A peer-to-peer file sharing application designed to work over direct Ethernet and WiFi connections without requiring internet access.

## Features

- **Direct Ethernet Connection**: Automatically configures IP addresses when computers are connected via Ethernet cable
- **WiFi Hotspot Creation**: Negotiates which device creates a hotspot and which connects to it
- **Fast Transfers**: Optimized for maximum transfer speed without internet or server bottlenecks
- **Secure**: End-to-end encryption for all file transfers
- **Easy to Use**: Simple terminal-based UI that handles all the technical details automatically

## Requirements

### Runtime Dependencies
- Linux-based operating system
- NetworkManager (for WiFi hotspot creation)
- Administrator privileges (for network interface configuration)

### Build Dependencies
- C compiler (GCC or Clang)
- CMake
- libnl (Network Link library)
- libpcap
- ncurses
- OpenSSL
- libsodium
- D-Bus
- NetworkManager development libraries
- zlib

## Building with Nix

This project includes a Nix flake for reproducible builds and development environments.

### Development Environment

To enter a development shell with all dependencies:

```bash
nix develop
```

### Building

To build the project with Nix:

```bash
nix build
```

The built executable will be available at `./result/bin/directshare`.

## Building Manually

```bash
# Create build directory
mkdir -p build && cd build

# Configure
cmake ..

# Build
make

# Install (optional)
sudo make install
```

## Usage

```bash
# Start DirectShare (requires root for network operations)
sudo directshare
```

The application will guide you through the process of discovering peers and transferring files.

If you don't have access to multiple machines you can [test DirectShare using VMs](./docs/testing.md).

## How It Works

DirectShare uses a combination of techniques to establish direct connections:

1. **Network Discovery**: Detects available network interfaces and identifies potential connection methods
2. **Connection Negotiation**: Devices communicate to decide the optimal connection method
3. **Direct Connection**: Establishes either:
   - Direct Ethernet connection with automatic IP address assignment
   - WiFi connection where one device creates a hotspot and the other connects to it
4. **File Transfer**: Uses an optimized protocol for maximum throughput with integrity verification

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.## Setting Up Multipass VMs for DirectShare Testing

