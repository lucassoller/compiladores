CXX = g++
CXXFLAGS = -std=c++11 -Wall

target: etapa2

etapa2: y.tab.c lex.yy.o main.o symbols.o
	$(CXX) lex.yy.o main.o symbols.o -o etapa2

y.tab.c: parser.y
	yacc -d parser.y 

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $< -c

lex.yy.cpp: scanner.l
	flex -o lex.yy.cpp scanner.l 

clean:
	rm etapa2 lex.yy.cpp *.o

