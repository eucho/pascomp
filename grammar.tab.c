
/* A Bison parser, made from grammar.y
   by GNU bison 1.29.  */

#define YYBISON 1  /* Identify Bison output.  */

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

#line 1 "grammar.y"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define YYSTYPE double /* double type for YACC stack */
#define Int 6
#define Real 1
#define Bool 2
#define Array 3
#define FuncPro 4
#define Record 5
#define STRING_MAX 255
#define ARRAY_MAX 1000
#define STACK_SIZE 1000
#define LIST_SIZE  10000

extern FILE* yyin;
extern char* yytext;
extern int yyleng;
extern int yylineno;
extern int no;


//变量部分===============================================================================================

FILE* fout;//输出文件指针
FILE* hout;//输出.h文件指针
FILE* tout;//输出符号表文件指针
int struct_no;//为record翻译为struct结构时使用的编号，每个struct有唯一的标识
int num1,num2;//用于"NUM .. NUM"的情况
int cur_fptr_position;//用于存放输出.h文件指针当前位置
int fptr_offset;//用于存放cur_fptr_position与输出.h文件指针当前位置的差值
int spc;//用于输出指定数量的空格时
int i;
int is_function;//状态变量，表明当前的variable是否是函数，若是函数，则为函数名赋值即为return
int is_logic;//状态变量，表明当前读到的表达式中的运算是否是逻辑运算，若是则为1，否则为0
int is_const;//状态变量，表明当前读到的factor是否为常数，若是则为1，否则为0
int write_in_c;//状态变量，表明当前应向.c文件输出还是向.h文件输出
int upper_level;//状态变量，表明当前的ID是属于上一级符号表的
int upper_level0;//同上
int para,para0;//状态变量，记录当前函数参数个数
typedef struct listsnode
{
    char name[STRING_MAX], type[STRING_MAX];
    int  define, appear[ARRAY_MAX],atop;
    struct list * list;
} ListNode;//符号表单元
typedef struct list
{
     int top;//符号表当前可用位
     ListNode table[LIST_SIZE];//符号表
} List; //符号表结构
    
List main_list;//主符号表

char stack[STACK_SIZE][STRING_MAX];//与分析栈平行的临时栈
int top;//临时栈栈顶位置
int cur_top;//存放调用fout_id_list函数前栈顶的位置
char top_str[STRING_MAX];//调用pop()函数时弹出的临时栈栈顶
char number[STRING_MAX];//用于数字转字符串时
char id_str[STRING_MAX];//用于将ID插入到符号表时

List* l_stack[STACK_SIZE];//符号表栈
List* l_top_str;//符号表栈顶元素
int l_top;//符号表栈栈顶位置

//类型表达式变量
char type_expression[STRING_MAX];//类型表达式字符串，用于定义标识符时为标识符属性type字符串赋值
char type_exp_fp[STRING_MAX];//函數及过程的类型表达式字符串
char type_exp_r[STRING_MAX];//记录结构的类型表达式字符串
char type_exp_a[STRING_MAX];//数组的类型字符串（取值为"int"或"float"）
char type_exp_v[STRING_MAX];//variable的类型字符串，用于赋值时的类型检查
//=======================================================================================================


//函数部分===============================================================================================

/*临时栈相关函数=================================*/
int push(char* temp);//入临时栈
int pop(void);//弹出临时栈栈顶
int isEmpty(void);//判断临时栈是否为空
int isReal(void);//判断临时栈顶的数是否为real型
int fout_id_list(int type,FILE* ofile);//输出ID列表
int test_out(int type);//测试用函数，根据type值输出各种信息
int isFloat(char* number);//判断一个数是否为浮点数

/*===============================================*/

/*符号表栈相关函数===============================*/
int l_push(List* list);//入符号表栈
int l_pop(void);//出符号表栈
/*===============================================*/

/*符号表相关函数=================================*/
int insert(char* id);//插入符号表操作。若该标识符已存在，则返回-1，提示存在重定义错误；否则将该标识符插入符号表
int search(char* id);//搜索符号表操作，若该标识符在当前符号表中存在，则返回其编号；若该标识符在当前符号表中不存在，则搜索其上级符号表，并返回其编号；若所有符号表中均不存在，则返回-1，并提示错误
/*===============================================*/

int initialList(List* list);//符号表初始化函数
int exp_check(char* des,char* src,int start,int end);//字符串比较函数，用于检查类型表达式
int para_number(char* type_expression);//统计函数及过程类型表达式中的参数个数
int get_range_number(char* type_expression);//获取数组类型表达式中的角标范围，分别保存在num1、num2中，同时将数组类型保存在type_exp_a中4

void yyerror(const char*);
//=======================================================================================================
#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>



#define	YYFINAL		174
#define	YYFLAG		-32768
#define	YYNTBASE	57

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 298 ? yytranslate[x] : 110)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      51,    52,    47,    45,    53,    46,    50,    48,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    54,    49,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    55,     2,    56,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG != 0
static const short yyprhs[] =
{
       0,     0,     5,     6,     7,    15,    19,    21,    25,    27,
      28,    33,    34,    40,    44,    46,    56,    57,    62,    64,
      66,    68,    73,    77,    78,    79,    87,    88,    94,    98,
      99,   105,   109,   110,   114,   116,   117,   121,   123,   124,
     129,   131,   133,   134,   135,   136,   137,   148,   149,   150,
     157,   162,   167,   169,   170,   176,   178,   179,   185,   186,
     191,   193,   194,   200,   203,   204,   205,   210,   212,   214,
     217,   218,   223,   224,   230,   231,   236,   238,   240,   241,
     247,   249,   250,   255,   256,   260,   262,   264,   266,   267,
     272
};
static const short yyrhs[] =
{
      58,    59,   107,    50,     0,     0,     0,    22,     3,    51,
      60,    61,    52,    49,     0,    61,    53,     3,     0,     3,
       0,    62,    53,     3,     0,     3,     0,     0,    28,    64,
      65,    49,     0,     0,    65,    49,    61,    54,    66,     0,
      61,    54,    66,     0,    68,     0,     9,    55,     4,    50,
      50,     4,    56,    20,    68,     0,     0,    25,    67,    65,
      14,     0,    18,     0,    24,     0,    11,     0,     4,    50,
      50,     4,     0,    69,   109,    49,     0,     0,     0,    16,
       3,    71,    73,    54,    68,    49,     0,     0,    21,     3,
      72,    73,    49,     0,    51,    74,    52,     0,     0,    74,
      49,    61,    54,    66,     0,    61,    54,    66,     0,     0,
      10,    76,    14,     0,    77,     0,     0,    77,    49,    78,
       0,    78,     0,     0,    86,     6,    79,    95,     0,    88,
       0,    75,     0,     0,     0,     0,     0,    17,    80,    95,
      81,    26,    78,    82,    13,    83,    78,     0,     0,     0,
      29,    84,    95,    85,    12,    78,     0,    23,    51,    62,
      52,     0,    30,    51,    92,    52,     0,     3,     0,     0,
       3,    87,    55,    95,    56,     0,     3,     0,     0,     3,
      89,    51,    90,    52,     0,     0,    90,    53,    91,    95,
       0,    95,     0,     0,    92,    53,    93,    94,    95,     0,
      94,    95,     0,     0,     0,    97,     8,    96,    97,     0,
      97,     0,   100,     0,   106,   100,     0,     0,    97,     5,
      98,   100,     0,     0,     3,    99,    55,    95,    56,     0,
       0,   100,     7,   101,   102,     0,   102,     0,     3,     0,
       0,     3,   103,    51,    90,    52,     0,     4,     0,     0,
      51,   104,    95,    52,     0,     0,    19,   105,   102,     0,
      27,     0,    15,     0,     5,     0,     0,    63,    69,   108,
      75,     0,    70,    63,    75,     0
};

#endif

#if YYDEBUG != 0
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   118,   120,   126,   130,   141,   146,   152,   197,   243,
     247,   251,   253,   266,   281,   294,   310,   327,   339,   344,
     349,   354,   378,   379,   381,   398,   416,   432,   450,   451,
     453,   459,   464,   470,   474,   475,   477,   479,   481,   508,
     518,   525,   527,   535,   541,   549,   556,   562,   570,   576,
     583,   585,   587,   657,   703,   727,   779,   840,   866,   874,
     878,   884,   891,   918,   946,   964,  1007,  1011,  1016,  1020,
    1034,  1063,  1076,  1140,  1163,  1199,  1222,  1227,  1295,  1357,
    1407,  1424,  1435,  1447,  1461,  1473,  1482,  1492,  1512,  1519,
    1523
};
#endif


