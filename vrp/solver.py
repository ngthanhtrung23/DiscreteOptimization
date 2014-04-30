#!/usr/bin/python
# -*- coding: utf-8 -*-
from subprocess import Popen, PIPE


def solve_it(output_file):
    process = Popen(['./copy', 'output.txt'], stdout=PIPE)
    (stdout, stderr) = process.communicate()

    return stdout.strip()


import sys

if __name__ == '__main__':
    if len(sys.argv) > 1:
        output_file = sys.argv[1].strip()
        print solve_it(output_file)
    else:
        print 'This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/gc_4_1)'
