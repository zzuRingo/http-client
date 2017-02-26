project : main.o \
		  basicfunction.o \
		  socket.o \
		  glovari.o \
		  download.o

	gcc -o project main.o basicfunction.o socket.o glovari.o download.o -lpthread\
		&& ./project http://data.5sing.kgimg.com/G033/M08/00/13/YQ0DAFXmvDWARPHZAMQZuP5hAs4250.mp3
		#http://5sing.kugou.com/bz/286389vffff9.html

main.o : main.cpp
	gcc -c main.cpp -lpthread

basicfunction.o : basicfunction.cpp
	gcc -c basicfunction.cpp

socket.o : socket.cpp
	gcc -c socket.cpp

glovari.o : glovari.cpp
	gcc -c glovari.cpp

download.o : download.cpp
	gcc -c download.cpp

clean :
	rm project main.o basicfunction.o socket.o download.o