#if YYDEBUG != 0 || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "ID", "NUM", "ADDOP", "ASSIGNOP", "MULOP", 
  "RELOP", "ARRAY", "PBEGIN", "BOOLEAN", "DO", "ELSE", "END", "FALSE", 
  "FUNCTION", "IF", "INTEGER", "NOT", "OF", "PROCEDURE", "PROGRAM", 
  "READ", "REAL", "RECORD", "THEN", "TRUE", "VAR", "WHILE", "WRITE", 
  "AND", "OR", "ADD", "SUB", "MUL", "DVD", "LT", "LE", "EQ", "GE", "GT", 
  "NE", "DIV", "MOD", "'+'", "'-'", "'*'", "'/'", "';'", "'.'", "'('", 
  "')'", "','", "':'", "'['", "']'", "program", "M", "program_head", "@1", 
  "identifier_list", "identifier_list2", "declarations", "@2", 
  "declaration", "type", "@3", "standard_type", "subproc_declarations", 
  "subproc_head", "@4", "@5", "arguments", "parameter_list", 
  "compound_statement", "optional_statements", "statement_list", 
  "statement", "@6", "@7", "@8", "@9", "@10", "@11", "@12", "variable", 
  "@13", "procedure_statement", "@14", "expression_list", "@15", 
  "expression_list2", "@16", "K", "expression", "@17", 
  "simple_expression", "@18", "@19", "term", "@20", "factor", "@21", 
  "@22", "@23", "sign", "program_body", "@24", "subproc_declaration", NULL
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,    57,    58,    60,    59,    61,    61,    62,    62,    64,
      63,    63,    65,    65,    66,    66,    67,    66,    68,    68,
      68,    68,    69,    69,    71,    70,    72,    70,    73,    73,
      74,    74,    74,    75,    76,    76,    77,    77,    79,    78,
      78,    78,    80,    81,    82,    83,    78,    84,    85,    78,
      78,    78,    86,    87,    86,    88,    89,    88,    91,    90,
      90,    93,    92,    92,    94,    96,    95,    95,    97,    97,
      98,    97,    99,    97,   101,   100,   100,   102,   103,   102,
     102,   104,   102,   105,   102,   102,   102,   106,   108,   107,
     109
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     4,     0,     0,     7,     3,     1,     3,     1,     0,
       4,     0,     5,     3,     1,     9,     0,     4,     1,     1,
       1,     4,     3,     0,     0,     7,     0,     5,     3,     0,
       5,     3,     0,     3,     1,     0,     3,     1,     0,     4,
       1,     1,     0,     0,     0,     0,    10,     0,     0,     6,
       4,     4,     1,     0,     5,     1,     0,     5,     0,     4,
       1,     0,     5,     2,     0,     0,     4,     1,     1,     2,
       0,     4,     0,     5,     0,     4,     1,     1,     0,     5,
       1,     0,     4,     0,     3,     1,     1,     1,     0,     4,
       3
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       2,     0,     0,    11,     0,     9,    23,     0,     3,     0,
      88,     1,     0,     6,     0,     0,     0,     0,    11,     0,
       0,     0,     0,     0,    10,    24,    26,     0,    35,    89,
      22,     0,     5,     0,     0,    20,    18,    19,    16,    13,
      14,     0,    29,    29,    90,    55,    42,     0,    47,     0,
      41,     0,    34,    37,     0,    40,     4,     0,     0,     0,
       0,    32,     0,     0,     0,     0,     0,     0,     0,    64,
      33,     0,    38,     0,     0,     0,    12,     0,     0,     0,
      27,     0,     0,    77,    80,    87,    86,    83,    85,    81,
      43,    67,    68,    76,     0,     8,     0,    48,     0,     0,
      36,     0,    21,     0,    17,     0,     0,     0,    28,     0,
       0,     0,    60,     0,     0,     0,     0,     0,    70,    65,
      74,    77,    69,    50,     0,     0,    51,    61,    63,    39,
       0,    31,     0,    25,    54,    57,    58,     0,     0,    84,
       0,     0,     0,     0,     0,     7,     0,    64,     0,     0,
       0,     0,     0,    82,    44,    71,    66,    75,    49,     0,
       0,    30,    59,    73,    79,     0,    62,     0,    45,    15,
       0,    46,     0,     0,     0
};

static const short yydefgoto[] =
{
     172,     1,     3,    12,    14,    96,     6,     9,    15,    39,
      59,    40,    10,    18,    42,    43,    62,    78,    50,    51,
      52,    53,   101,    66,   117,   165,   170,    68,   125,    54,
      64,    55,    65,   111,   150,    98,   147,    99,   112,   143,
      91,   142,   113,    92,   144,    93,   114,   116,   115,    94,
       7,    19,    20
};

static const short yypact[] =
{
  -32768,    -6,    15,     8,   -18,-32768,-32768,    -2,-32768,    50,
       6,-32768,    50,-32768,   -10,    12,    71,    73,     8,    29,
      42,    16,    97,    54,    50,-32768,-32768,    29,    37,-32768,
  -32768,    52,-32768,    55,    47,-32768,-32768,-32768,-32768,-32768,
  -32768,    -8,    56,    56,-32768,     0,-32768,    57,-32768,    58,
  -32768,    89,    61,-32768,    98,-32768,-32768,    62,   102,    50,
      54,    50,    59,    65,    60,    66,     5,   108,     5,-32768,
  -32768,    37,-32768,   112,    68,    -7,-32768,    27,   -24,    53,
  -32768,     5,     5,   -32,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,    26,   113,-32768,    11,-32768,    30,-32768,    32,     5,
  -32768,     5,-32768,    69,-32768,    50,    54,    50,-32768,    72,
      67,    35,-32768,    70,    75,    11,     5,    96,-32768,-32768,
  -32768,    76,   113,-32768,   121,   116,-32768,-32768,-32768,-32768,
     125,-32768,    36,-32768,-32768,-32768,-32768,     5,     5,-32768,
      78,    37,    11,     5,    11,-32768,    37,-32768,    77,    54,
       5,    79,    45,-32768,-32768,   113,   126,-32768,-32768,     5,
     114,-32768,-32768,-32768,-32768,   119,-32768,    53,-32768,-32768,
      37,-32768,   136,   137,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,-32768,-32768,   -11,-32768,   120,-32768,    80,   -57,
  -32768,   -74,-32768,-32768,-32768,-32768,    99,-32768,     2,-32768,
  -32768,   -71,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,     3,-32768,-32768,-32768,    -4,   -64,-32768,
      -3,-32768,-32768,   -83,-32768,  -103,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768
};


#define	YYLAST		143


static const short yytable[] =
{
     100,    21,    90,    76,    97,   109,   -52,   104,    83,    84,
      85,   122,   139,    41,   121,    84,     2,   110,     4,   -78,
      86,    29,    16,   -72,    87,   107,    86,    17,   108,    44,
      87,   118,    88,     8,   119,   128,     5,   129,    88,    28,
      45,   157,   105,    22,    23,    22,    60,    28,    11,   131,
      77,   -56,   140,    13,    46,   -53,    89,    33,    33,   155,
      47,    24,    89,    34,    35,    35,    48,    49,    31,    22,
     154,    36,    36,   151,    25,   158,    26,    37,    37,    38,
      22,   106,   123,   124,   126,   127,   162,   135,   136,    22,
     149,    30,   161,   169,    41,   166,   132,   164,   136,   171,
      32,    56,    58,    70,    72,    57,    74,    61,    67,    69,
      71,    95,    73,    79,    80,    81,   102,    82,   103,   130,
     120,   133,   141,   134,   145,   137,   138,   -78,   146,   148,
     153,   118,   168,   160,   167,   163,   173,   174,    27,    75,
     156,   152,    63,   159
};

static const short yycheck[] =
{
      71,    12,    66,    60,    68,    79,     6,    14,     3,     4,
       5,    94,   115,    24,     3,     4,    22,    81,     3,    51,
      15,    19,    16,    55,    19,    49,    15,    21,    52,    27,
      19,     5,    27,    51,     8,    99,    28,   101,    27,    10,
       3,   144,    49,    53,    54,    53,    54,    10,    50,   106,
      61,    51,   116,     3,    17,    55,    51,     4,     4,   142,
      23,    49,    51,     9,    11,    11,    29,    30,    52,    53,
     141,    18,    18,   137,     3,   146,     3,    24,    24,    25,
      53,    54,    52,    53,    52,    53,   150,    52,    53,    53,
      54,    49,   149,   167,   105,   159,   107,    52,    53,   170,
       3,    49,    55,    14,     6,    50,     4,    51,    51,    51,
      49,     3,    50,    54,    49,    55,     4,    51,    50,    50,
       7,    49,    26,    56,     3,    55,    51,    51,    12,     4,
      52,     5,    13,    56,    20,    56,     0,     0,    18,    59,
     143,   138,    43,   147
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/local/share/bison/bison.simple"

/* Skeleton output parser for bison,
   Copyright 1984, 1989, 1990, 2000, 2001 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
# ifdef alloca
#  define YYSTACK_USE_ALLOCA 1
# else /* alloca not defined */
#  ifdef __GNUC__
#   define YYSTACK_USE_ALLOCA 1
#   define alloca __builtin_alloca
#  else /* not GNU C.  */
#   if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#    define YYSTACK_USE_ALLOCA 1
#    include <alloca.h>
#   else /* not sparc */
     /* We think this test detects Watcom and Microsoft C.  */
     /* This used to test MSDOS, but that is a bad idea since that
	symbol is in the user namespace.  */
#    if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#     if 0
       /* No need for malloc.h, which pollutes the namespace; instead,
	  just don't use alloca.  */
#      include <malloc.h>
#     endif
#    else /* not MSDOS, or __TURBOC__ */
#     if defined(_AIX)
       /* I don't know what this was needed for, but it pollutes the
	  namespace.  So I turned it off.  rms, 2 May 1997.  */
       /* #include <malloc.h>  */
 #pragma alloca
#      define YYSTACK_USE_ALLOCA 1
#     else /* not MSDOS, or __TURBOC__, or _AIX */
#      if 0
	/* haible@ilog.fr says this works for HPUX 9.05 and up, and on
	   HPUX 10.  Eventually we can turn this on.  */
#       ifdef __hpux
#        define YYSTACK_USE_ALLOCA 1
#        define alloca __builtin_alloca
#  	endif /* __hpux */
#      endif
#     endif /* not _AIX */
#    endif /* not MSDOS, or __TURBOC__ */
#   endif /* not sparc */
#  endif /* not GNU C */
# endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#if YYSTACK_USE_ALLOCA
# define YYSTACK_ALLOC alloca
#else
# define YYSTACK_ALLOC malloc
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG
# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    fprintf Args;				\
} while (0)
/* Nonzero means print parse trace. [The following comment makes no
   sense to me.  Could someone clarify it?  --akim] Since this is
   uninitialized, it does not stop multiple parsers from coexisting.
   */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).  */
