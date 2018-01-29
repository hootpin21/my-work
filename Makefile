# -*- MakeFile -*-

run: build/bbc-microbit-classic-gcc/source/microbit-samples-combined.hex
	cp build/bbc-microbit-classic-gcc/source/microbit-samples-combined.hex /run/media/billy/MICROBIT/

build/bbc-microbit-classic-gcc/source/microbit-samples-combined.hex: source/main.cpp
	yt build
