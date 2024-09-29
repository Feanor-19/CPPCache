#!/bin/bash

sudo perf record $1 -t
sudo perf report -n -f
