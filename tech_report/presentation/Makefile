src=present

all:
	latex $(src).tex
	latex $(src).tex
	dvipdf $(src).dvi
	make clean

clean:
	rm -f *.aux *.log *.out *.dvi *.nav *.snm *.toc *.log *.vrb
