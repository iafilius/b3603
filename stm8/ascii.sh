#!/bin/sh

# start to find duplicate strings

grep ascii  *.rst > ascii_tmp.rst.arfi


cat ascii_tmp.rst.arfi  | awk -F "ascii" '{print $2}'
