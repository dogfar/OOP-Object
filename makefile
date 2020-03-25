IPATH = -I"C:\cs\curl-7.60.0-win64-mingw\include" 
LPATH = -L"C:\cs\curl-7.60.0-win64-mingw\lib"

all: main 

main: main.o RottenTomatoCrawler.o BaseCrawler.o Catcher.o BaseObject.o BaseData.o main.cpp RottenTomatoCrawler.cpp BaseCrawler.cpp Catcher.cpp BaseData.cpp
	g++ -o main main.o RottenTomatoCrawler.o BaseCrawler.o Catcher.o BaseObject.o BaseData.o -std=c++11 $(IPATH) $(LPATH) -lcurl

%.o: %.cpp
	g++ -c -o $@ $^ -std=c++11 $(IPATH) $(LPATH) -lcurl

clean:
	rm *.o

clean = .PHONY
