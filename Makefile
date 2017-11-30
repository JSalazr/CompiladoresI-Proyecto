TARGET=sample1
C_SRCFILES=main.cpp expr_lexer.cpp expr_parser.cpp
OBJ_FILES=${C_SRCFILES:.cpp=.o}
.PHONY: clean

$(TARGET): $(OBJ_FILES)
	g++ -o $@ $(OBJ_FILES)

%.o: %.cpp
	g++ -std=c++11 -c -o $@ $<

run: $(TARGET)
	./$(TARGET) tiny-python-samples/RelOperators.py

clean:
	rm -f *.o
	rm -f $(TARGET)