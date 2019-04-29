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



main: main.cc Dongle.cc FingerButtons.cc Joysticks.cc Dpad.cc
	$(CXX) -std=c++14 -Wall -g -I../include main.cc FingerButtons.cc Joysticks.cc Dongle.cc Dpad.cc -o $@ $(LEAP_LIBRARY)
	make fixLib exName=$@

testDongle: testDongle.cc Dongle.cc USBController.cc
	$(CXX) -std=c++14 -Wall -g -I../include -l SDL2 testDongle.cc Dongle.cc USBController.cc -o $@
	make fixLib exName=$@

testFingerButtons: testFingerButtons.cc FingerButtons.cc
	$(CXX) -std=c++14 -Wall -g -I../include testFingerButtons.cc FingerButtons.cc -o $@ $(LEAP_LIBRARY)
	make fixLib exName=$@

adjustFingerButtons: adjustFingerButtons.cc FingerButtons.cc
	$(CXX) -std=c++14 -Wall -g -I../include adjustFingerButtons.cc FingerButtons.cc -o $@ $(LEAP_LIBRARY)
	make fixLib exName=$@

testJoysticks: testJoysticks.cc Joysticks.cc
	$(CXX) -std=c++14 -Wall -g -I../include testJoysticks.cc Joysticks.cc -o $@ $(LEAP_LIBRARY)
	make fixLib exName=$@

testDpad: testDpad.cc Dpad.cc
	$(CXX) -std=c++14 -Wall -g -I../include testDpad.cc Dpad.cc -o $@ $(LEAP_LIBRARY)
	make fixLib exName=$@

fixLib:
ifeq ($(OS), Darwin)
	install_name_tool -change @loader_path/libLeap.dylib ../lib/libLeap.dylib $(exName)
endif

clean:
	rm -rf main testGesture testFButtons testJoysticks testDongle testDpad *.dSYM
