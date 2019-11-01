CPP = g++
#OPTS =  -I"../include" -L"/usr/lib" ../bin/macosx-gcc/libirrklang.dylib -pthread
OPTS =  -I"../include" -L"/usr/lib" musicplayer.h -pthread


all:
	$(CPP) main.cpp -o main $(OPTS)

clean:
	rm main
