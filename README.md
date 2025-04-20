# DirectShare

A peer-to-peer file sharing application designed to work over direct Ethernet and WiFi connections without requiring internet access.

## Features

> [!WARNING]
> This project is still in very early development and has no current release. Features listed here are the end goal.

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
- pkg-config
- libnl (Network Link library)
- ncurses

## Building

### Building with nix

This project includes a Nix flake for reproducible builds and development environments.

To build the project with Nix:

```bash
nix build
```
The built executable will be available at `./result/bin/directshare`.

### Building Manually

> [!NOTE]
> Building manuall requires you to have all [dependencies](#build-dependencies) installed, either manually or through `nix develop`.

```bash
# Create dir and configure
cmake -B build

# Build
cmake --build build # bin in build/src/directshare

# Install [Optional]
cmake --install build # bin in ./bin/directshare
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

## Development

### Development Environment

To enter a development shell with all dependencies available:

```bash
nix develop
```

### Unit testing

Tests are built by default in the build directory (expect with `nix build`. To deactivate them run `cmake -B build -DBUILD_TESTING=OFF`.
```bash
# Configure
cmake -B build

# Build
cmake --build build

# Test
ctest --test-dir build
```

## License

This project is licensed under the GPL3 License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
