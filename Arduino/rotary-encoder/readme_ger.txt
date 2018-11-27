Projektseite:
http://www.homofaciens.de/technics-base-circuits-encoder-disc_ge_navion.htm

Die Software in diesem Paket ist in einem rein experimentellen Stadium.

"commands-host.c" läuft unter Linux von der Kommandozeile.
Getestet habe ich die Software unter Ubuntu 12.04LTS and 14.04LTS.
Mit dem Programm können Schrittbefehle an den Ardunio Unu übermittelt werden.

Installation: 
(1)Kopiere die Dateien in einen Ordner deiner Wahl.
(2)Kompilliere '*.ino' des Projekts und lade den generierten Code auf den Arduino Uno.
   Mehr Informationen hierzu gibt's hier:
   https://www.arduino.cc/en/Guide/HomePage
(3)Öffne ein Terminal Fenster (Ctrl + T unter Ubuntu) und wechsle in den Ordner in dem sich commands-host.c befindet:
   cd /YourDirectory
(4)Kompilliere den Quellcode mit:
   gcc commands-host.c -o commands-host -lm
(5)Starte das Programm mit:
   ./commands-host

Schaltungen:
digital-servo.pdf kann mit den Dateien "wiper_servo"verwendet werden, wenn ich ein kleines Servo bauen möchtet (z.B. wie in dem Video gezeigt aus einem alten CD-Laufwerk).
