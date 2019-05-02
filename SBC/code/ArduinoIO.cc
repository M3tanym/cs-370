#include "ArduinoIO.h"

#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyACM0"

static bool ioReady;

ArduinoIO::ArduinoIO()
{
  t = std::thread(&ArduinoIO::run, this);
}

ArduinoIO::~ArduinoIO()
{
  running = false;
}

bool ArduinoIO::leftStickEnabled()
{
  return left;
}

bool ArduinoIO::rightStickEnabled()
{
  return right;
}

void ArduinoIO::setStatus(std::string s)
{
  status = s;
}

void ArduinoIO::catchSignal(int)
{
  std::cerr << "received SIGIO signal.\n";
  ioReady = true;
}

void ArduinoIO::run()
{
  ioReady = false;
  int fd, res;
  struct termios newtio;
  //struct sigaction saio;
  char buf[255];

 // open the device to be non-blocking (read will return immediatly)
 fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
 if(fd < 0)
  throw std::runtime_error("Couldn't connect to Arduino!");

  // install the signal handler before making the device asynchronous
  //saio.sa_handler = catchSignal;
  //saio.sa_mask = 0;
  //saio.sa_flags = 0;
  //saio.sa_restorer = NULL;
  //sigaction(SIGIO, &saio, NULL);
  signal(SIGIO, catchSignal);

  // allow the process to receive SIGIO
  fcntl(fd, F_SETOWN, getpid());
  // Make the file descriptor asynchronous
  fcntl(fd, F_SETFL, FASYNC);
  // set new port settings for canonical input processing
  newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR | ICRNL;
  newtio.c_oflag = 0;
  newtio.c_lflag = ICANON;
  newtio.c_cc[VMIN]=1;
  newtio.c_cc[VTIME]=0;
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);

  while(running)
  {
    std::cerr << "waiting...\n";
    usleep(100000);
    // after receiving SIGIO, wait_flag = FALSE, input is available and can be read */
    if(ioReady)
    {
      res = read(fd, buf, 255);
      buf[res] = 0;
      std::cerr << "Input! " << buf << std::endl;
      // wait for more input
      ioReady = false;
    }
  }
}
