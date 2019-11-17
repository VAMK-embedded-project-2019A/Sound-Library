GPP = g++
#OPTS =  -I"../include" -L "../include/macosx-gcc/libirrklang.dylib" -pthread


all:
	$(GPP) main.cpp musicplayer.cpp -o main -lmpg123 -lao
	#$(CPP) main.cpp musicplayer.cpp -o main $(OPTS)

clean:
	rm -f main
