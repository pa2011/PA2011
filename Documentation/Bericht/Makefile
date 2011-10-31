ifdef MSYS_HOME
	findCommand=$(MSYS_HOME)/bin/find
else
	findCommand=find
endif

main.pdf: main.tex $(shell $(findCommand) inhalt)
	pdflatex main.tex
	
.PHONY: all clean purge

all: main.pdf

clean:
	rm -f *.log
	rm -f *.aux
	rm -f *.synctex.gz
	rm -f inhalt/*.log
	rm -f inhalt/*.aux
	rm -f inhalt/*.synctex.gz
	
purge: clean
	rm -f main.pdf
