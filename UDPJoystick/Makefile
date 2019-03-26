CXX=g++
main: UDPSocket.cc Joystick.cc main.cc
	$(CXX) -std=c++14 -Wall UDPSocket.cc Joystick.cc main.cc -o main -I include -L lib -l SDL2
clean:
	rm main
