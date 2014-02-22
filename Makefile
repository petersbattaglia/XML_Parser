all: parser_test

parser_test: parser_test.cpp Parser.cpp Node.cpp Text.cpp String.cpp Element.cpp
	g++ parser_test.cpp Parser.cpp Node.cpp Text.cpp String.cpp Element.cpp -o parser_test

clean:
	rm -f *.o parser_test

