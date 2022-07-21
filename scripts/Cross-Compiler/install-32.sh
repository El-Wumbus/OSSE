#!/usr/bin/env bash

urls=(
  "https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.gz"
  "https://ftp.gnu.org/gnu/binutils/binutils-2.36.1.tar.xz"
)

archives=(
  "gcc-10.2.0.tar.xz"
  "binutils-2.36.1.tar.xz"
)

for url in "${urls[@]}"; do
  if [ ! -f "$archive" ]; then
    curl -O "$url"
  fi
done

for archive in "${archives[@]}"; do
  tar -xvf "$archive"
done

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
if [ "$(echo "$PATH" | grep "$HOME/opt/cross/bin")" == "" ]; then
  export PATH="$PREFIX/bin:$PATH"
fi

rm -rf "{$PREFIX}/*"
cd binutils-2.36.1 || exit 1
mkdir -p build-binutils
cd build-binutils || exit 1
../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
sudo make install
cd ../../

cd gcc-10.2.0 || exit 1
mkdir -p build
cd build || exit 1
../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc
cd ../../
