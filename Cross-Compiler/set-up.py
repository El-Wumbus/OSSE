#!/usr/bin/env python3

import os
import sys
import subprocess


def main() -> int:
    subprocess.run(
        ["curl", "-O", "https://ftp.gnu.org/gnu/binutils/binutils-2.38.tar.xz"], check=True)
    subprocess.run(["tar", "-xJvf", "binutils-2.38.tar.xz"], check=True)

    subprocess.run(
        ["curl", "-O", "https://ftp.gnu.org/gnu/gcc/gcc-10.4.0/gcc-10.4.0.tar.xz"], check=True)
    subprocess.run(["tar", "-xJvf", "gcc-10.4.0.tar.xz"], check=True)


