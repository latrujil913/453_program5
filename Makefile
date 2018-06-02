#Change this variable to point to your Arduino device
#Mac - it may be different
#DEVICE = /dev/tty.usbmodemFD121

#Linux (/dev/ttyACM0 or possibly /dev/ttyUSB0)
#DEVICE = /dev/ttyACM0

#Windows
DEVICE = COM7

#default target to compile the code
program_5: program5.c SdReader.c os_util.c serial.c os.c synchro.c #serial.c ext2.c
	avr-gcc -std=gnu99 -mmcu=atmega2560 -DF_CPU=16000000 -O2 -o program5.elf program5.c SdReader.c os_util.c serial.c os.c synchro.c #ext2.c
	avr-objcopy -O ihex program5.elf program5.hex
	avr-size program5.elf

#flash the Arduino with the program
program: program5.hex
	#Mac
	avrdude -D -pm2560 -P $(DEVICE) -c wiring -F -u -U flash:w:program5.hex

#remove build files
clean:
	rm -fr *.elf *.hex *.o
