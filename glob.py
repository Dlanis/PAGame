#!/usr/bin/python3

from os import walk, path
from sys import argv


def main():
    if(len(argv) != 3):
        print('Invalid number of arguments! Correct: [extension] [directory]')
        return
    
    ext = '.' + argv[1]
    sdir = argv[2]
    
    for root, dirs, files in walk(sdir, topdown=True, onerror=print, followlinks=False):
        for f in files:
            if f.endswith(ext):
                print(f'{path.normpath(path.join(root, f))}', end='\0')


if __name__ == "__main__":
    main()
