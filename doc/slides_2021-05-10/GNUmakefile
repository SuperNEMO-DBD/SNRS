# Makefile for GNUmake

# SHELL=/bin/sh
# LN=/usr/bin/ln

# SYSTEM=$(shell uname -s)

# ifeq (${SYSTEM},Linux)
# MAKE=make
# CXX=g++
# CPP=cpp
# ARCH=Linux
# endif


THE_DIR=.
XFIG_DIR=$(THE_DIR)/fig
IMG_DIR=$(THE_DIR)/img
PNG_DIR=$(IMG_DIR)
TEX_DIR=$(THE_DIR)
PDFTEX_DIR=$(THE_DIR)/pdftex
PDFTEX_T_DIR=$(THE_DIR)/pdftex_t

# nom du fichier source LaTeX
MAIN=SW_SNRS_2021-05

.PHONY : all
all : talk

.PHONY : printable
printable : ps

SUFFIXES : .tex .fig .eps .png \
   .pdftex_t .pdf

vpath %.fig $(XFIG_DIR)
vpath %.png $(PNG_DIR)
vpath %.tex $(TEX_DIR)
vpath %.pdf        $(PDFTEX_DIR)
vpath %.pdftex_t   $(PDFTEX_T_DIR)

fig_files:=$(notdir $(shell ls $(XFIG_DIR)/*.fig))

fig2pdfsrc_files    :=$(notdir $(shell ls $(XFIG_DIR)/*.fig))
fig2pdftex_files    := $(fig2pdfsrc_files:%.fig=%.pdf)
fig2pdftex_t_files  := $(fig2pdfsrc_files:%.fig=%.pdftex_t)

.PHONY : test
test :
	@echo Test...
	@echo system is $(SYSTEM)
	@echo make is $(MAKE)
	@echo cxx is $(CXX)
	@echo -e  "\nfig2pdfsrc_files   = $(fig2pdfsrc_files)"
	@echo -e  "\nfig2pdftex_t_files = $(fig2pdftex_t_files)"
	@echo -e  "\nfig2pdftex_files   = $(fig2pdftex_files)"

.PHONY : clean
clean : clean_all
	@$(RM) $(FIG2PDF_DIR)/*~

.PHONY : clean_all
clean_all : clean_talk clean_pdftex clean_pdftex_t
	@$(RM) *~

.PHONY : clean_pdftex
clean_pdftex :
	@$(RM) $(PDFTEX_DIR)/*.pdf

.PHONY : clean_pdftex_t
clean_pdftex_t :
	@$(RM) $(PDFTEX_T_DIR)/*.pdftex_t

.PHONY : talk
talk : $(MAIN).pdf

$(MAIN).pdf : $(MAIN).tex  special
	pdflatex $(MAIN).tex
	pdflatex $(MAIN).tex

.PHONY : show
show : $(MAIN).pdf
	evince $(MAIN).pdf &

.PHONY : clean_talk
clean_talk :
	$(RM) $(MAIN).aux $(MAIN).head $(MAIN).log $(MAIN).out
	$(RM) $(MAIN).snm $(MAIN).toc
	$(RM) $(MAIN).pdf $(MAIN).nav

$(MAIN).tex :
	@echo "Nothing to do..."

.PHONY : special
special : images fig2pdftex_t
	@echo "special: rien a faire pour l'instant..."

.PHONY : images
images : #fig2pdf fig2eps
	@echo "images: rien a faire pour l'instant..."

%.pdftex_t : %.fig
	test -d $(PDFTEX_T_DIR) || mkdir -p $(PDFTEX_T_DIR)
	fig2dev -L pdftex_t  -l landscape \
	  -p "\pdfteximgpath/"$*.pdf	  $< \
	  $(PDFTEX_T_DIR)/$(notdir $(@))

%.pdf : %.fig
	test -d $(PDFTEX_DIR) || mkdir -p $(PDFTEX_DIR)
	@echo "building '%.pdf'..."
	fig2dev -L pdftex -l landscape $< $(PDFTEX_DIR)/$(notdir $(@))

.PHONY : fig2pdftex_t
fig2pdftex_t : $(fig2pdftex_t_files) fig2pdftex

.PHONY : fig2pdftex
fig2pdftex : $(fig2pdftex_files)

# end
