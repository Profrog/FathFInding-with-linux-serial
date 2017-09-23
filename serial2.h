
#include <iostream>
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <stdio.h>      // standard input / output functions
#include <string.h>
#include <unistd.h>     // UNIX standard function definitions
using namespace std;

//parameter
int boardNum = 1; //the num of how much you connect device
int *USB = new int[boardNum];
int j = 0;
struct termios tty;
struct termios* tty2 = new termios[boardNum]; //about terminal interface
int outCounter = 0;
speed_t baudrate1 = B9600; //serial io stream speed
int length_size = 256;
int length_size2 = 6;
std::string* Dataline = new string[boardNum]; //getting data in serial save
std::string* porty = new string[boardNum];
string knowing = "";

//string line = "/dev/ttyACM";
//char *buf[256] = new char[boardNum];
//parameter



#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
using namespace std; //for int to string


void setDataline(int index, string Data)
{
  if(index < boardNum)  //set to send data saving for serial, index is port name
  {
    Dataline[index] = Data;
  }
}

string making_String(string a, string b) //plus a string with b string
{
  string forretturning = a + b;
  return forretturning;
}

char* making_pointer(string alpa) //string to char pointer
{
  int size = alpa.length();
  char* returnData = new char[size];

  for(int i = 0 ; i < size ; i++)
  {
    returnData[i] = alpa[i];
  }

  return returnData;
}

string making_pointer2(string alpa, int beta) //only minmum than 10
{
  string line = alpa;
  int someInt = beta;
  char str[256];
  sprintf(str, "%d", someInt);

  int size = alpa.length() + 1;

  for(int i = 0 ; i < size ; i++)
  {
    line += str[i];
  }
  return line;
} //using getting portname


void allDisconnect() //all serial port is closed
{
  for(int j = 0 ; j < boardNum ; j++)
  {
    close(USB[j]);
  }
}

void openingSerial() //serial open,connection start
{
 //if your serial file is other, modifing it
 string line = "/dev/ttyACM";
  j = 0;
  string portname;
  for(int i = 0 ; i < 10 ; i++)
  {
    portname = making_pointer2(line, i);
    cout << portname;
    USB[j] = open( making_pointer(portname), O_RDWR | O_NOCTTY | O_APPEND);

    if(USB[j] > 0)
    {
    cout << "Serial connection start in" << portname << endl;
    porty[j] = portname;
    j++;
    }

    if(j >= boardNum)
      {
        break;
        j = 0;
      }

    else
    {
    cout << "Error " << errno << " opening " << portname << ": " << strerror (errno) << endl;
    }
  }
}

void terminal_connect() //connecting serial port
{
  tty2 = new termios[boardNum];
  for(int j = 0 ; j < boardNum ; j++)
  {
  memset (&tty2[j], 0, sizeof tty2[j]);

  /* Error Handling */
  if ( tcgetattr ( USB[j], &tty2[j] ) != 0 )
  {
  cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
  }

  /* Set Baud Rate */
  cfsetospeed (&tty2[j], baudrate1); //boardrate1 is speed of termainl
  cfsetispeed (&tty2[j], baudrate1);

  /* Setting other Port Stuff */
tty2[j].c_cflag     &=  ~PARENB;        // Make 8n1
tty2[j].c_cflag     &=  ~CSTOPB;
tty2[j].c_cflag     &=  ~CSIZE;
tty2[j].c_cflag     |=  CS8;
tty2[j].c_cflag     &=  ~CRTSCTS;       // no flow control
tty2[j].c_lflag     =   0;          // no signaling chars, no echo, no canonical processing
tty2[j].c_oflag     =   0;                  // no remapping, no delays
tty2[j].c_cc[VMIN]      =   0;                  // read doesn't block
tty2[j].c_cc[VTIME]     =   10;                  // 0.5 seconds read timeout i maked it to 1

tty2[j].c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
tty2[j].c_iflag     &=  ~(IXON | IXOFF | IXANY);// turn off s/w flow ctrl
tty2[j].c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
tty2[j].c_oflag     &=  ~OPOST;              // make raw


  tcflush( USB[j], TCIFLUSH );

  if ( tcsetattr ( USB[j], TCSANOW, &tty2[j] ) != 0)
  {
  cout << "Error " << errno << " from tcsetattr" << endl;
}
  cout << "Serial port " << j << endl;
}
}

bool* count = new bool[boardNum]; //checking already port


string conveyData(int j)
{
  string line = "";
  line = Dataline[j];
  length_size2 = Dataline[j].length();

  return line; //algortihm -> Dataline[i] -> Serial
}

void reading(int j)
{
  //sleep(1);
  cout << "reading Port" << j << endl;
  //USB[j] = 3;
  //checking Serial();
  char buf[256];
  memset (&buf, '$', sizeof buf); //inital with "$$$$$.."
  /* *** READ *** */
  int n = read(USB[j], buf , sizeof(buf)); //it is reading code

  /* Error Handling */
  if (n < 0)
  {
       cout << "Error reading: " << strerror(errno) << endl;
       count[j] = true;
       outCounter++;
       close(USB[j]);
  }


  /* Print what I read... */
  else{
  //printf("%s", buf);
  string data1 = "";

  for(int i = 0; i < 256 ; i++)
  {
    if(buf[i] == '$') //'$' == parsing parameter
    break;

    else
    data1 += buf[i];
  }

  cout << "reading" << data1 << endl;
  knowing = data1;
}
}

void writting(int j)
{
  //sleep(1);
  cout << "writting" << j << endl;
  char cmd[256];
  //USB[j] = 4;
  tcdrain(USB[j]);
  tcflush(USB[j], TCIOFLUSH);
  memset (&cmd, '$', sizeof cmd);
  strcpy(cmd, making_pointer(conveyData(j)));
  int write2 = 0;
  knowing = "";
  write2 = write(USB[j],cmd, sizeof(cmd));

  close(USB[j]);
  //closeDevice(USB[j]);
  //USB[j] = open( making_pointer(porty[j]), O_RDWR | O_NOCTTY | O_APPEND);
  write2 = read(USB[j], cmd, sizeof(cmd));
  cout << "/ngetting data is " << cmd << endl;
  cout << "USB" << USB[j] << endl;

  string data1 = "";
  for(int i = 0; i < 32 ; i++)
  {
    if(cmd[i] == '$')
    break;

    else
    data1 += cmd[i];
  }

  knowing = data1;
  if(write2 < 0)
  {
    cout << "Error writting: " << strerror(errno) << endl;
  }
  else{
    //tcdrain(USB[j]);
    reading(0);
    }
}

void other()
{
  for(int i = 0 ; i < boardNum ; i++)
  {
    writting(i);
    sleep(1);
    reading(i);
    //sleep(1);
  }
}

void closeDevice(int j)
{
  tcflush(USB[j], TCIOFLUSH);
  //close(USB[j]);
}

/* Error Handling */
/*

*/
