.PHONY : all
all : snrs-build-fsf.pdf

.PHONY : show
show : snrs-build-fsf.pdf
	xdg-open $< &

.PHONY : clean
clean :
	$(RM) snrs-build-fsf.pdf
	find . -name "*~" -exec rm -f \{\} \;

snrs-build-fsf.pdf : snrs-build-fsf.rst
	rst2pdf $<

# end

