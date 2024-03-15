#!/usr/bin/env zsh
_date="$(date -Iseconds)"

bsdtar -cf "../PAGame-$_date.tar" --exclude "build" --exclude ".cache" --exclude "subprojects/*-*" *
