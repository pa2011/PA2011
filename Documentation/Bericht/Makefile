ifdef MSYS_HOME
	findCommand=$(MSYS_HOME)/bin/find
else
	findCommand=find
endif

main.pdf: main.tex $(shell $(findCommand) inhalt)  $(shell $(findCommand) src)
	pdflatex main.tex
	makeindex -s main.ist -t main.glg -o main.gls main.glo
	pdflatex main.tex
	
.PHONY: all clean purge

all: main.pdf

clean:
	rm -f *.log
	rm -f *.aux
	rm -f *.lof
	rm -f *.toc
	rm -f *.acn
	rm -f *.glo
	rm -f *.gls
	rm -f *.ist
	rm -f *.out
	rm -f *.synctex.gz
	rm -f inhalt/*.log
	rm -f inhalt/*.aux
	rm -f inhalt/*.lof
	rm -f inhalt/*.toc
	rm -f inhalt/*.acn
	rm -f inhalt/*.glo
	rm -f inhalt/*.gls
	rm -f inhalt/*.ist
	rm -f inhalt/*.out
	rm -f inhalt/*.synctex.gz
	
purge: clean
	rm -f main.pdf
