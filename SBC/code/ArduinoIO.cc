#include "ArduinoIO.h"
#include <iostream>
ArduinoIO::ArduinoIO()
{
  arduino.open("/dev/ttyACM0", std::ios::in | std::ios::out);
  t = std::thread(&ArduinoIO::run, this);
}

ArduinoIO::~ArduinoIO()
{
  running = false;
  arduino.close();
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
  while(running)
  {
    std:: string s = "";
    getline(arduino, s);
    if(s[1] == ':' && s.size() == 3)
    {
      std::cout << s << std::endl;
    }
  }
}
