{pkgs ? import <nixpkgs> {}}:
  (pkgs.buildFHSUserEnv {
    name = "ctf";
    targetPkgs = pkgs: (with pkgs; [
      gcc
      llvmPackages_latest.clang
      clang-tools
      steam-run
      wget
      glibc
      openssl
      qmk
    ]);
    runScript = ''zsh
    '';
}).env
