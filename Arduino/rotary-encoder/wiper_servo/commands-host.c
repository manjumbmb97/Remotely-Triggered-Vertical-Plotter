//Project: CNC v2
//Homepage: www.HomoFaciens.de
//Author Norbert Heinz
//Version: 1.0
//Creation date: 23.12.2015
//This program is free software you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3 of the License.
//This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//For a copy of the GNU General Public License see http://www.gnu.org/licenses/
//
//compile with gcc commands-host.c -o commands-host -lm
//For details see:
//http://www.HomoFaciens.de

#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>



#define BAUDRATE B115200
//#define ARDUINOPORT "/dev/ttyUSB0"
#define ARDUINOPORT "/dev/ttyACM0"
#define FALSE 0
#define TRUE 1

#define BUFFER_MAX 30

#define PI 3.1415927

int KeyHit = 0;
int KeyCode[5];
int SingleKey=0;


int  MaxRows = 24;
int  MaxFileRows = 10;
int  FilesFound = 0;
int  MaxCols = 80;
int  MessageX = 1;
int  MessageY = 24;
long MoveLength = 1;


int fd = 0;
struct termios TermOpt;




//+++++++++++++++++++++++ Start gotoxy ++++++++++++++++++++++++++
//Thanks to 'Stack Overflow', found on http://www.daniweb.com/software-development/c/code/216326
int gotoxy(int x, int y) {
  char essq[100]; // String variable to hold the escape sequence
  char xstr[100]; // Strings to hold the x and y coordinates
  char ystr[100]; // Escape sequences must be built with characters
   
  //Convert the screen coordinates to strings.
  sprintf(xstr, "%d", x);
  sprintf(ystr, "%d", y);
   
  //Build the escape sequence (vertical move).
  essq[0] = '\0';
  strcat(essq, "\033[");
  strcat(essq, ystr);
   
  //Described in man terminfo as vpa=\E[%p1%dd. Vertical position absolute.
  strcat(essq, "d");
   
  //Horizontal move. Horizontal position absolute
  strcat(essq, "\033[");
  strcat(essq, xstr);
  // Described in man terminfo as hpa=\E[%p1%dG
  strcat(essq, "G");
   
  //Execute the escape sequence. This will move the cursor to x, y
  printf("%s", essq);
  return 0;
}
//------------------------ End gotoxy ----------------------------------

//+++++++++++++++++++++++ Start clrscr ++++++++++++++++++++++++++
void clrscr(int StartRow, int EndRow) {
  int i, i2;
  
  if (EndRow < StartRow){
    i = EndRow;
    EndRow = StartRow;
    StartRow = i;
  }
  gotoxy(1, StartRow);
  for (i = 0; i <= EndRow - StartRow; i++){
    for(i2 = 0; i2 < MaxCols; i2++){
      printf(" ");
    }
    printf("\n");
  }
}
//----------------------- End clrscr ----------------------------

//+++++++++++++++++++++++ Start kbhit ++++++++++++++++++++++++++++++++++
//Thanks to Undertech Blog, http://www.undertec.de/blog/2009/05/kbhit_und_getch_fur_linux.html
int kbhit(void) {

   struct termios term, oterm;
   int fd = 0;
   int c = 0;
   
   tcgetattr(fd, &oterm);
   memcpy(&term, &oterm, sizeof(term));
   term.c_lflag = term.c_lflag & (!ICANON);
   term.c_cc[VMIN] = 0;
   term.c_cc[VTIME] = 1;
   tcsetattr(fd, TCSANOW, &term);
   c = getchar();
   tcsetattr(fd, TCSANOW, &oterm);
   if (c != -1)
   ungetc(c, stdin);

   return ((c != -1) ? 1 : 0);

}
//------------------------ End kbhit -----------------------------------

