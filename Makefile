
SRC=createpw2
CXXFLAGS=-g -Wall
LIBS=-lpthread

all:
	g++ $(CXXFLAGS)  $(SRC).cpp  -o $(SRC) $(LIBS)




clean:
	rm $(SRC)

test:  clean all
	./$(SRC) -d wordfiles/test_seq1.txt -i test/test_2word.wl
