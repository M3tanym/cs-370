#include "ArduinoIO.h"

#include <iostream>
#include <unistd.h>
#include <fstream>

ArduinoIO::ArduinoIO()
{
  t = std::thread(&ArduinoIO::run, this);
}

ArduinoIO::~ArduinoIO()
{
  running = false;
  std::terminate(); // kill all threads
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
  std::fstream arduino("/dev/ttyACM0", std::ios::in | std::ios::out);
  while(running)
  {
    std::cerr << "!\n";
    std::string s = "";
    getline(arduino, s);
    if(s[1] == ':' && s.size() == 3)
    {
      std::cerr << s << std::endl;
    }
  }
  arduino.close();
}
