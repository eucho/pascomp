%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Grammar.h"

#define STRING_MAX 255
 
extern int yylval;
extern int push(char* temp);
extern FILE* fout;
extern char id_str[STRING_MAX];

int yywrap(void);
int i;
int no;//���ű��
%}

/////////////////////////////////////////////////////////////////////////////
/*���涨��ʽ*/

white [ \t\n]
letter [a-zA-Z]
digit [0-9]
digits {digit}{digit}*
id {letter}({letter}|{digit})*
optional_fraction (\.{digits})?
optional_exponent ([Ee][+\-]?{digits})?
num {digits}{optional_fraction}{optional_exponent}
%x COMMENT1
%x COMMENT2
%%
/////////////////////////////////////////////////////////////////////////////
/*���򲿷�*/


//ע��
\{                 {;;BEGIN COMMENT1;}
<COMMENT1>\}       {;;BEGIN 0;}
<COMMENT1>\n       {;;}
<COMMENT1>[^\}]+   {;;}

"/*"               {;;BEGIN COMMENT2;}
<COMMENT2>"*/"     {;;BEGIN 0;}
<COMMENT2>\n       {;;}
<COMMENT2>[^"*/"]+ {;;}

//�ؼ���
[aA][nN][dD]						 {;;no = AND;return (MULOP);}
[aA][rR][rR][aA][yY]				 {;;return (ARRAY);}
[bB][eE][gG][iI][nN]				 {;;return (PBEGIN);}
[bB][oO][oO][lL][eE][aA][nN]		 {;;return (BOOLEAN);}
[dD][oO]							 {;;return (DO);}
[eE][lL][sS][eE]					 {;;return (ELSE);}
[eE][nN][dD]						 {;;return (END);}
[fF][aA][lL][sS][eE]				 {;;return (FALSE);}
[fF][uU][nN][cC][tT][iI][oO][nN]	 {;;return (FUNCTION);}
[iI][fF]							 {;;return (IF);}
[iI][nN][tT][eE][gG][eE][rR]		 {;;return (INTEGER);}
[nN][oO][tT]						 {;;return (NOT);}
[oO][fF]							 {;;return (OF);}
[oO][rR]							 {;;no = OR;return (ADDOP);}
[pP][rR][oO][cC][eE][dD][uU][rR][eE] {;;return (PROCEDURE);}
[pP][rR][oO][gG][rR][aA][mM]		 {;;return (PROGRAM);}
[rR][eE][aA][dD]					 {;;return (READ);}
[rR][eE][aA][lL]					 {;;return (REAL);}
[rR][eE][cC][oO][rR][dD]			 {;;return (RECORD);}
[tT][hH][eE][nN]					 {;;return (THEN);}
[tT][rR][uU][eE]					 {;;return (TRUE);}
[vV][aA][rR]						 {;;return (VAR);}
[wW][hH][iI][lL][eE]				 {;;return (WHILE);}
[wW][rR][iI][tT][eE]				 {;;return (WRITE);}

//ר�÷���
"+"			 {;;no = ADD;return (ADDOP);}
"-"			 {;;no = SUB;return (ADDOP);}
"*"			 {;;no = MUL;return (MULOP);}
"/"			 {;;no = DVD;return (MULOP);}
"<"			 {;;no = LT; return (RELOP);}
"<="		 {;;no = LE; return (RELOP);}
"="			 {;;no = EQ; return (RELOP);}
">="		 {;;no = GE; return (RELOP);}
">"			 {;;no = GT; return (RELOP);}
"<>"		 {;;no = NE; return (RELOP);}
[dD][iI][vV] {;;no = DIV;return (MULOP);}
[mM][oO][dD] {;;no = MOD;return (MULOP);}
":="		 {;;return (ASSIGNOP);}
"\."		 {;;return ('.');}
"("			 {;;return ('(');}
")"			 {;;return (')');}
"["			 {;;return ('[');}
"]"			 {;;return (']');}
","          {;;return (',');}
";"          {;;return (';');}
":"          {;;return (':');}
//�հ�
{white}+		   {
					;;					
				   }

//��ʶ��
{id}			   {
                    ;;
                    strcpy(id_str,yytext);
                    return (ID);
                   }
			
//����
{num}			   {
					;;
					push(yytext);
					return (NUM);
				   }
%%
/////////////////////////////////////////////////////////////////////////////
/*��������*/
#ifndef yywrap
 int yywrap(){return 1;}
#endif