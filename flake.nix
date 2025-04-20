{
  description = "DirectShare - Peer-to-peer file sharing over Ethernet and WiFi";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        packages = {
          default = self.packages.${system}.directshare;

          directshare = pkgs.stdenv.mkDerivation {
            pname = "directshare";
            version = "0.1.0";
            src = ./.;

            nativeBuildInputs = with pkgs; [
              pkg-config
              cmake
            ];

            buildInputs = with pkgs; [
              libnl
              ncurses
            ];

            configurePhase = ''
              cmake -B build
            '';

            buildPhase = ''
              cmake --build build
            '';

            installPhase = ''
              mkdir -p $out/bin
              install -m755 build/directshare $out/bin/directshare
            '';

            meta = with pkgs.lib; {
              description = "Peer-to-peer file sharing application over direct Ethernet and WiFi connections";
              homepage = "https://github.com/comejv/directshare";
              license = licenses.gpl3;
              platforms = platforms.linux;
            };
          };
        };

        devShells.default = pkgs.mkShell {
          name = "directshare-dev-shell";

          packages = with pkgs; [
            # Build essentials
            gcc
            cmake
            pkg-config
            
            # Development tools
            # clang-tools # For clangd, clang-format, etc. (commented out)
            gdb         # For debugging
            
            # Minimal libraries
            libnl
            ncurses
          ];

          shellHook = ''
            echo "Welcome to the DirectShare development environment!"
            echo "Build with GCC: mkdir -p build && cd build && cmake .. && make"
            echo ""
            echo "Note: NetworkManager operations require elevated privileges."
          '';
        };
      }
    );
}
