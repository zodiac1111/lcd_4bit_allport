all:
	avr-gcc main.c -o main.elf -Wall -mmcu=atmega8 -Os -DF_CPU=8000000
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
