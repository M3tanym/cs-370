#include "ArduinoIO.h"

#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

ArduinoIO::ArduinoIO()
{
  t = std::thread(&ArduinoIO::run, this);
}

ArduinoIO::~ArduinoIO()
{
  running = false;
  t.join();
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

void ArduinoIO::run()
{
  int fd = open("/dev/ttyACM0", O_RDWR);
  if(fd < 0)
    throw std::runtime_error("Couldn't connect to Arduino!");

  // Create new termios struct
  struct termios tty;
  memset(&tty, 0, sizeof tty);
  // Read in existing settings, and handle any error
  if(tcgetattr(fd, &tty) != 0)
    throw std::runtime_error("Couldn't read serial port settings");

  tty.c_cflag &= ~PARENB;  // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB;  // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag |= CS8;      // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;    // Disable echo
  tty.c_lflag &= ~ECHOE;   // Disable erasure
  tty.c_lflag &= ~ECHONL;  // Disable new-line echo
  tty.c_lflag &= ~ISIG;    // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // No special handling of received bytes
  tty.c_oflag &= ~OPOST;   // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR;   // Prevent conversion of newline to carriage return/line feed
  tty.c_cc[VTIME] = 2;    // Wait for up to 2 deciseconds, returning as soon as any data is received
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);

  // Save tty settings, also checking for error
  if(tcsetattr(fd, TCSANOW, &tty) != 0)
    throw std::runtime_error("Couldn't save serial port settings");

  while(running)
  {
    char buf[255];
    int len = read(fd, buf, 255);
    buf[len] = 0;
    if(len > 3 && buf[1] == ':')
    {
      bool l = buf[0] == '0';
      bool r = buf[2] == '0';
      std::cerr << "Input! " << std::boolalpha << l << ", " << r << std::endl;
      setStatus(l ? "0" : "2");
      std::cerr << "writing " << status << std::endl;
      write(fd, status.c_str(), status.size() + 1);
    }
  }
  close(fd);
}
