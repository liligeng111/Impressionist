#!/usr/bin/python2.7

import os
import re

here = os.path.dirname(os.path.abspath(__file__))

def get_list():
    return [file for file in os.listdir(here) if file != 'rename.py']

def mapping(name):
    return name.lower()

def convert(file):
    return os.rename(file, mapping(file))

def main():
    li = get_list()
    map(convert, li)

if __name__ == '__main__':
    main()
