all: Cparser.c
	-@gcc $< -o Cparser
clean:
	-@rm -Rf *~ *.o *.out Cparser 