#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
# define __yy_memcpy(To, From, Count)	__builtin_memcpy (To, From, Count)
#else				/* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
# ifndef __cplusplus
__yy_memcpy (to, from, count)
     char *to;
     const char *from;
     unsigned int count;
# else /* __cplusplus */
__yy_memcpy (char *to, const char *from, unsigned int count)
# endif
{
  register const char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif

#line 212 "/usr/local/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# ifdef __cplusplus
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else /* !__cplusplus */
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif /* !__cplusplus */
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define _YY_DECL_VARIABLES				\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
_YY_DECL_VARIABLES				\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
_YY_DECL_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yysv': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
# if YYLSP_NEEDED
  YYLTYPE yyloc;
# endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;
  
 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack. Use copies of
	 these so that the &'s don't force the real ones into memory.
	 */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#if YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of the
	 data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow ("parser stack overflow",
		  &yyss1, size * sizeof (*yyssp),
		  &yyvs1, size * sizeof (*yyvsp),
		  &yyls1, size * sizeof (*yylsp),
		  &yystacksize);
# else
      yyoverflow ("parser stack overflow",
		  &yyss1, size * sizeof (*yyssp),
		  &yyvs1, size * sizeof (*yyvsp),
		  &yystacksize);
# endif

      yyss = yyss1; yyvs = yyvs1;
# if YYLSP_NEEDED
      yyls = yyls1;
# endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror ("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
# if YYLSP_NEEDED
	      free (yyls);
# endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
# ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
# endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
# if YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
# endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#if YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %d\n", yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 2:
#line 121 "grammar.y"
{
       initialList(&main_list);
       l_push(&main_list);
  ;
    break;}
case 3:
#line 127 "grammar.y"
{
                   cur_top = top;
               ;
    break;}
case 4:
#line 131 "grammar.y"
{
                   fprintf(fout,"int main(");
                   
                   push("int");
                   push("standard");
                   strcpy(type_expression,"int");
				   fout_id_list(1,fout);
                   fprintf(fout,"){\n");    
               ;
    break;}
case 5:
#line 142 "grammar.y"
{
                    sprintf(number, "%d", insert(id_str));
                    push(number);
                ;
    break;}
case 6:
#line 147 "grammar.y"
{
                    sprintf(number, "%d", insert(id_str));
                    push(number);
                ;
    break;}
case 7:
#line 153 "grammar.y"
{
                    if (write_in_c == 0)
                    {
                        fprintf(hout,"scanf(\"");
                        if(strcmp(l_stack[l_top-1]->table[search(id_str)].type,"int")==0)
                        {
                            fprintf(hout,"%%");
                            fprintf(hout,"d\", &");
                        }
                        else if (strcmp(l_stack[l_top-1]->table[search(id_str)].type,"float")==0)
                        {
                            fprintf(hout,"%%");
                            fprintf(hout,"f\", &");
                        }
                        else
                        {
                            fprintf(hout,"%%");
                            fprintf(hout,"d\", &");
                        }
                        fprintf(hout,id_str);
                        fprintf(hout,");\n");
                    }
                    else
                    {
                        fprintf(fout,"scanf(\"");
                        if(strcmp(l_stack[l_top-1]->table[search(id_str)].type,"int")==0)
                        {
                            fprintf(fout,"%%");
                            fprintf(fout,"d\", &");
                        }
                        else if (strcmp(l_stack[l_top-1]->table[search(id_str)].type,"float")==0)
                        {
                            fprintf(fout,"%%");
                            fprintf(fout,"f\", &");
                        }
                        else
                        {
                            fprintf(fout,"%%");
                            fprintf(fout,"d\", &");
                        }
                        fprintf(fout,id_str);
                        fprintf(fout,");\n");
                    }
                ;
    break;}
case 8:
#line 198 "grammar.y"
{                   
                    if (write_in_c == 0)
                    {
                        fprintf(hout,"scanf(\"");
                        if(strcmp(l_stack[l_top-1]->table[search(id_str)].type,"int")==0)
                        {
                            fprintf(hout,"%%");
                            fprintf(hout,"d\", &");
                        }
                        else if (strcmp(l_stack[l_top-1]->table[search(id_str)].type,"float")==0)
                        {
                            fprintf(hout,"%%");
                            fprintf(hout,"f\", &");
                        }
                        else
                        {
                            fprintf(hout,"%%");
                            fprintf(hout,"d\", &");
                        }
                        fprintf(hout,id_str);
                        fprintf(hout,");\n");
                    }
                    else
                    {
                        fprintf(fout,"scanf(\"");
                        if(strcmp(l_stack[l_top-1]->table[search(id_str)].type,"int")==0)
                        {
                            fprintf(fout,"%%");
                            fprintf(fout,"d\", &");
                        }
                        else if (strcmp(l_stack[l_top-1]->table[search(id_str)].type,"float")==0)
                        {
                            fprintf(fout,"%%");
                            fprintf(fout,"f\", &");
                        }
                        else
                        {
                            fprintf(fout,"%%");
                            fprintf(fout,"d\", &");
                        }
                        fprintf(fout,id_str);
                        fprintf(fout,");\n");
                    }
                ;
    break;}
case 9:
#line 244 "grammar.y"
{
                   write_in_c = 0;
               ;
    break;}
case 10:
#line 248 "grammar.y"
{
                  
               ;
    break;}
case 12:
#line 254 "grammar.y"
{
                  if (write_in_c == 0)  
                  {         
				      fout_id_list(0,hout);
				      fprintf(hout,";\n");    
				  } 
				  else
				  {
				      fout_id_list(0,fout);
				      fprintf(fout,";\n"); 				  				  
				  }            
              ;
    break;}
case 13:
#line 267 "grammar.y"
{
                  if (write_in_c == 0)
                  {                               
				      fout_id_list(0,hout);
				      fprintf(hout,";\n");
				  }
				  else
				  {
				      fout_id_list(0,fout);
				      fprintf(fout,";\n"); 				  				  
				  }			  
			  ;
    break;}
case 14:
#line 282 "grammar.y"
{
           switch((int)yyvsp[0])
           {
               case Int:case Bool:
                   strcpy(type_expression,"int");
                   break;
               case Real:
                   strcpy(type_expression,"float");
                   break;          
           }
           push("standard");
       ;
    break;}
case 15:
#line 295 "grammar.y"
{
           strcpy(type_expression,"array(");
           strcat(type_expression,stack[top-3]);
           strcat(type_expression,"..");
           strcat(type_expression,stack[top-2]);
           strcat(type_expression,",");
           strcat(type_expression,stack[top-1]);
           strcat(type_expression,")");
           push("array");
           if (isFloat(stack[top-3]) || isFloat(stack[top-2]))
           {
               printf("\nLine %d: Warning: The range of array must be integer.\n",yylineno);
               return 0;
           }
       ;
    break;}
case 16:
#line 311 "grammar.y"
{           
           memset(type_exp_r,'\0',sizeof(type_exp_r));
           
           fprintf(hout,"struct s_");
           sprintf(number, "%d", struct_no++);
           fprintf(hout,number);
           fprintf(hout,"{\n");
           cur_top = top;
           
           //为记录创建符号表
           
           l_stack[l_top - 1]->table[atoi(stack[top-1])].list = malloc (sizeof(List));
           initialList(l_stack[l_top - 1]->table[atoi(stack[top-1])].list);
           l_push(l_stack[l_top - 1]->table[atoi(stack[top-1])].list);
           
       ;
    break;}
case 17:
#line 328 "grammar.y"
{
           strcpy(type_expression,"record(");
           strcat(type_expression,type_exp_r);
           strcat(type_expression,")");           
           fprintf(hout,"}");
           cur_top = 0;
           push("record");
           l_pop();     
       ;
    break;}
case 18:
#line 340 "grammar.y"
{
				    push("int");
				    yyval = Int;
				;
    break;}
case 19:
#line 345 "grammar.y"
{
                    push("float");
                    yyval = Real;
                ;
    break;}
case 20:
#line 350 "grammar.y"
{
                    push("int");
                    yyval = Bool;
                ;
    break;}
case 21:
#line 355 "grammar.y"
{
                    pop();
                    if (isReal())
                    {
                        push("float");
                        yyval = Real;                        
                    }  
                    else
                    {
                        pop();
                        if (isReal())
                        {
                            push("float");
                            yyval = Real;
                        }
                        else
                        {
                            push("int");
                            yyval = Int;
                        }
                    }    
                ;
    break;}
case 24:
#line 382 "grammar.y"
{
                   yyvsp[0] = insert(id_str);                                                         
                   cur_fptr_position = ftell(hout); 
                   for (spc = 0; spc < 7 + yyleng; spc ++)
                       fprintf(hout," ");
                   fprintf(hout,"(");                                     
                   fflush(hout);
                   
                   //为函数创建符号表
                   l_stack[l_top - 1]->table[(int)yyvsp[0]].list = malloc (sizeof(List));
                   initialList(l_stack[l_top - 1]->table[(int)yyvsp[0]].list);
                   l_push(l_stack[l_top - 1]->table[(int)yyvsp[0]].list);
                   
                   memset(type_exp_fp,'\0',sizeof(type_exp_fp));
                   
               ;
    break;}
case 25:
#line 399 "grammar.y"
{
                   fprintf(hout,"){\n");
                   pop();
                   fptr_offset = cur_fptr_position - ftell(hout);
                   fseek(hout,fptr_offset,1);
                   fprintf(hout,top_str);
                   fseek(hout,1,1);  
                   fprintf(hout,l_stack[l_top - 2]->table[(int)yyvsp[-5]].name);                       
                   fflush(hout);                                 
                   fseek(hout,- fptr_offset - (ftell(hout) - cur_fptr_position),1);
                   
                   //为函数类型表达式赋值
                   strcat(type_exp_fp,"-");
                   strcat(type_exp_fp,top_str);
                   strcpy(l_stack[l_top - 2]->table[(int)yyvsp[-5]].type,type_exp_fp);
                   
               ;
    break;}
case 26:
#line 417 "grammar.y"
{
                   yyvsp[0] = insert(id_str);
                   cur_fptr_position = ftell(hout); 
                   for (spc = 0; spc < 6 + yyleng; spc ++)
                       fprintf(hout," ");
                   fprintf(hout,"(");                                     
                   fflush(hout);
                   
                   //为过程创建符号表
                   l_stack[l_top - 1]->table[(int)yyvsp[0]].list = malloc (sizeof(List));
                   initialList(l_stack[l_top - 1]->table[(int)yyvsp[0]].list);
                   l_push(l_stack[l_top - 1]->table[(int)yyvsp[0]].list);
                   
                   memset(type_exp_fp,'\0',sizeof(type_exp_fp));
               ;
    break;}
case 27:
#line 433 "grammar.y"
{
                   fprintf(hout,"){");
                   fptr_offset = cur_fptr_position - ftell(hout);
                   fseek(hout,fptr_offset,1);
                   fprintf(hout,"void");
                   fseek(hout,1,1);  
                   fprintf(hout,l_stack[l_top - 2]->table[(int)yyvsp[-3]].name);                    
                   fflush(hout);                                 
                   fseek(hout,- fptr_offset - (ftell(hout) - cur_fptr_position),1);
                   
                   //为过程类型表达式赋值
                   strcat(type_exp_fp,"-");
                   strcat(type_exp_fp,"void");
                   strcpy(l_stack[l_top - 2]->table[(int)yyvsp[-3]].type,type_exp_fp);                  

               ;
    break;}
case 30:
#line 454 "grammar.y"
{     
                      fprintf(hout,", ");             
                      fout_id_list(1,hout);
                      fflush(hout);
                 ;
    break;}
case 31:
#line 460 "grammar.y"
{
                      fout_id_list(1,hout);
				      fflush(hout);
                 ;
    break;}
case 32:
#line 465 "grammar.y"
{                 
                      fprintf(hout,"void");                      
                      fflush(hout);
                 ;
    break;}
case 38:
#line 482 "grammar.y"
{
                pop();
                strcpy(type_exp_v,top_str);
                if (is_function == 1)
                {
                    if (write_in_c == 0)
						fprintf(hout,"return ");
					else
					{
					    printf("\nLine %d: Warning: %s is not a variable.\n",yylineno,id_str);
						fprintf(fout,id_str);
						fprintf(fout," = ");
				    }
                }
                else
                {
					if (write_in_c == 0)
						fprintf(hout," = ");
					else
						fprintf(fout," = ");   				                
			    }
			    fflush(hout);
				fflush(fout);
                
            ;
    break;}
case 39:
#line 508 "grammar.y"
{
                if (strcmp(type_expression,"int") != strcmp(type_exp_v,"int"))
                    printf("\nLine %d: Warning: Conversion from integer to float.\n",yylineno);
                if (write_in_c == 0)
                    fprintf(hout,";\n");
                else
                    fprintf(fout,";\n");
                fflush(hout);
				fflush(fout);
            ;
    break;}
case 40:
#line 519 "grammar.y"
{
                if (write_in_c == 0)
                    fprintf(hout,";\n");
                else
                    fprintf(fout,";\n");
            ;
    break;}
case 42:
#line 528 "grammar.y"
{
                if (write_in_c == 0)
                    fprintf(hout,"if (");
                else
                    fprintf(fout,"if (");
            ;
    break;}
case 43:
#line 535 "grammar.y"
{
                if (write_in_c == 0)
                    fprintf(hout,"){\n");
                else
                    fprintf(fout,"){\n");
            ;
    break;}
case 44:
#line 542 "grammar.y"
{
                if (write_in_c == 0)
                    fprintf(hout,"}\n");
                else
                    fprintf(fout,"}\n");
            ;
    break;}
case 45:
#line 549 "grammar.y"
{
                if (write_in_c == 0)
                    fprintf(hout,"else {\n");
                else
                    fprintf(fout,"else {\n");
            ;
    break;}
case 46:
#line 556 "grammar.y"
{
                if (write_in_c == 0)
                    fprintf(hout,"} ");
                else
                    fprintf(fout,"} ");
            ;
    break;}
case 47:
#line 563 "grammar.y"
{
                if (write_in_c == 0)
                    fprintf(hout,"while (");
                else
                    fprintf(fout,"while (");
            ;
    break;}
case 48:
#line 570 "grammar.y"
{
                if (write_in_c == 0)
                    fprintf(hout,"){\n");
                else
                    fprintf(fout,"){\n");
            ;
    break;}
case 49:
#line 577 "grammar.y"
{
                if (write_in_c == 0)
                    fprintf(hout,"}\n");
                else
                    fprintf(fout,"}\n");
            ;
    break;}
case 52:
#line 588 "grammar.y"
{
               yyvsp[0] = search(id_str);
               if (yyvsp[0] == -1)
               {
                  if (l_top != 1)
                  {
                     l_pop();
                     yyvsp[0] = search(id_str);
                     if (yyvsp[0] == -1)
                     {
                         printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                         yyvsp[0] = insert(id_str);
                         strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                     }
                     upper_level = 1;
                     l_push(l_top_str);
                  }
                  else                             
                  {
                     printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                     yyvsp[0] = insert(id_str);
                     strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                     upper_level = 0;
                  }
               }
               else
				   upper_level = 0;                                               
               for (i = 0;i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type) && l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type[i] != '-'; i++);
                
               
               if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "int",i+1,i+3) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))                 
               {
                   is_function = 1;
                   push("int");
               }
               else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
               {
                   is_function = 1;
                   push("float");
               }    
               else if (upper_level == 0 && exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
               {
                   printf("\nLine %d: Warning: Procedure has no return value.\n",yylineno);
                   is_function = 1;
                   push("int");//将错误调用的过程处理为int类型的变量，以继续分析
               }
               else
               {
                   is_function = 0;
                   if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "array",0,4))
                   {
                       printf("\nLine %d: Warning: Incorrect reference of the array:lack of corner mark.\n",yylineno);
                       push("int");//将错误引用的数组名处理为int类型的变量，以继续分析
                   }
                   else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "record",0,5))
                   {
                       printf("\nLine %d: Warning: Incorrect reference of the record.\n",yylineno);
                       push("int");//将错误引用的记录名处理为int类型的变量，以继续分析
                   }
                   else
                   {
                       push(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type);
                   }
                   if (write_in_c == 0) 
                       fprintf(hout,id_str);
				   else
                       fprintf(fout,id_str);
               }                   
           ;
    break;}
