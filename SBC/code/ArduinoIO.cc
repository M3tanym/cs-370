#include "ArduinoIO.h"

#include <iostream>
#include <unistd.h>

ArduinoIO::ArduinoIO()
{
  t = std::thread(&ArduinoIO::run, this);
}

ArduinoIO::~ArduinoIO()
{
  std::cerr << "dtor!\n";
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
  arduino.open("/dev/ttyACM0", std::ios::in | std::ios::out);
  while(running)
  {
    std::string s = "";
    alarm(1);
    getline(arduino, s);
    alarm(0);
    if(s[1] == ':' && s.size() == 3)
    {
      std::cerr << s << std::endl;
    }
  }
  arduino.close();
}
