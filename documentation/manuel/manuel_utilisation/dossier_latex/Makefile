PAPER= manuel_utilisation.tex  # set the path to your TeX file here
SRC_PLANTUML = $(realpath ../schemas/)

PLANTUML = java -jar ../../plantuml.jar
DEPEND_LATEX = .dependLatex
OUTPUT_LATEX = ./build
OUTPUT_PLANTUML = $(SRC_PLANTUML)
SRC_FILES := $(wildcard $(SRC_PLANTUML)/*.plantuml) $(wildcard $(SRC_PLANTUML)/**/*.plantuml)

all: latex

latex: 
	xelatex $(PAPER)

clean_vscode:
	@rm -f *.toc *.out *.log *.lof *.ist *.glo *.fls *.fdb_latexmk *.aux *.acn *.synctex.gz *.pdf

clean: clean_latex clean_vscode ## Clean output files

.PHONY: all clean latex clean_latex

-include $(DEPEND_LATEX)