//+++++++++++++++++++++++ Start getch ++++++++++++++++++++++++++++++++++
//Thanks to Undertech Blog, http://www.undertec.de/blog/2009/05/kbhit_und_getch_fur_linux.html
int getch(){
   static int ch = -1, fd = 0;
   struct termios new, old;

   fd = fileno(stdin);
   tcgetattr(fd, &old);
   new = old;
   new.c_lflag &= ~(ICANON|ECHO);
   tcsetattr(fd, TCSANOW, &new);
   ch = getchar();
   tcsetattr(fd, TCSANOW, &old);

//   printf("ch=%d ", ch);

   return ch;
}
//------------------------ End getch -----------------------------------

//++++++++++++++++++++++ Start MessageText +++++++++++++++++++++++++++++
void MessageText(char *message, int x, int y, int alignment){
  int i;
  char TextLine[300];

  clrscr(y, y);
  gotoxy (x, y);
  
  TextLine[0] = '\0';
  if(alignment == 1){
    for(i=0; i < (MaxCols - strlen(message)) / 2 ; i++){
      strcat(TextLine, " ");
    }
  }
  strcat(TextLine, message);
  
  printf("%s\n", TextLine);
}
//-------------------------- End MessageText ---------------------------

//++++++++++++++++++++++ Start PrintRow ++++++++++++++++++++++++++++++++
void PrintRow(char character, int y){
  int i;
  gotoxy (1, y);
  for(i=0; i<MaxCols;i++){
    printf("%c", character);
  }
}
//-------------------------- End PrintRow ------------------------------

//+++++++++++++++++++++++++ ErrorText +++++++++++++++++++++++++++++
void ErrorText(char *message){
  clrscr(MessageY + 2, MessageY + 2);
  gotoxy (1, MessageY + 2);  
  printf("Last error: %s", message);
}
//----------------------------- ErrorText ---------------------------

//+++++++++++++++++++++++++ PrintMenue_01 ++++++++++++++++++++++++++++++
void PrintMenue_01(){
  char TextLine[300];
  
   clrscr(1, MessageY-2);
   MessageText("*** Main menu plotter ***", 1, 1, 1);
   sprintf(TextLine, "M            - toggle move length, current value = %ld step(s)", MoveLength);
   MessageText(TextLine, 10, 3, 0);
   MessageText("Cursor right - move motor clockwise", 10, 4, 0);
   MessageText("Cursor left  - move motor counterclockwise", 10, 5, 0);

   MessageText("Esc          - leave program", 10, 18, 0);
   
}
//------------------------- PrintMenue_01 ------------------------------



//+++++++++++++++++++++++ Start readport ++++++++++++++++++++++++++
char  readport(void){
  int n;
	char buff;
  n = read(fd, &buff, 1);
  if(n > 0){
    return buff;
  }
  return 0;
}
//------------------------ End readport ----------------------------------

//+++++++++++++++++++++++ Start sendport ++++++++++++++++++++++++++
void sendport(unsigned char ValueToSend){
  int n;

  n = write(fd, &ValueToSend, 1);
  
  if (n < 0){
    ErrorText("write() of value failed!\r");
  }
  else{
    while(readport() != 'r');
  }
  
}
//------------------------ End sendport ----------------------------------

//+++++++++++++++++++++++ Start openport ++++++++++++++++++++++++++
void openport(void){
    
    fd = open(ARDUINOPORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
      ErrorText("init_serialport: Unable to open port ");
    }
    
    if (tcgetattr(fd, &TermOpt) < 0) {
      ErrorText("init_serialport: Couldn't get term attributes");
    }
    speed_t brate = BAUDRATE; // let you override switch below if needed

    cfsetispeed(&TermOpt, brate);
    cfsetospeed(&TermOpt, brate);

    // 8N1
    TermOpt.c_cflag &= ~PARENB;
    TermOpt.c_cflag &= ~CSTOPB;
    TermOpt.c_cflag &= ~CSIZE;
    TermOpt.c_cflag |= CS8;
    // no flow control
    TermOpt.c_cflag &= ~CRTSCTS;

    TermOpt.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    TermOpt.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    TermOpt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    TermOpt.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    TermOpt.c_cc[VMIN]  = 0;
    TermOpt.c_cc[VTIME] = 20;
    
    if( tcsetattr(fd, TCSANOW, &TermOpt) < 0) {
      ErrorText("init_serialport: Couldn't set term attributes");
    }

} 
//------------------------ End openport ----------------------------------



