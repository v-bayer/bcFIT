NAME=proj2

$(NAME).pdf: $(NAME).dvi
	dvipdf $(NAME).dvi

$(NAME).dvi: $(NAME).tex proj2.tex
	latex $(NAME).tex	

clean:
	-rm $(NAME).dvi $(NAME).aux $(NAME).toc $(NAME).log $(NAME).out
