# Menaka Lashitha Bandara <lashi@club.csse.monash.edu.au>

# Modify these to suit your machine.

LATEX = latex
DVIPS = dvips
DVIPDFM = dvipdfm 

RESOLUTION=600

# VERSION!!!!
VERSION=

# Do not modify from below here, unless you know what you're doing.
DOCNAME = mosp
DEPENDENCIES = introduction.tex methodology.tex

all: postscript

dvi: $(DEPENDENCIES)
	@echo -n "\t[Building DVI] -> "
	@$(LATEX) $(DOCNAME).tex > /dev/null 2>&1 
	@$(LATEX) $(DOCNAME).tex > /dev/null 2>&1 
	#@mv $(DOCNAME).dvi $(DOCNAME)$(VERSION).dvi
	@echo "[Done]"
	@echo -e "\tDO NOT READ DVI FILE DIRECTLY,\n\tIT DOESN'T CONTAIN PROPER LAYOUT UNTIL POSTSCRIPT."
	
postscript: dvi
	@echo -n "\t[Building Postscript] -> "
	@$(DVIPS) -D $(RESOLUTION) -Z $(DOCNAME)$(VERSION).dvi -o $(DOCNAME)$(VERSION).ps 2>> /dev/null
	@echo "[Done]"

pdf: postscript
	@echo -n "\t[Building PDF] -> "
	@$(DVIPDFM) $(DOCNAME)$(VERSION).dvi
	@echo "[Done]"

clean:
	@echo -e "\t[Cleaning Documentation Source]"
	@rm -rf *.ps *.pdf *.dvi *.aux *.log *.toc *.bak
