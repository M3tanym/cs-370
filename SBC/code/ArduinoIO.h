#ifndef ArduinoIO_INCLUDED
#define ArduinoIO_INCLUDED

#include <string>
#include <thread>

class ArduinoIO
{
  public:
    ArduinoIO();
    ~ArduinoIO();
    bool leftStickEnabled();
    bool rightStickEnabled();
    void setStatus(std::string s);
  private:
    std::string status = "";
    bool left = true;
    bool right = true;
    bool running = true;
    std::thread t;
    void run();
};

#endif
