bison -d grammar.y
flex word.l
cc grammar.tab.c lex.yy.c -o pascomp -g
