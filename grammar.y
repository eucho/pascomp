%{
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

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

void yyerror(void);
//=======================================================================================================
%}

%token ID NUM ADDOP ASSIGNOP MULOP RELOP  
%token ARRAY PBEGIN BOOLEAN DO ELSE END FALSE FUNCTION IF INTEGER NOT OF PROCEDURE PROGRAM READ REAL RECORD THEN TRUE VAR WHILE WRITE
%token AND OR ADD SUB MUL DVD LT LE EQ GE GT NE DIV MOD

%left '+' '-'
%left '*' '/'
%left ';'
%%
program :M program_head program_body '.'
         ;
M :
  {
       initialList(&main_list);
       l_push(&main_list);
  }
  ;
program_head : PROGRAM ID '(' 
               {
                   cur_top = top;
               }
               identifier_list ')' ';'
               {
                   fprintf(fout,"int main(");
                   
                   push("int");
                   push("standard");
                   strcpy(type_expression,"int");
				   fout_id_list(1,fout);
                   fprintf(fout,"){\n");    
               }
             ;
identifier_list : identifier_list ',' ID
                {
                    itoa(insert(id_str), number,10);
                    push(number);
                }
                | ID
                {
                    itoa(insert(id_str), number,10);
                    push(number);
                }
                ;
identifier_list2 : identifier_list2 ',' ID
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
                }
                | ID
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
                }
                ;
declarations : VAR 
               {
                   write_in_c = 0;
               }
               declaration ';'
               {
                  
               }
             |
             ;
declaration : declaration ';' identifier_list ':' type
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
              }
            | identifier_list ':' type
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
			  }
            ;

type : standard_type
       {
           switch((int)$1)
           {
               case Int:case Bool:
                   strcpy(type_expression,"int");
                   break;
               case Real:
                   strcpy(type_expression,"float");
                   break;          
           }
           push("standard");
       }
     | ARRAY '[' NUM '.''.' NUM ']' OF standard_type
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
       }
     | RECORD 
       {           
           memset(type_exp_r,'\0',sizeof(type_exp_r));
           
           fprintf(hout,"struct s_");
           itoa(struct_no ++,number,10);
           fprintf(hout,number);
           fprintf(hout,"{\n");
           cur_top = top;
           
           //为记录创建符号表
           
           l_stack[l_top - 1]->table[atoi(stack[top-1])].list = malloc (sizeof(List));
           initialList(l_stack[l_top - 1]->table[atoi(stack[top-1])].list);
           l_push(l_stack[l_top - 1]->table[atoi(stack[top-1])].list);
           
       }
       declaration END
       {
           strcpy(type_expression,"record(");
           strcat(type_expression,type_exp_r);
           strcat(type_expression,")");           
           fprintf(hout,"}");
           cur_top = 0;
           push("record");
           l_pop();     
       }
     ;

standard_type : INTEGER
				{
				    push("int");
				    $$ = Int;
				}
              | REAL
                {
                    push("float");
                    $$ = Real;
                }
              | BOOLEAN
                {
                    push("int");
                    $$ = Bool;
                }
              | NUM '.''.' NUM
                {
                    pop();
                    if (isReal())
                    {
                        push("float");
                        $$ = Real;                        
                    }  
                    else
                    {
                        pop();
                        if (isReal())
                        {
                            push("float");
                            $$ = Real;
                        }
                        else
                        {
                            push("int");
                            $$ = Int;
                        }
                    }    
                }
              ;
subproc_declarations : subproc_declarations subproc_declaration ';'
                     |
                     ;
subproc_head : FUNCTION ID 
               {
                   $2 = insert(id_str);                                                         
                   cur_fptr_position = ftell(hout); 
                   for (spc = 0; spc < 7 + yyleng; spc ++)
                       fprintf(hout," ");
                   fprintf(hout,"(");                                     
                   fflush(hout);
                   
                   //为函数创建符号表
                   l_stack[l_top - 1]->table[(int)$2].list = malloc (sizeof(List));
                   initialList(l_stack[l_top - 1]->table[(int)$2].list);
                   l_push(l_stack[l_top - 1]->table[(int)$2].list);
                   
                   memset(type_exp_fp,'\0',sizeof(type_exp_fp));
                   
               }
               arguments ':' standard_type ';'
               {
                   fprintf(hout,"){\n");
                   pop();
                   fptr_offset = cur_fptr_position - ftell(hout);
                   fseek(hout,fptr_offset,1);
                   fprintf(hout,top_str);
                   fseek(hout,1,1);  
                   fprintf(hout,l_stack[l_top - 2]->table[(int)$2].name);                       
                   fflush(hout);                                 
                   fseek(hout,- fptr_offset - (ftell(hout) - cur_fptr_position),1);
                   
                   //为函数类型表达式赋值
                   strcat(type_exp_fp,"-");
                   strcat(type_exp_fp,top_str);
                   strcpy(l_stack[l_top - 2]->table[(int)$2].type,type_exp_fp);
                   
               }   
             | PROCEDURE ID 
               {
                   $2 = insert(id_str);
                   cur_fptr_position = ftell(hout); 
                   for (spc = 0; spc < 6 + yyleng; spc ++)
                       fprintf(hout," ");
                   fprintf(hout,"(");                                     
                   fflush(hout);
                   
                   //为过程创建符号表
                   l_stack[l_top - 1]->table[(int)$2].list = malloc (sizeof(List));
                   initialList(l_stack[l_top - 1]->table[(int)$2].list);
                   l_push(l_stack[l_top - 1]->table[(int)$2].list);
                   
                   memset(type_exp_fp,'\0',sizeof(type_exp_fp));
               }
               arguments ';'
               {
                   fprintf(hout,"){");
                   fptr_offset = cur_fptr_position - ftell(hout);
                   fseek(hout,fptr_offset,1);
                   fprintf(hout,"void");
                   fseek(hout,1,1);  
                   fprintf(hout,l_stack[l_top - 2]->table[(int)$2].name);                    
                   fflush(hout);                                 
                   fseek(hout,- fptr_offset - (ftell(hout) - cur_fptr_position),1);
                   
                   //为过程类型表达式赋值
                   strcat(type_exp_fp,"-");
                   strcat(type_exp_fp,"void");
                   strcpy(l_stack[l_top - 2]->table[(int)$2].type,type_exp_fp);                  

               }
             ;
