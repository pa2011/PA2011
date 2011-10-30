
main.pdf: main.tex $(shell find inhalt)
	pdflatex main.tex
	
.PHONY: all clean purge

all: main.pdf

clean:
	rm -f *.log
	rm -f *.aux
	rm -f *.synctex.gz
	
purge: clean
	rm -f main.pdf
