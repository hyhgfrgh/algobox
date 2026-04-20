#! /bin/bash
pandoc 数据结构wl.md -o 数据结构.pdf \
              --toc --number-sections --pdf-engine=xelatex \
              -V CJKmainfont="Noto Sans CJK SC" \
              -V geometry:margin=2cm

pandoc 图论wl.md -o 图论.pdf \
              --toc --number-sections --pdf-engine=xelatex \
              -V CJKmainfont="Noto Sans CJK SC" \
              -V geometry:margin=2cm

pandoc 数学wl.md -o 数学.pdf \
              --toc --number-sections --pdf-engine=xelatex \
              -V CJKmainfont="Noto Sans CJK SC" \
              -V geometry:margin=2cm

pandoc 字符串wl.md -o 字符串.pdf \
              --toc --number-sections --pdf-engine=xelatex \
              -V CJKmainfont="Noto Sans CJK SC" \
              -V geometry:margin=2cm

pandoc 杂项wl.md -o 杂项.pdf \
              --toc --number-sections --pdf-engine=xelatex \
              -V CJKmainfont="Noto Sans CJK SC" \
              -V geometry:margin=2cm

