CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -g -w
LIBS = -lGLEW -lglfw -lGL -lX11 -pthread -ldl -w
LDFLAGS =
RM = /bin/rm -f

all: out
out: main.o *.o *.h
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS)
*.o: *.cpp
	$(CC) $(CFLAGS) $(LIBS) -c *.cpp
clean:
	$(RM) *.o out
