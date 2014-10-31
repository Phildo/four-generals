#!/bin/bash
amake build
../bin/pc.out > out/1.out &
../bin/pc.out > out/2.out &
../bin/pc.out > out/3.out &
../bin/pc.out > out/4.out &

