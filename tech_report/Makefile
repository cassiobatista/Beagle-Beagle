source=tech_report

all: 
	latex $(source).tex
	bibtex $(source).aux
	latex $(source).tex
	latex $(source).tex
	dvipdf $(source).dvi
	make clean
	clear

clean:
	rm -f *.aux *.dvi *.log *.lof *.out *.toc *.bbl *.blg
