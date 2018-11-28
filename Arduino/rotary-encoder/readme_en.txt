Projekt page:
http://www.homofaciens.de/technics-base-circuits-encoder-disc_en_navion.htm

The software in this package is highly experimental!

"commands-host.c" runs in the terminal of a Linux computer.
I have tested the software with Ubuntu 12.04LTS and 14.04LTS.
You can send stepper commands to the Arduino using this software.

Installation: 
(1)Copy the files to a directory of your choice.
(2)Compile '*.ino' of the project and load the resulting code to the Arduino Uno.
   More information about how to upload a sketch to your Arduino can be fond on:
   https://www.arduino.cc/en/Guide/HomePage
(3)Open a terminal (Ctrl + T in Ubuntu) and change to your installation directory:
   cd /YourDirectory
(4)Compile the source with:
   gcc commands-host.c -o commands-host -lm
(5)Start the program with:
   ./commands-host

Circuits:
digital-servo.pdf can be used with wiper_servo if you intend to build a low power servo (e.g. from an old optical drive as demonstrated in the video).