arguments : '(' parameter_list ')'
          |
          ;
parameter_list : parameter_list ';' identifier_list ':' type
                 {     
                      fprintf(hout,", ");             
                      fout_id_list(1,hout);
                      fflush(hout);
                 }
               | identifier_list ':' type
                 {
                      fout_id_list(1,hout);
				      fflush(hout);
                 }
               |
                 {                 
                      fprintf(hout,"void");                      
                      fflush(hout);
                 }
               ;
compound_statement : PBEGIN
                     optional_statements
                     END
                   ;
optional_statements : statement_list
                    |
                    ;
statement_list : statement_list ';'                  
                 statement
               | statement
               ;
statement : variable ASSIGNOP 
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
                
            }
            expression
            {
                if (strcmp(type_expression,"int") != strcmp(type_exp_v,"int"))
                    printf("\nLine %d: Warning: Conversion from integer to float.\n",yylineno);
                if (write_in_c == 0)
                    fprintf(hout,";\n");
                else
                    fprintf(fout,";\n");
                fflush(hout);
				fflush(fout);
            }
                      | procedure_statement
            {
                if (write_in_c == 0)
                    fprintf(hout,";\n");
                else
                    fprintf(fout,";\n");
            }
          | compound_statement

          | IF 
            {
                if (write_in_c == 0)
                    fprintf(hout,"if (");
                else
                    fprintf(fout,"if (");
            }
            expression 
            {
                if (write_in_c == 0)
                    fprintf(hout,"){\n");
                else
                    fprintf(fout,"){\n");
            }
            THEN statement 
            {
                if (write_in_c == 0)
                    fprintf(hout,"}\n");
                else
                    fprintf(fout,"}\n");
            }
            ELSE 
            {
                if (write_in_c == 0)
                    fprintf(hout,"else {\n");
                else
                    fprintf(fout,"else {\n");
            }
            statement
            {
                if (write_in_c == 0)
                    fprintf(hout,"} ");
                else
                    fprintf(fout,"} ");
            }
          | WHILE 
            {
                if (write_in_c == 0)
                    fprintf(hout,"while (");
                else
                    fprintf(fout,"while (");
            }
            expression 
            {
                if (write_in_c == 0)
                    fprintf(hout,"){\n");
                else
                    fprintf(fout,"){\n");
            }
            DO statement
            {
                if (write_in_c == 0)
                    fprintf(hout,"}\n");
                else
                    fprintf(fout,"}\n");
            }
          | READ '(' identifier_list2 ')'
          
          | WRITE '(' expression_list2 ')'
          ;
variable : ID
           {
               $1 = search(id_str);
               if ($1 == -1)
               {
                  if (l_top != 1)
                  {
                     l_pop();
                     $1 = search(id_str);
                     if ($1 == -1)
                     {
                         printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                         $1 = insert(id_str);
                         strcpy(l_stack[0]->table[(int)$1].type,"int");//未定义就使用的变量，均认为是整型
                     }
                     upper_level = 1;
                     l_push(l_top_str);
                  }
                  else                             
                  {
                     printf("\nLine %d: Warning: %s is undefined.\n",yylineno,id_str);
                     $1 = insert(id_str);
                     strcpy(l_stack[0]->table[(int)$1].type,"int");//未定义就使用的变量，均认为是整型
                     upper_level = 0;
                  }
               }
               else
				   upper_level = 0;                                               
               for (i = 0;i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type) && l_stack[l_top-1-upper_level]->table[(int)$1].type[i] != '-'; i++);
                
               
               if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "int",i+1,i+3) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))                 
               {
                   is_function = 1;
                   push("int");
               }
               else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
               {
                   is_function = 1;
                   push("float");
               }    
               else if (upper_level == 0 && exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
               {
                   printf("\nLine %d: Warning: Procedure has no return value.\n",yylineno);
                   is_function = 1;
                   push("int");//将错误调用的过程处理为int类型的变量，以继续分析
               }
               else
               {
                   is_function = 0;
                   if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "array",0,4))
                   {
                       printf("\nLine %d: Warning: Incorrect reference of the array:lack of corner mark.\n",yylineno);
                       push("int");//将错误引用的数组名处理为int类型的变量，以继续分析
                   }
                   else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "record",0,5))
                   {
                       printf("\nLine %d: Warning: Incorrect reference of the record.\n",yylineno);
                       push("int");//将错误引用的记