case 53:
#line 658 "grammar.y"
{
               yyvsp[0] = search(id_str);
               if (yyvsp[0] == -1)
               {
                  if (l_top != 1)
                  {
                     l_pop();
                     yyvsp[0] = search(id_str);
                     if (yyvsp[0] == -1)
                     {
                         printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                         yyvsp[0] = insert(id_str);
                         strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                     }
                     upper_level = 1;
                     l_push(l_top_str);
                  }
                  else                             
                  {
                     printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                     yyvsp[0] = insert(id_str);
                     strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                     upper_level = 0;
                  }
               }
               else
				  upper_level = 0;   
					    
			    if (! exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "array",0,4)) 
					 printf("\nLine %d: Warning: %s is not an array.\n",id_str);
               is_function = 0;
               if (write_in_c == 0)
               {
                   fprintf(hout,id_str);
                   fprintf(hout,"[");
               }
               else
               {
                   fprintf(fout,id_str);
                   fprintf(fout,"[");
               }
               sprintf(number, "%d", upper_level);
               push(number);
                   
           ;
    break;}
case 54:
#line 704 "grammar.y"
{
               pop();
               upper_level = atoi(top_str);
               if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[-4]].type, "array",0,4) && is_const == 1)
               {
                   get_range_number(l_stack[l_top-1-upper_level]->table[(int)yyvsp[-4]].type);
                   if (atoi(number) < num1 || atoi(number) > num2)
                   {
                        printf("\nLine %d: Warning: Incorrect reference of the array:%s is out of range.\n",yylineno,number);
                   }
               }
               if (write_in_c == 0)
               {
                   fprintf(hout,"]");
               }
               else
               {
                   fprintf(fout,"]");
               }
               get_range_number(l_stack[l_top-1-upper_level]->table[(int)yyvsp[-4]].type);
               push(type_exp_a);
           ;
    break;}
