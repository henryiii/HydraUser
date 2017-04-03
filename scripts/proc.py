#!/usr/bin/env python
from __future__ import print_function

from plumbum import local
import re


EPS = local['./EPS_CUDA']
OPS = local['./OPS_CUDA']

for i in range(1,16):
    for _ in range(10):
        n = i*1000000
        eps = EPS('-n', n)
        ops = OPS('-n', n)
        print(
                n,
                eps.splitlines()[0].split(':')[1],
                eps.splitlines()[1].split(':')[1],
                ops.splitlines()[0].split(':')[1],
                ops.splitlines()[1].split(':')[1],
                sep=',')