//######################################################################
//################## Main ##############################################
//######################################################################

int main(int argc, char **argv){

  int MenueLevel = 0;
  struct winsize terminal;
  int i=0;

  openport();

  printf("\n\nWaiting for 'X' from Arduino (Arduino pluged in?)...\n");

  //Wait for 'X' from Arduino
  while(readport() != 'X');


  if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal)<0){
    printf("Can't get size of terminal window");
  }
  else{
    MaxRows = terminal.ws_row;
    MaxCols = terminal.ws_col;
    MessageY = MaxRows-3;
  }

  clrscr(1, MaxRows);
  PrintRow('-', MessageY - 1);
  PrintMenue_01();


  while (1){
    MessageText("Waiting for key press.", MessageX, MessageY, 0);
    
    i=0;
    SingleKey = 1;
    KeyCode[0] = 0;
    KeyCode[1] = 0;
    KeyCode[2] = 0;
    KeyCode[3] = 0;
    KeyCode[4] = 0;
    KeyHit = 0;
    while (kbhit()){
      KeyHit = getch();
      KeyCode[i] = KeyHit;
      i++;
      if(i == 5){
        i = 0;
      }
      if(i > 1){
        SingleKey = 0;
      }
    }
    if(SingleKey == 0){
      KeyHit = 0;
    }

    //Move servo clockwise
    if(KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 68 && KeyCode[3] == 0 && KeyCode[4] == 0){
      int iPlus = 1;
      if(MoveLength >= 100){
        iPlus = 100;
      }
      if(MoveLength >= 1000){
        iPlus = 1000;
      }
      for(i = 0; i < MoveLength; i+=iPlus){
        if(MoveLength - i < 1000){
          iPlus = 100;
        }
        if(MoveLength - i < 100){
          iPlus = 1;
        }
        if(iPlus == 1000){
          sendport(17);
        }
        if(iPlus == 100){
          sendport(15);
        }
        if(iPlus == 10){
          sendport(13);
        }
        if(iPlus == 1){
          sendport(19);
        }
      }
    }

    //Move servo counterclockwise
    if(KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 67 && KeyCode[3] == 0 && KeyCode[4] == 0){
      int iPlus = 1;
      if(MoveLength >= 100){
        iPlus = 100;
      }
      for(i = 0; i < MoveLength; i+=iPlus){
        if(MoveLength - i < 1000){
          iPlus = 100;
        }
        if(MoveLength - i < 100){
          iPlus = 1;
        }
        if(iPlus == 1000){
          sendport(18);
        }
        if(iPlus == 100){
          sendport(16);
        }
        if(iPlus == 10){
          sendport(14);
        }
        if(iPlus == 1){
          sendport(20);
        }
      }
    }
    
    if(KeyHit == 'm'){
      if(MoveLength == 1000){
        MoveLength = 0;
      }
      if(MoveLength == 100){
        MoveLength = 1000;
      }
      if(MoveLength == 16){
        MoveLength = 100;
      }
      if(MoveLength == 8){
        MoveLength = 16;
      }
      if(MoveLength == 1){
        MoveLength = 8;
      }
      if(MoveLength == 0){
        MoveLength = 1;
      }
      PrintMenue_01();
    }
            
    if(KeyHit == 27){
      if(MenueLevel == 0){
        clrscr(MessageY + 1, MessageY + 1);
        MessageText("Exit program (y/n)?", MessageX, MessageY + 1, 0);
        while(KeyHit != 'y' && KeyHit != 'n'){
          KeyHit = getch();
          if(KeyHit == 'y'){
            exit(0);
          }
        }
      }
      clrscr(MessageY + 1, MessageY + 1);
    }
  }

  return 0;
}
