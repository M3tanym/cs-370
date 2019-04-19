OS := $(shell uname)
ARCH := $(shell uname -m)

ifeq ($(OS), Linux)
  ifeq ($(ARCH), x86_64)
    LEAP_LIBRARY := ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64
  else
    LEAP_LIBRARY := ../lib/x86/libLeap.so -Wl,-rpath,../lib/x86
  endif
else
  # OS X
  LEAP_LIBRARY := ../lib/libLeap.dylib
endif

CXX=/usr/bin/g++



main: main.cc UDPJoystick.cc FingerButtons.cc Joysticks.cc
	$(CXX) -std=c++14 -Wall -g -I../include main.cc UDPJoystick.cc FingerButtons.cc Joysticks.cc -o $@ $(LEAP_LIBRARY)
	make fixLib exName=$@

testGesture: testGesture.cc HandSignal.cc HandSignalCollection.cc
	$(CXX) -std=c++14 -Wall -g -I../include testGesture.cc HandSignal.cc HandSignalCollection.cc -o $@ $(LEAP_LIBRARY)
	make fixLib exName=$@

testFButtons: testFButtons.cc FingerButtons.cc
	$(CXX) -std=c++14 -Wall -g -I../include testFButtons.cc FingerButtons.cc -o $@ $(LEAP_LIBRARY)
	make fixLib exName=$@

testUDPJoystick: testUDPJoystick.cc UDPJoystick.cc Joystick.cc
	$(CXX) -std=c++14 -Wall -g -I../include -l SDL2 testUDPJoystick.cc UDPJoystick.cc Joystick.cc -o $@
	make fixLib exName=$@

fixLib:
ifeq ($(OS), Darwin)
	install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib $(exName)
endif

clean:
	rm -rf main testGesture testFButtons testUDPJoystick *.dSYM
