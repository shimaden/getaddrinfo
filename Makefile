EXEC = getaddrinfo
OBJS = getaddrinfo.o

all: $(EXEC)

getaddrinfo: $(OBJS)
	g++ -Wall -o getaddrinfo getaddrinfo.o

.cpp.o:
	g++ -Wall -c $<

getaddrinfo.o: getaddrinfo.cpp

clean:
	rm -f *~ $(EXEC) $(OBJS)
