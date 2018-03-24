#!/bin/bash
gcc -fdump-rtl-all-slim-stats -dA -dD -dP -fdump-ipa-all -fdump-statistics-details -std=gnu11 -O3 -lm fb6hil.c -o fb6hil
