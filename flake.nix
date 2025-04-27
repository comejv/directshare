{
  description = "DirectShare - Peer-to-peer file sharing over Ethernet and WiFi";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };

        libmnlStatic = pkgs.libmnl.overrideAttrs (oldAttrs: {
          configureFlags = (oldAttrs.configureFlags or [ ]) ++ [
            "--enable-static"
            "--disable-shared"
          ];
          dontDisableStatic = true;
        });

        ncursesStatic = pkgs.ncurses.override { enableStatic = true; };
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
              libmnlStatic
              ncursesStatic
              glibc.static
            ];

            LDFLAGS = "-static -pthread";

            configurePhase = ''
              cmake -B build \
                -DBUILD_TESTING=OFF \
                -DCMAKE_FIND_LIBRARY_SUFFIXES=".a" \
                -DCMAKE_EXE_LINKER_FLAGS="-static -pthread"
            '';

            buildPhase = ''
              cmake --build build
            '';

            installPhase = ''
              mkdir -p $out/bin
              cp build/src/directshare $out/bin/
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
            gdb
            clang-tools
            cmake-language-server
            cmake-format

            # Libraries
            libmnlStatic
            ncursesStatic
            glibc.static
          ];

          shellHook = ''
            export LIBRARY_PATH="${ncursesStatic}/lib:${libmnlStatic}/lib:$LIBRARY_PATH";
            export LDFLAGS="-static -pthread"
            export CFLAGS="-I${libmnlStatic}/include"
            echo "Welcome to the DirectShare development environment!"
            echo "Build with Nix : nix build"
            echo "Or with GCC: cmake -B build && cmake --build build"
          '';
        };
      }
    );
}
