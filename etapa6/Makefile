CXX = g++
CXXFLAGS = -std=c++11 -Wall

target: etapa6

etapa6: y.tab.c lex.yy.o main.o symbols.o ast.o semantic.o tac.o
	$(CXX) lex.yy.o main.o symbols.o ast.o semantic.o tac.o -o etapa6

y.tab.c: parser.y
	yacc -d parser.y 

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $< -c

lex.yy.cpp: scanner.l
	flex -o lex.yy.cpp scanner.l 

clean:
	rm etapa6 lex.yy.cpp *.o y.tab.c y.tab.h