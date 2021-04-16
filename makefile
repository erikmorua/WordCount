# Nathan Azoulay, cssc3302 Red ID: 821871239
# Erik Morua, cssc3333, Red ID: 816670615
# makefile


wordcount: wordcount.c wordcount.h
	gcc -pthread -lm -o wordcount wordcount.c 

clean:
	rm wordcount