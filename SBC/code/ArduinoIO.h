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
    std::string status = "0";
    bool left = true;
    bool right = true;
    bool running = true;
    bool needToWrite = true;
    std::thread t;
    void run();
};

/* setStatus options:
0 - off
1 - white
2 - red
3 - orange
4 - green
5 - turquoise
6 - blue
7 - purple
*/

#endif
