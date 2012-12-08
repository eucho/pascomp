#ifndef GRAMMAR_TAB_H
# define GRAMMAR_TAB_H

# ifndef YYSTYPE
#  define YYSTYPE int
# endif
# define	ID	257
# define	NUM	258
# define	ADDOP	259
# define	ASSIGNOP	260
# define	MULOP	261
# define	RELOP	262
# define	ARRAY	263
# define	PBEGIN	264
# define	BOOLEAN	265
# define	DO	266
# define	ELSE	267
# define	END	268
# define	FALSE	269
# define	FUNCTION	270
# define	IF	271
# define	INTEGER	272
# define	NOT	273
# define	OF	274
# define	PROCEDURE	275
# define	PROGRAM	276
# define	READ	277
# define	REAL	278
# define	RECORD	279
# define	THEN	280
# define	TRUE	281
# define	VAR	282
# define	WHILE	283
# define	WRITE	284
# define	AND	285
# define	OR	286
# define	ADD	287
# define	SUB	288
# define	MUL	289
# define	DVD	290
# define	LT	291
# define	LE	292
# define	EQ	293
# define	GE	294
# define	GT	295
# define	NE	296
# define	DIV	297
# define	MOD	298


extern YYSTYPE yylval;

#endif /* not GRAMMAR_TAB_H */
