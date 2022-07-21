#!/usr/bin/env python3
import sys

if len(sys.argv) != 3:
    print("Usage: replace_file_extention.py <file> <extention>")
    sys.exit(1)

print(sys.argv[1].split(".")[0] + sys.argv[2])
