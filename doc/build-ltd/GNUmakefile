.PHONY : all
all : snrs-build-ltd.pdf

.PHONY : show
show : snrs-build-ltd.pdf
	xdg-open $< &

.PHONY : clean
clean :
	$(RM) snrs-build-ltd.pdf
	find . -name "*~" -exec rm -f \{\} \;

snrs-build-ltd.pdf : snrs-build-ltd.rst
	rst2pdf $<

# end

