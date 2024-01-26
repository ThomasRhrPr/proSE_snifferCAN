PAPER= dossier_spec_inc1.tex  # set the path to your TeX file here
SRC_PLANTUML = $(realpath ../schemas/)

PLANTUML = java -jar ../../plantuml.jar
DEPEND_LATEX = .dependLatex
OUTPUT_LATEX = ./build
OUTPUT_PLANTUML = $(SRC_PLANTUML)
SRC_FILES := $(wildcard $(SRC_PLANTUML)/*.plantuml) $(wildcard $(SRC_PLANTUML)/**/*.plantuml)

all: eps latex

latex: 
	xelatex $(PAPER)
eps: 
	$(PLANTUML) -charset UTF-8 -teps -stdrpt:1 -recursive $(SRC_FILES) -o $(OUTPUT_PLANTUML)

clean_eps:
	@rm -rf $(OUTPUT_PLANTUML)/*.eps $(OUTPUT_PLANTUML)/*.pdf

clean_vscode:
	@rm -f *.toc *.out *.log *.lof *.ist *.glo *.fls *.fdb_latexmk *.aux *.acn *.synctex.gz *.pdf

clean: clean_eps clean_latex clean_vscode ## Clean output files

.PHONY: all clean eps clean_eps latex clean_latex

-include $(DEPEND_LATEX)
