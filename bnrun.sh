#!/bin/bash

set -ex

CXX=g++
CARGS="-g -O0 -o testapp"

${CXX} ${CARGS} unit.cpp
./testapp
