#!/usr/bin/env python 

import os
import sys

def main(path_to_fakebook):
	os.system('python3 ../mma.py ' + path_to_fakebook)

if __name__ == '__main__':
    main(*sys.argv[1:])
