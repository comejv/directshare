# Testing the app on a single machine

## 1. Install Canonical Multipass
- Download and install from https://multipass.run/
- Choose Hyper-V as the hypervisor (default on Windows)

## 2. Create Two VMs
```bash
# Create first VM
multipass launch --name directshare-1 --memory 2G --cpus 2 --disk 10G

# Create second VM
multipass launch --name directshare-2 --memory 2G --cpus 2 --disk 10G
```

## 3. Configure Network Bridging
```bash
# List available networks
multipass networks

# Set your preferred physical network (replace eth0 with actual network)
multipass set local.bridged-network=eth0

# Stop both VMs
multipass stop directshare-1 directshare-2

# Enable bridged networking for both VMs
multipass set local.directshare-1.bridged=true
multipass set local.directshare-2.bridged=true

# Start the VMs
multipass start directshare-1 directshare-2
```

## 4. Verify Connectivity
```bash
# View assigned IP addresses
multipass list

# Connect to the first VM
multipass shell directshare-1

# Test connectivity to the second VM (use its actual IP)
ping [IP-of-VM2]
```
