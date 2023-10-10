#!/bin/sh

gcc -o generate-list `pkg-config --cflags --libs gio-2.0` generate-list.c
