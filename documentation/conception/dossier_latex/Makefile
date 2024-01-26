PAPER = dossier_conception.tex  # set the path to your TeX file here
SRC_PLANTUML = $(realpath ../schemas/Conception_detaillee/)
SRC_ANIMUML = $(realpath ../animUML/)

PLANTUML = java -jar ../../plantuml.jar
DEPEND_LATEX = .dependLatex
OUTPUT_LATEX = ./build
OUTPUT_PLANTUML = $(SRC_PLANTUML)
SRC_FILES := $(wildcard $(SRC_PLANTUML)/*.plantuml) $(wildcard $(SRC_PLANTUML)/*/*.plantuml)
SVG_FILES := $(patsubst %.plantuml,%.svg,$(SRC_FILES))
PDF_FILES := $(patsubst %.svg,%.pdf,$(SVG_FILES))

all: figures animuml latex

animuml:
	$(MAKE) example.tex -C ../animUML/

latex: $(PDF_FILES)
	xelatex $(PAPER)

figures: $(SVG_FILES) $(PDF_FILES)

$(SVG_FILES): $(SRC_FILES)
	plantuml -charset UTF-8 -tsvg -stdrpt:1 -recursive $^ -o $(OUTPUT_PLANTUML)

%.svg: %.plantuml
	plantuml -charset UTF-8 -tsvg -o $(OUTPUT_PLANTUML) $<

%.pdf: %.svg
	@echo "Converting $< to $@ using rsvg-convert"
	rsvg-convert -f pdf $< -o $@

clean_figures:
	@echo "Cleaning up..."
	rm -f $(SVG_FILES) $(PDF_FILES)

clean_vscode:
	@rm -f *.toc *.out *.log *.lof *.ist *.glo *.fls *.fdb_latexmk *.aux *.acn *.synctex.gz *.pdf *.svg

clean_animuml:
	$(MAKE) clean -C ../animUML/

clean: clean_figures clean_vscode clean_animuml ## Clean output files

.PHONY: all clean figures clean_figures latex

-include $(DEPEND_LATEX)