case 55:
#line 728 "grammar.y"
{
                        yyvsp[0] = search(id_str);
                        if (yyvsp[0] == -1)
                        {
                             if (l_top != 1)
                             {
                                 l_pop();
                                 yyvsp[0] = search(id_str);
                                 if (yyvsp[0] == -1)
                                 {
                                     printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                                     yyvsp[0] = insert(id_str);
                                     strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                                 }
                                 upper_level = 1;
                                 l_push(l_top_str);
                             }
                             else                             
                             {
                                 printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                                 yyvsp[0] = insert(id_str);
                                 strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                                 upper_level = 0;
                             }
                        }
                        else
							 upper_level = 0; 
                 
						  for (i = 0;i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type) && l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type[i] != '-'; i++);
						  if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "int",i+1,i+3) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))                 
                          {

                          }
                          else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
                          {

                          }    
                          else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
                          {
                              
                          }
                          else
						  {
							  printf("\nLine %d: Warning: %s is not a function or a procedure.\n",yylineno,id_str);                   
						  }        
                 
                          if (write_in_c == 0)
                              fprintf(hout,id_str);
                          else
                              fprintf(fout,id_str);
                      ;
    break;}
case 56:
#line 780 "grammar.y"
{
                          yyvsp[0] = search(id_str);
                          if (yyvsp[0] == -1)
                          {
                             if (l_top != 1)
                             {
                                 l_pop();
                                 yyvsp[0] = search(id_str);
                                 if (yyvsp[0] == -1)
                                 {
                                     printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                                     yyvsp[0] = insert(id_str);
                                     strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                                 }
                                 upper_level = 1;
                                 l_push(l_top_str);
                             }
                             else                             
                             {
                                 printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                                 yyvsp[0] = insert(id_str);
                                 strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                                 upper_level = 0;
                             }
                          }
                          else
							 upper_level = 0; 
                          
                          
						  for (i = 0;i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type) && l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type[i] != '-'; i++);
						  if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "int",i+1,i+3) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))                 
                          {

                          }
                          else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
                          {
                          
                          }    
                          else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
                          {
                             
                          }
                          else
						  {
							  printf("\nLine %d: Warning: %s is not a function or a procedure.\n",yylineno,id_str);                   
						  }
						  
						  para0 = para_number(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type);
						  
                          if (write_in_c == 0)
                          {
                              fprintf(hout,id_str);
                              fprintf(hout,"(");
                          }
                          else
                          {
                              fprintf(fout,id_str);
                              fprintf(fout,"(");
                          }
                      ;
    break;}
case 57:
#line 841 "grammar.y"
{
                          if (para != para0)
                              printf("\nLine %d: Warning: The number of parameters is incorrect.\n",yylineno);
                          else
                          {
                              for (i = para0; i >= 1; i --)
                              {
                                  pop();
                                  if (strcmp(l_stack[0]->table[(int)yyvsp[-4]].list->table[i-1].type, top_str) != 0)
                                  {
                                      printf("\nLine %d: Warning: Parameter %d is of incorrect type.\n",yylineno,i);
                                  }
                              }
                          }
                          if (write_in_c == 0)
                          {
                              fprintf(hout,")");
                          }
                          else
                          {
                              fprintf(fout,")");
                          }
                          
                      ;
    break;}
case 58:
#line 867 "grammar.y"
{
                      if (write_in_c == 0)
                          fprintf(hout,", ");
                      else
                          fprintf(fout,", ");
                  ;
    break;}
case 59:
#line 874 "grammar.y"
{
                      para ++;
                      push(type_expression);
                  ;
    break;}
case 60:
#line 879 "grammar.y"
{
                      para = 1;
                      push(type_expression);
                  ;
    break;}
case 61:
#line 885 "grammar.y"
{
                      if (write_in_c == 0)
                          fprintf(hout,", ");
                      else
                          fprintf(fout,", ");
                  ;
    break;}
case 62:
#line 892 "grammar.y"
{
                      if (write_in_c == 0)
                      {
                          if (strcmp(type_expression,"int")==0)
                          {                              
                              fptr_offset = cur_fptr_position - ftell(hout);
                              fseek(hout,fptr_offset,1);
                              fprintf(hout,"d");                
                              fflush(hout);                                 
                              fseek(hout,- fptr_offset - (ftell(hout) - cur_fptr_position),1);
                          }
                          fprintf(hout,");\n");
                      }
                      else
                      {
                          if (strcmp(type_expression,"int")==0)
                          {                              
                              fptr_offset = cur_fptr_position - ftell(hout);
                              fseek(fout,fptr_offset,1);
                              fprintf(fout,"d");                
                              fflush(fout);                                 
                              fseek(fout,- fptr_offset - (ftell(hout) - cur_fptr_position),1);
                          }
                          fprintf(fout,");\n");
                      }
                  ;
    break;}
case 63:
#line 919 "grammar.y"
{
                      if (write_in_c == 0)
                      {
                          if (strcmp(type_expression,"int")==0)
                          {                              
                              fptr_offset = cur_fptr_position - ftell(hout);
                              fseek(hout,fptr_offset,1);
                              fprintf(hout,"d");                
                              fflush(hout);                                 
                              fseek(hout,- fptr_offset - (ftell(hout) - cur_fptr_position),1);
                          }
                          fprintf(hout,");\n");
                      }
                      else
                      {
                          if (strcmp(type_expression,"int")==0)
                          {                              
                              fptr_offset = cur_fptr_position - ftell(fout);
                              fseek(fout,fptr_offset,1);
                              fprintf(fout,"d");                
                              fflush(fout);                                 
                              fseek(fout,- fptr_offset - (ftell(fout) - cur_fptr_position),1);
                          }
                          fprintf(fout,");\n");
                      }
                  ;
    break;}
case 64:
#line 947 "grammar.y"
{
     if (write_in_c == 0)
     {
         fprintf(hout,"printf(\"");
         fprintf(hout,"%%");
         cur_fptr_position = ftell(hout); 
         fprintf(hout,"f \",");
     }
     else
     {
         fprintf(fout,"printf(\"");
         fprintf(fout,"%%");
         cur_fptr_position = ftell(fout); 
         fprintf(fout,"f \",");
     }
 ;
    break;}
case 65:
#line 965 "grammar.y"
{                               
                 switch(no)
                 {
                     case LT:
                         if (write_in_c == 0)
                             fprintf(hout," < ");
                         else
                             fprintf(fout," < ");
                         break;    
                     case LE:
                         if (write_in_c == 0)
                             fprintf(hout," <= ");
                         else
                             fprintf(fout," <= ");
                         break;
                     case EQ:
                         if (write_in_c == 0)
                             fprintf(hout," == ");
                         else
                             fprintf(fout," == ");
                         break; 
                     case GE:
                         if (write_in_c == 0)
                             fprintf(hout," >= ");
                         else
                             fprintf(fout," >= ");
                         break; 
                     case GT:
                         if (write_in_c == 0)
                             fprintf(hout," > ");
                         else
                             fprintf(fout," > ");
                         break;  
                     case NE:
                         if (write_in_c == 0)
                             fprintf(hout," != ");
                         else
                             fprintf(fout," != ");
                         break;  
                 }                 
             ;
    break;}
case 66:
#line 1007 "grammar.y"
{
                 strcpy(type_expression,"int");
                 is_const = 0;
             ;
    break;}
case 67:
#line 1012 "grammar.y"
{
                is_const = yyvsp[0];
             ;
    break;}
case 68:
#line 1017 "grammar.y"
{
                       yyval = yyvsp[0];
                   ;
    break;}
case 69:
#line 1021 "grammar.y"
{
                       yyval = yyvsp[0];
                       if (yyvsp[0] == 1)
                       {
                           if (yyvsp[-1] == -1)
                           {
                               strcpy(top_str,"-");
                               strcat(top_str,number);
                               strcpy(number,top_str);
                           }
                           
                       }
                   ;
    break;}
case 70:
#line 1035 "grammar.y"
{
                        push(type_expression);
                        switch(no)
                        {
                             case ADD:
                                 if (write_in_c == 0)
                                     fprintf(hout," + ");
                                 else
                                     fprintf(fout," + ");
                                 is_logic = 0;
                                 break;
                             case SUB:
                                 if (write_in_c == 0)
                                     fprintf(hout," - ");
                                 else
                                     fprintf(fout," - ");
                                 is_logic = 0;
                                 break;
                             case OR:
                                 if (write_in_c == 0)
                                     fprintf(hout," || ");
                                 else
                                     fprintf(fout," || ");
                                 is_logic = 1;
                                 break;
                        }
                   ;
    break;}
case 71:
#line 1063 "grammar.y"
{
                       pop();
                       if (is_logic)//若为逻辑运算，则规约出的simple_expression类型直接为int
                           strcpy(type_expression,"int");
                       else//若为算术运算
                       {
                           if (strcmp(type_expression,"int")!=0 || strcmp(top_str,"int")!=0)//若参与运算的两个数均为int类型，则规约出的simple_expression也为int型，否则为float型
						   {
							    strcpy(type_expression,"float");
						   }
                       }
                       yyval = 0;
                   ;
    break;}
case 72:
#line 1077 "grammar.y"
{
                        yyvsp[0] = search(id_str);
                        if (yyvsp[0] == -1)
                        {
                             if (l_top != 1)
                             {
                                 l_pop();
                                 yyvsp[0] = search(id_str);
                                 if (yyvsp[0] == -1)
                                 {
                                     printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                                     yyvsp[0] = insert(id_str);
                                     strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                                 }
                                 upper_level = 1;
                                 l_push(l_top_str);
                             }
                             else                             
                             {
                                 printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                                 yyvsp[0] = insert(id_str);
                                 strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                                 upper_level = 0;
                             }
                        }
                        else
							 upper_level = 0;   						 
						for (i = 0;i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type) && l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type[i] != '-'; i++);
						if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "int",i+1,i+3) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))                 
						{
							printf("\nLine %d: Warning: %s is a function.\n",yylineno,id_str);
							strcpy(type_expression,"int");
						}
						else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
						{
							printf("\nLine %d: Warning: %s is a function.\n",yylineno,id_str);
							strcpy(type_expression,"float");
						}    
						else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
						{
							printf("\nLine %d: Warning: %s is a procedure.\n",yylineno,id_str);
							strcpy(type_expression,"int");
						}
						else
						{
							if (! exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "array",0,4)) 
							{
								printf("\nLine %d: Warning: %s is not an array.\n",id_str);
								strcpy(type_expression,"int");//将错误引用的数组名转化为int类型，以继续分析
							}
						}	 
										    
                        if (write_in_c == 0)
						{
						    fprintf(hout,id_str);
							fprintf(hout,"[");
						}
						else
						{
							fprintf(fout,id_str);
							fprintf(fout,"[");
						}
					;
    break;}
case 73:
#line 1141 "grammar.y"
{
					   
					    if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[-4]].type, "array",0,4) && is_const == 1)
                        {
                            get_range_number(l_stack[l_top-1-upper_level]->table[(int)yyvsp[-4]].type);
							if (atoi(number) < num1 || atoi(number) > num2)
							{
									printf("\nLine %d: Warning: Incorrect reference of the array:%s is out of range.\n",yylineno,number);
							}
						}
						if (write_in_c == 0)
						{
							fprintf(hout,"]");
						}
						else
						{
							fprintf(fout,"]");
						}
						if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[-4]].type, "array",0,4))
						    strcpy(type_expression,type_exp_a);
					;
    break;}
case 74:
#line 1164 "grammar.y"
{
           push(type_expression);
           switch(no)
           {
               case MUL:
                   if (write_in_c == 0)
                       fprintf(hout," * ");
                   else
                       fprintf(fout," * ");
                   is_logic = 0;
                   break;
               case DIV:case DVD:
                   if (write_in_c == 0)
                       fprintf(hout," / ");
                   else
                       fprintf(fout," / ");
                   is_logic = 0;
                   break;
               case MOD:
                   if (write_in_c == 0)
                       fprintf(hout," %% ");
                   else
                       fprintf(fout," %% ");
                   is_logic = 0;
                   break;
               case AND:
                   if (write_in_c == 0)
                       fprintf(hout," && ");
                   else
                       fprintf(fout," && ");
                   is_logic = 1;
                   break;
           }
       ;
    break;}
case 75:
#line 1199 "grammar.y"
{
           pop();
           if (is_logic)//若为逻辑运算，则term类型直接为int
               strcpy(type_expression,"int");
           else//若为算术运算
           {
               if (exp_check(type_expression,"array",0,4) || exp_check(top_str,"array",0,4))//若参与运算的数中有错误引用的数组名
               {
                   printf("\nLine %d: Warning: Incorrect reference of the array:lack of corner mark.\n",yylineno);
               } 
               else if (exp_check(type_expression,"record",0,5) || exp_check(top_str,"record",0,5))//若参与运算的数中有错误引用的记录名
               {
                   printf("\nLine %d: Warning: Wrong reference of the record.\n",yylineno);
               }
                             
               if (strcmp(type_expression,"int")!=0 || strcmp(top_str,"int")!=0)//若参与运算的两个数均为int类型，则规约出的term也为int型，否则为float型
               {
                   strcpy(type_expression,"float");
               }
               
           }
           yyval = 0;
       ;
    break;}
case 76:
#line 1223 "grammar.y"
{
           yyval = yyvsp[0];         
       ;
    break;}
case 77:
#line 1228 "grammar.y"
{                         
             yyvsp[0] = search(id_str);
             if (yyvsp[0] == -1)
             {
                  if (l_top != 1)
                  {
                      l_pop();
                      yyvsp[0] = search(id_str);
                      if (yyvsp[0] == -1)
                      {
                          printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                          yyvsp[0] = insert(id_str);
                          strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                      }
                      upper_level = 1;
                      l_push(l_top_str);
                  }
                  else                             
                  {
                      printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                      yyvsp[0] = insert(id_str);
                      strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                      upper_level = 0;
                  }
             }
             else
		         upper_level = 0;     
             for (i = 0;i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type) && l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type[i] != '-'; i++);
             if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "int",i+1,i+3) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))                 
             {
                 printf("\nLine %d: Warning: %s is a function.Lack of parameters.\n",yylineno,id_str);
                 strcpy(type_expression,"int");
             }
             else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
             {
                 printf("\nLine %d: Warning: %s is a function.Lack of parameters.\n",yylineno,id_str);
                 strcpy(type_expression,"float");
             }    
             else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
             {
                   printf("\nLine %d: Warning: Procedure has no return value.\n",yylineno);
                   strcpy(type_expression,"int");//将错误调用的过程处理为返回值为int类型的函数，以继续分析
             }
             else
             {
                   if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "array",0,4))
                   {
                       printf("\nLine %d: Warning: Wrong reference of the array:lack of corner mark.\n",yylineno);
                       strcpy(type_expression,"int");//将错误引用的数组名处理为为int类型，以继续分析
                   }
                   else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "record",0,5))
                   {
                       printf("\nLine %d: Warning: Wrong reference of the record.\n",yylineno);
                       strcpy(type_expression,"int");//将错误引用的结构名处理为为int类型，以继续分析
                   }
                   else
                       strcpy(type_expression,l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type);
                   
             }           
                                              
             if (write_in_c == 0)
                 fprintf(hout,id_str);
             else
                 fprintf(fout,id_str);
             yyval = 0;//表明该factor不是数
             
         ;
    break;}
case 78:
#line 1296 "grammar.y"
{
             yyvsp[0] = search(id_str);
             if (yyvsp[0] == -1)
             {
                 if (l_top != 1)
                 {
                    l_pop();
                    yyvsp[0] = search(id_str);
                    if (yyvsp[0] == -1)
                    {
                       printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                       yyvsp[0] = insert(id_str);
                       strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                    }
                    upper_level = 1;
                    l_push(l_top_str);
                 }
                 else                             
                 {
                    printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                    yyvsp[0] = insert(id_str);
                    strcpy(l_stack[0]->table[(int)yyvsp[0]].type,"int");//未定义就使用的变量，均认为是整型
                    upper_level = 0;
                 }
             }
             else
				 upper_level = 0;            
             for (i = 0;i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type) && l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type[i] != '-'; i++);
             if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "int",i+1,i+3) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))                 
             {
                 strcpy(type_expression,"int");
             }
             else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
             {
                 strcpy(type_expression,"float");
             }    
             else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type))
             {
                   printf("\nLine %d: Warning: Procedure has no return value.\n",yylineno);
                   strcpy(type_expression,"int");//将错误调用的过程处理为返回值为int类型的函数，以继续分析
             }
             else
             {
                   printf("\nLine %d: Warning: %s is not a function.\n",yylineno,id_str);    
                   strcpy(type_expression,"int");//将错误调用的函数处理为int类型的变量，以继续分析               
             }        
             
             para0 = para_number(l_stack[l_top-1-upper_level]->table[(int)yyvsp[0]].type);    
                 
             if (write_in_c == 0)
             {
                 fprintf(hout,id_str);
                 fprintf(hout,"(");
             }
             else
             {
                 fprintf(fout,id_str);
                 fprintf(fout,"(");
             }
                 
         ;
    break;}
case 79:
#line 1358 "grammar.y"
{
             if (write_in_c == 0)
             {
                 fprintf(hout,")");
                 for (i = 0;i<=strlen(l_stack[l_top-2]->table[(int)yyvsp[-4]].type) && l_stack[l_top-2]->table[(int)yyvsp[-4]].type[i] != '-'; i++);
				 if (exp_check(l_stack[l_top-2]->table[(int)yyvsp[-4]].type, "int",i+1,i+3) && i<=strlen(l_stack[l_top-2]->table[(int)yyvsp[-4]].type))                 
					strcpy(type_expression,"int");
				 else if (exp_check(l_stack[l_top-2]->table[(int)yyvsp[-4]].type, "float",i+1,i+5) && i<=strlen(l_stack[l_top-2]->table[(int)yyvsp[-4]].type))
					strcpy(type_expression,"float");
			     else if (exp_check(l_stack[l_top-2]->table[(int)yyvsp[-4]].type, "void",i+1,i+4) && i<=strlen(l_stack[l_top-2]->table[(int)yyvsp[-4]].type))
					strcpy(type_expression,"void");
				 else
				 {
					printf("\nLine:%d: %s is not a function or a procedure.\n",yylineno,l_stack[l_top-2]->table[(int)yyvsp[-4]].name);
					strcpy(type_expression,l_stack[l_top-2]->table[(int)yyvsp[-4]].type);
				 }
             }
             else
             {
                 fprintf(fout,")");
                 for (i = 0;i<=strlen(l_stack[l_top-1]->table[(int)yyvsp[-4]].type) && l_stack[l_top-1]->table[(int)yyvsp[-4]].type[i] != '-'; i++);
				 if (exp_check(l_stack[l_top-1]->table[(int)yyvsp[-4]].type, "int",i+1,i+3) && i<=strlen(l_stack[l_top-1]->table[(int)yyvsp[-4]].type))                 
					strcpy(type_expression,"int");
				 else if (exp_check(l_stack[l_top-1]->table[(int)yyvsp[-4]].type, "float",i+1,i+5) && i<=strlen(l_stack[l_top-1]->table[(int)yyvsp[-4]].type))
					strcpy(type_expression,"float");
			     else if (exp_check(l_stack[l_top-1]->table[(int)yyvsp[-4]].type, "void",i+1,i+4) && i<=strlen(l_stack[l_top-1]->table[(int)yyvsp[-4]].type))
					strcpy(type_expression,"void");
				 else
				 {
					printf("\nLine:%d: %s is not a function or a procedure.\n",yylineno,l_stack[l_top-1]->table[(int)yyvsp[-4]].name);
				    strcpy(type_expression,l_stack[l_top-2]->table[(int)yyvsp[-4]].type);	
				 }
             }
             if (para != para0)
                 printf("\nLine %d: Warning: The number of parameters is incorrect.\n",yylineno);
             else
             {
                 for (i = para0; i >= 1; i --)
                 {
                     pop();
                     if (strcmp(l_stack[0]->table[(int)yyvsp[-4]].list->table[i-1].type, top_str) != 0)
                     {
                         printf("\nLine %d: Warning: Parameter %d is of incorrect type.\n",yylineno,i);
                     }
                 }
             }    
             yyval = 0;//表明该factor不是常数
         ;
    break;}
case 80:
#line 1408 "grammar.y"
{
             pop();
             strcpy(type_expression,"int");
             if (write_in_c == 0)
                 fprintf(hout,top_str);
             else
                 fprintf(fout,top_str);
             for (i = 0; i <= strlen(top_str)-1 ; i++)
                 if (top_str[i] == '.')
                 {
                     strcpy(type_expression,"float");
                     break;
                 }             
             yyval = 1;//表明该factor是常数
             strcpy(number,top_str);
         ;
    break;}
case 81:
#line 1425 "grammar.y"
{
             if (write_in_c == 0)
             {
                 fprintf(hout,"(");
             }
             else
             {
                 fprintf(fout,"(");
             }         
          ;
    break;}
case 82:
#line 1436 "grammar.y"
{
             if (write_in_c == 0)
             {
                 fprintf(hout,")");
             }
             else
             {
                 fprintf(fout,")");
             }
             yyval = yyvsp[-2];         
          ;
    break;}
case 83:
#line 1448 "grammar.y"
{
             if (write_in_c == 0)
             {
                 fprintf(hout,"!");
                 fprintf(hout,"(");
             }
             else
             {
                 fprintf(fout,"!");
                 fprintf(fout,"(");
             }
         ;
    break;}
case 84:
#line 1461 "grammar.y"
{
         
             if (write_in_c == 0)
             {
                 fprintf(hout,")");
             }
             else
             {
                 fprintf(fout,")");
             }
             yyval = yyvsp[-1];
         ;
    break;}
case 85:
#line 1474 "grammar.y"
{
             if (write_in_c == 0)
                 fprintf(hout," 1 ");
             else
                 fprintf(fout," 1 ");
             yyval = 0;//表明该factor不是常数
             strcpy(type_expression,"int");
         ;
    break;}
case 86:
#line 1483 "grammar.y"
{
             if (write_in_c == 0)
                 fprintf(hout," 0 ");
             else
                 fprintf(fout," 0 ");
             yyval = 0;//表明该factor不是常数
             strcpy(type_expression,"int");
         ;
    break;}
case 87:
#line 1493 "grammar.y"
{
           if (no == ADD)
           {
             if (write_in_c == 0)
                 fprintf(hout," + ");
             else
                 fprintf(fout," + ");
             yyval = 1;
           }
           else if (no == SUB)
           {
             if (write_in_c == 0)
                 fprintf(hout," - ");
             else
                 fprintf(fout," - ");
             yyval = -1;  
           }
       ;
    break;}
case 88:
#line 1514 "grammar.y"
{
                   //进入向.c文件写入的状态
                   write_in_c = 1;
               ;
    break;}
case 89:
#line 1519 "grammar.y"
{
                   fprintf(fout,"\nprintf(\"\\n\");\nreturn 0;\n}\n");
               ;
    break;}
case 90:
#line 1526 "grammar.y"
{
                         
                         fprintf(hout,"\n}\n");
                         l_pop();                                                  
                      ;
    break;}
}

#line 606 "/usr/local/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (int) (sizeof (yytname) / sizeof (char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen (yytname[x]) + 15, count++;
	  size += strlen ("parse error, unexpected `") + 1;
	  size += strlen (yytname[YYTRANSLATE (yychar)]);
	  msg = (char *) malloc (size);
	  if (msg != 0)
	    {
	      strcpy (msg, "parse error, unexpected `");
	      strcat (msg, yytname[YYTRANSLATE (yychar)]);
	      strcat (msg, "'");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (int) (sizeof (yytname) / sizeof (char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat (msg, count == 0 ? ", expecting `" : " or `");
			strcat (msg, yytname[x]);
			strcat (msg, "'");
			count++;
		      }
		}
	      yyerror (msg);
	      free (msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#if YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#if YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 1533 "grammar.y"

int main(int argc, char *argv[])//主函数
{ 
    int i;
    char source_file[STRING_MAX],c_file[STRING_MAX];
    //打开PASCAL源文件
    if (argc > 1)
    {
		if((yyin = fopen(argv[1],"rt"))==NULL)
		{
			printf("Can't open file %s\n",argv[1]);
			return 0;
		} 
    }
    else
    {
        printf("Please drag the PASCAL source file to this program OR input the path:\n");
        scanf("%s",source_file);
        if((yyin = fopen(source_file,"rt"))==NULL)
		{
			printf("Can't open file %s\n",source_file);
			return 0;
		}       
    }
    //初始化临时栈
    for (i = 0; i <= STACK_SIZE - 1; i++)
    {
        memset(stack[i],'\0',sizeof(stack[i]));
    }
    top = 0;
    
    //初始化符号表栈
    for (i = 0; i <= STACK_SIZE - 1; i++)
    {
        l_stack[i] = NULL;
    }
    top = 0;
    l_top = 0;
    
    //初始的struct结构编号为0
    struct_no = 0;
    
    //创建目标C文件
    strcpy(c_file,"target.c");
    if ((fout = fopen(c_file, "wt")) == NULL)
    {
        printf("Can't write in the file %s\n",c_file);
        return 0;
    }
        
    //创建输出.h文件，用于存放函数
    strcpy(c_file,"target.h");
    if ((hout = fopen(c_file, "wt")) == NULL)
    {
        printf("Can't write in the file %s\n",c_file);
        return 0;
    }
    
    //为目标C文件添加预处理信息       
    fprintf(fout,"#include \"");
    fprintf(fout,c_file);
    fprintf(fout,"\"\n");
    
    //为输出.h文件添加预处理信息       
    fprintf(hout,"#include <stdlib.h>\n#include <stdio.h>\n#include <string.h>\n\n");
    
    //初始时是向.c文件中输出
    write_in_c = 1;
    
        
    //创建输出符号表文件，用于在翻译结束后输出符号表
    strcpy(c_file,"tokenlist.txt");
    if ((tout = fopen(c_file, "wt")) == NULL)
    {
        printf("Can't write in the file %s\n",c_file);
        return 0;
    }
    
    //翻译
	yyparse();
	
	//输出符号表到符号表文件
	test_out(1);
   
	//翻译完成
	printf("\nTranslation complete.\n\nPlease check out the result in target.c and target.h.\n\nAll the token lists are in tokenlist.txt.\n");
	
	fclose(fout);
	fclose(hout);
	fclose(tout);
	return 0;
}

/*临时栈相关函数=================================*/
int test_out(int type)//测试用函数，输出临时栈
{
    int i,j;
    printf("\n");
    if (type == 0)
    {
        for (i = 0; i <= STACK_SIZE - 1; i ++)
            if (strcmp(stack[i],"\0")!=0)
                printf("%d[ %s ]\n",i,stack[i]);
    }
    else if (type == 1)//输出符号表到符号表文件
    {
        fprintf(tout,"name\t\ttype\t\tdefine\n");
        //输出主表
        for (i = 0; i <= main_list.top - 1; i ++)
        {
            fprintf(tout,main_list.table[i].name);
            fprintf(tout,"\t\t");
            fprintf(tout,main_list.table[i].type);
            fprintf(tout,"\t\t");
            sprintf(number, "%d", main_list.table[i].define);
            fprintf(tout,number);
            fprintf(tout,"\n");
            
        }
        //输出所有结构、函数及过程的符号表
        for (i = 0; i <= main_list.top - 1; i++)
        {
            if (exp_check(main_list.table[i].type,"record",0,5) || 
                exp_check(main_list.table[i].type,"-int",strlen(main_list.table[i].type)-4,strlen(main_list.table[i].type)-1) ||
                exp_check(main_list.table[i].type,"-float",strlen(main_list.table[i].type)-6,strlen(main_list.table[i].type)-1) ||
                exp_check(main_list.table[i].type,"-void",strlen(main_list.table[i].type)-5,strlen(main_list.table[i].type)-1)
               )
            {
                fprintf(tout,"\n");
                fprintf(tout,main_list.table[i].name);
                fprintf(tout,":\n");
                fprintf(tout,"name\t\ttype\t\tdefine\n");
                for (j = 0; main_list.table[i].list!=NULL && j <= main_list.table[i].list->top - 1; j ++)                
                {
                    fprintf(tout,main_list.table[i].list->table[j].name);
					fprintf(tout,"\t\t");
					fprintf(tout,main_list.table[i].list->table[j].type);
					fprintf(tout,"\t\t");
					sprintf(number, "%d", main_list.table[i].list->table[j].define);
					fprintf(tout,number);
					fprintf(tout,"\n");
                }
            }
        }
    }
    else if (type == 2)
    {
        for (i = 0; i <= l_stack[l_top - 1]->top - 1; i ++)
        {
            printf("%d[ %s %s]",i,l_stack[l_top - 1]->table[i].name,l_stack[l_top - 1]->table[i].type);
            for (j = 0; j <= l_stack[l_top - 1]->table[i].atop - 1; j++)
                printf(" %d",l_stack[l_top - 1]->table[i].appear[j]);
            printf ("\n");
        }
    }    
    return 0;
}

int isEmpty(void)//判断临时栈是否为空
{
    if (top == 0)
        return 1;
    else
        return 0;
}

int push(char* temp)//入临时栈
{
    if (top < STACK_SIZE)
    {
        strcpy(stack[top++],temp);
        return 1;
    }
    else
        printf("The Stack is FULL!\n");
    return 0;
}

int pop(void)//弹出临时栈栈顶
{
    if (top > 0)
    {
        strcpy(top_str,stack[--top]);
        memset(stack[top],'\0',sizeof(stack[top]));
        return 1;
    }
    else
        printf("The Stack is EMPTY!The previous top_str is %s\n",top_str);
    return 0;
}

int isReal(void)//判断临时栈顶的数是否为real型
{
    int i,l;
    l = strlen(top_str);
    for (i = 0; i <= l - 1; i++)
        if (top_str[i] == '.')
             return 1;
    return 0;
}
int isFloat(char* number)//判断Number是否是浮点数
{
    int i;
    for (i = 0; i <= strlen(number) - 1; i ++)
        if (number[i] == '.')
            return 1;
    return 0;
}
int fout_id_list(int type,FILE* ofile)//输出ID列表
{
    int ptr,kind = 0;
    char pad_str[STRING_MAX] = "\0";   
    char temp_str[STRING_MAX] = "\0";
    memset(temp_str,'\0',sizeof(temp_str));
    pop();
    if (strcmp(top_str,"standard") == 0)
        kind = 0;
    else if (strcmp(top_str,"array") == 0) 
        kind = 1;
    else if (strcmp(top_str,"record") == 0)
        kind = 2;
    if (kind != 2)
        pop();
    if (strcmp(top_str,"float") == 0)
        strcpy(temp_str,"float ");
    else if (strcmp(top_str,"int") == 0)
        strcpy(temp_str,"int ");
    fprintf(ofile,temp_str);
    if (type == 0)
    {
        strcpy(temp_str,"\0");
        if (kind == 1)
        {
            strcpy(pad_str,"[");
            pop();
            num1 = atoi(top_str);
            pop();
            num2 = atoi(top_str);
            sprintf(pad_str+1, "%d", num1 + 1);
            strcat(pad_str,"]");
        }
    }
    if (type == 1)
    {
        if (kind == 1)
        {
            pop();
            num1 = atoi(top_str);
            pop();
            num2 = atoi(top_str);
            strcat(temp_str,"*"); 
            fprintf(ofile,"*");
        }
    }     
    //在C输出文件中输出ID名      
    fprintf(ofile,l_stack[l_top-1]->table[atoi(stack[cur_top])].name);
    //为ID的type赋值
    strcpy(l_stack[l_top-1]->table[atoi(stack[cur_top])].type,type_expression);
    
    //向记录结构的类型表达式字符串type_exp_r写入
    strcpy(type_exp_r,l_stack[l_top-1]->table[atoi(stack[cur_top])].name);
    strcat(type_exp_r,"*");
    strcat(type_exp_r,type_expression);
    
    //向函数及过程的类型表达式字符串type_exp_fp写入
    strcpy(type_exp_fp,type_expression);
    
    fprintf(ofile,pad_str);
    
    for (ptr = cur_top + 1; ptr < top; ptr ++)
    {
        fprintf(ofile,", ");
        fprintf(ofile,temp_str);
        //在输出文件中输出ID名
        fprintf(ofile,l_stack[l_top-1]->table[atoi(stack[ptr])].name);
        //为ID的type赋值    
        strcpy(l_stack[l_top-1]->table[atoi(stack[ptr])].type,type_expression);
        fprintf(ofile,pad_str);
        
        //向记录结构的类型表达式字符串type_exp_r写入
        strcat(type_exp_r," ");
        strcat(type_exp_r,l_stack[l_top-1]->table[atoi(stack[ptr])].name);
        strcat(type_exp_r,"*");
        strcat(type_exp_r,type_expression);
        
        //向函数及过程的类型表达式字符串type_exp_fp写入
        strcat(type_exp_fp," ");
        strcat(type_exp_fp,type_expression);
    }
    while (top > cur_top)
        pop();
    return 0;
}

/*===============================================*/


/*符号表栈相关函数===============================*/

int l_push(List* list)//入符号表栈
{
    if (l_top < STACK_SIZE)
    {
        l_stack[l_top ++] = list;
        return 1;
    }
    else
        printf("The List Stack is FULL!\n");
    return 0;
}
int l_pop(void)//出符号表栈
{
    if (l_top > 0)
    {
        l_top_str = l_stack[--l_top];
        l_stack[l_top] = (List*)NULL;
        return 1;
    }
    else
        printf("The List Stack is EMPTY!\n");
    return 0;
}
/*===============================================*/



int initialList(List* list)//符号表初始化函数
{
     int i;
     list->top = 0;
     for (i = 0; i <= LIST_SIZE - 1; i ++)
     {
         memset(list->table[i].name,'\0',sizeof(list->table[i].name));
         memset(list->table[i].type,'\0',sizeof(list->table[i].type));
         list->table[i].define = 0;
         list->table[i].atop = 0;
         memset(list->table[i].appear,0,sizeof(list->table[i].appear));
         list->table[i].list = NULL;
     }
     return 0;
}


/*符号表相关函数=================================*/


int insert(char* id)//插入符号表操作。若该标识符已存在，则返回-1，提示存在重定义错误；否则将该标识符插入符号表
{
    int i;
    for (i = 0; i <= l_stack[l_top - 1]->top - 1; i ++)
    {
        if (strcmp(l_stack[l_top - 1]->table[i].name,id) == 0)
            return -1;
    }
    strcpy (l_stack[l_top - 1]->table[l_stack[l_top - 1]->top].name, id);
    l_stack[l_top - 1]->table[l_stack[l_top - 1]->top].define = yylineno;
    l_stack[l_top - 1]->top ++;
    return l_stack[l_top - 1]->top - 1;
}


int search(char* id)//搜索符号表操作，若该标识符在当前符号表中存在，则返回其编号；否则返回-1，并提示错误
{
    int i,j,flag = 0;
    for (i = 0; i <= l_stack[l_top - 1]->top - 1; i ++)
    {
        if (strcmp(l_stack[l_top - 1]->table[i].name,id) == 0)
        {
            for (j = 0; j <= l_stack[l_top - 1]->table[i].atop; j ++)
                if (l_stack[l_top - 1]->table[i].appear[j] == yylineno)
                {
                     flag = 1;                         
                     break;
                }
            if (flag == 0)     
                l_stack[l_top - 1]->table[i].appear[l_stack[l_top - 1]->table[i].atop ++] = yylineno;                
            return i;
        }
    }
    return -1;
}
/*===============================================*/

int exp_check(char* des,char* src,int start,int end)//字符串比较函数，用于检查类型表达式
{
    int i,j;
    for (i = start, j = 0; i <= end ; i++)
    {
        if (des[i] != src[i - start])
            return 0;
    }
    return 1;
}
int para_number(char* type_expression)//统计函数及过程类型表达式中的参数个数
{
    int i,count = 0;
    for (i = 0; i <= strlen(type_expression) - 1 && type_expression[i] != '-'; i ++)
    {
        if (exp_check(type_expression,"int",i,i+2))
        {
            count ++;
        }
        else if (exp_check(type_expression,"float",i,i+4))
        {
            count ++;
        }
    }
    return count;
}

int get_range_number(char* type_expression)//获取数组类型表达式中的角标范围，分别保存在num1、num2中，同时将数组类型保存在type_exp_a中
{
    int i,j;
    char num_temp[STRING_MAX];
    memset(num_temp,'\0',sizeof(num_temp)); 
    for (i = 6 , j = 0;(i <= strlen(type_expression) - 1) && (type_expression[i] != '.'); i ++)
    {
        num_temp[j ++] = type_expression[i];
    }
    num1 = atoi(num_temp);
    memset(num_temp,'\0',sizeof(num_temp));    
    for (i = i + 2, j = 0;( i <= strlen(type_expression) - 1) && (type_expression[i] != ','); i ++)
    {
        num_temp[j ++] = type_expression[i];
    }
    num2 = atoi(num_temp);
    memset(type_exp_a,'\0',sizeof(type_exp_a));
    for (i = i + 1, j = 0; ( i <= strlen(type_expression) - 1) && (type_expression[i] != ')'); i ++)
        type_exp_a[j ++] = type_expression[i];
    return 0;
}


void yyerror(const char *s)
{
    printf("\nLine %d: Error: An error occured.Translation aborted\n",yylineno);
    puts(s);
}

