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
                       push("int");//将错误引用的记录名处理为int类型的变量，以继续分析
                   }
                   else
                   {
                       push(l_stack[l_top-1-upper_level]->table[(int)$1].type);
                   }
                   if (write_in_c == 0) 
                       fprintf(hout,id_str);
				   else
                       fprintf(fout,id_str);
               }                   
           }
         | ID 
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
					    
			    if (! exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "array",0,4)) 
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
               itoa(upper_level,number,10);
               push(number);
                   
           }
           '[' expression ']'
           {
               pop();
               upper_level = atoi(top_str);
               if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "array",0,4) && is_const == 1)
               {
                   get_range_number(l_stack[l_top-1-upper_level]->table[(int)$1].type);
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
               get_range_number(l_stack[l_top-1-upper_level]->table[(int)$1].type);
               push(type_exp_a);
           }
         ;
procedure_statement : ID
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

                          }
                          else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
                          {

                          }    
                          else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
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
                      }
                    | ID 
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

                          }
                          else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
                          {
                          
                          }    
                          else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
                          {
                             
                          }
                          else
						  {
							  printf("\nLine %d: Warning: %s is not a function or a procedure.\n",yylineno,id_str);                   
						  }
						  
						  para0 = para_number(l_stack[l_top-1-upper_level]->table[(int)$1].type);
						  
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
                      }
                      '(' expression_list ')'
                      {
                          if (para != para0)
                              printf("\nLine %d: Warning: The number of parameters is incorrect.\n",yylineno);
                          else
                          {
                              for (i = para0; i >= 1; i --)
                              {
                                  pop();
                                  if (strcmp(l_stack[0]->table[(int)$1].list->table[i-1].type, top_str) != 0)
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
                          
                      }
                    ;
expression_list : expression_list ',' 
                  {
                      if (write_in_c == 0)
                          fprintf(hout,", ");
                      else
                          fprintf(fout,", ");
                  }
                  expression
                  {
                      para ++;
                      push(type_expression);
                  }
                | expression
                  {
                      para = 1;
                      push(type_expression);
                  }
                ;
expression_list2 : expression_list2 ',' 
                  {
                      if (write_in_c == 0)
                          fprintf(hout,", ");
                      else
                          fprintf(fout,", ");
                  }
                  K expression
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
                  }
                | K expression
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
                  }
                ;
K:
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
 }
 ;
expression : simple_expression RELOP 
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
             }
             simple_expression
             {
                 strcpy(type_expression,"int");
                 is_const = 0;
             }
           | simple_expression
             {
                is_const = $1;
             }
           ;
simple_expression :term
                   {
                       $$ = $1;
                   }
                  |sign term
                   {
                       $$ = $2;
                       if ($2 == 1)
                       {
                           if ($1 == -1)
                           {
                               strcpy(top_str,"-");
                               strcat(top_str,number);
                               strcpy(number,top_str);
                           }
                           
                       }
                   }
                  |simple_expression ADDOP 
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
                   }
                   term
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
                       $$ = 0;
                   }
                  |ID 
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
							printf("\nLine %d: Warning: %s is a function.\n",yylineno,id_str);
							strcpy(type_expression,"int");
						}
						else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
						{
							printf("\nLine %d: Warning: %s is a function.\n",yylineno,id_str);
							strcpy(type_expression,"float");
						}    
						else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
						{
							printf("\nLine %d: Warning: %s is a procedure.\n",yylineno,id_str);
							strcpy(type_expression,"int");
						}
						else
						{
							if (! exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "array",0,4)) 
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
					}
					'[' expression ']'
					{
					   
					    if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "array",0,4) && is_const == 1)
                        {
                            get_range_number(l_stack[l_top-1-upper_level]->table[(int)$1].type);
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
						if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "array",0,4))
						    strcpy(type_expression,type_exp_a);
					}
                  ;
term : term MULOP 
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
       }
       factor
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
           $$ = 0;
       }
     | factor
       {
           $$ = $1;         
       }
     ;
factor : ID
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
                 printf("\nLine %d: Warning: %s is a function.Lack of parameters.\n",yylineno,id_str);
                 strcpy(type_expression,"int");
             }
             else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
             {
                 printf("\nLine %d: Warning: %s is a function.Lack of parameters.\n",yylineno,id_str);
                 strcpy(type_expression,"float");
             }    
             else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
             {
                   printf("\nLine %d: Warning: Procedure has no return value.\n",yylineno);
                   strcpy(type_expression,"int");//将错误调用的过程处理为返回值为int类型的函数，以继续分析
             }
             else
             {
                   if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "array",0,4))
                   {
                       printf("\nLine %d: Warning: Wrong reference of the array:lack of corner mark.\n",yylineno);
                       strcpy(type_expression,"int");//将错误引用的数组名处理为为int类型，以继续分析
                   }
                   else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "record",0,5))
                   {
                       printf("\nLine %d: Warning: Wrong reference of the record.\n",yylineno);
                       strcpy(type_expression,"int");//将错误引用的结构名处理为为int类型，以继续分析
                   }
                   else
                       strcpy(type_expression,l_stack[l_top-1-upper_level]->table[(int)$1].type);
                   
             }           
                                              
             if (write_in_c == 0)
                 fprintf(hout,id_str);
             else
                 fprintf(fout,id_str);
             $$ = 0;//表明该factor不是数
             
         }
       | ID 
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
                 strcpy(type_expression,"int");
             }
             else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "float",i+1,i+5) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
             {
                 strcpy(type_expression,"float");
             }    
             else if (exp_check(l_stack[l_top-1-upper_level]->table[(int)$1].type, "void",i+1,i+4) && i <= strlen(l_stack[l_top-1-upper_level]->table[(int)$1].type))
             {
                   printf("\nLine %d: Warning: Procedure has no return value.\n",yylineno);
                   strcpy(type_expression,"int");//将错误调用的过程处理为返回值为int类型的函数，以继续分析
             }
             else
             {
                   printf("\nLine %d: Warning: %s is not a function.\n",yylineno,id_str);    
                   strcpy(type_expression,"int");//将错误调用的函数处理为int类型的变量，以继续分析               
             }        
             
             para0 = para_number(l_stack[l_top-1-upper_level]->table[(int)$1].type);    
                 
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
                 
         }
         '(' expression_list ')'
         {
             if (write_in_c == 0)
             {
                 fprintf(hout,")");
                 for (i = 0;i<=strlen(l_stack[l_top-2]->table[(int)$1].type) && l_stack[l_top-2]->table[(int)$1].type[i] != '-'; i++);
				 if (exp_check(l_stack[l_top-2]->table[(int)$1].type, "int",i+1,i+3) && i<=strlen(l_stack[l_top-2]->table[(int)$1].type))                 
					strcpy(type_expression,"int");
				 else if (exp_check(l_stack[l_top-2]->table[(int)$1].type, "float",i+1,i+5) && i<=strlen(l_stack[l_top-2]->table[(int)$1].type))
					strcpy(type_expression,"float");
			     else if (exp_check(l_stack[l_top-2]->table[(int)$1].type, "void",i+1,i+4) && i<=strlen(l_stack[l_top-2]->table[(int)$1].type))
					strcpy(type_expression,"void");
				 else
				 {
					printf("\nLine:%d: %s is not a function or a procedure.\n",yylineno,l_stack[l_top-2]->table[(int)$1].name);
					strcpy(type_expression,l_stack[l_top-2]->table[(int)$1].type);
				 }
             }
             else
             {
                 fprintf(fout,")");
                 for (i = 0;i<=strlen(l_stack[l_top-1]->table[(int)$1].type) && l_stack[l_top-1]->table[(int)$1].type[i] != '-'; i++);
				 if (exp_check(l_stack[l_top-1]->table[(int)$1].type, "int",i+1,i+3) && i<=strlen(l_stack[l_top-1]->table[(int)$1].type))                 
					strcpy(type_expression,"int");
				 else if (exp_check(l_stack[l_top-1]->table[(int)$1].type, "float",i+1,i+5) && i<=strlen(l_stack[l_top-1]->table[(int)$1].type))
					strcpy(type_expression,"float");
			     else if (exp_check(l_stack[l_top-1]->table[(int)$1].type, "void",i+1,i+4) && i<=strlen(l_stack[l_top-1]->table[(int)$1].type))
					strcpy(type_expression,"void");
				 else
				 {
					printf("\nLine:%d: %s is not a function or a procedure.\n",yylineno,l_stack[l_top-1]->table[(int)$1].name);
				    strcpy(type_expression,l_stack[l_top-2]->table[(int)$1].type);	
				 }
             }
             if (para != para0)
                 printf("\nLine %d: Warning: The number of parameters is incorrect.\n",yylineno);
             else
             {
                 for (i = para0; i >= 1; i --)
                 {
                     pop();
                     if (strcmp(l_stack[0]->table[(int)$1].list->table[i-1].type, top_str) != 0)
                     {
                         printf("\nLine %d: Warning: Parameter %d is of incorrect type.\n",yylineno,i);
                     }
                 }
             }    
             $$ = 0;//表明该factor不是常数
         }
         
       | NUM
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
             $$ = 1;//表明该factor是常数
             strcpy(number,top_str);
         }
       | '(' 
          {
             if (write_in_c == 0)
             {
                 fprintf(hout,"(");
             }
             else
             {
                 fprintf(fout,"(");
             }         
          }       
          expression ')'
          {
             if (write_in_c == 0)
             {
                 fprintf(hout,")");
             }
             else
             {
                 fprintf(fout,")");
             }
             $$ = $2;         
          }         
       | NOT 
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
         }
         factor
         {
         
             if (write_in_c == 0)
             {
                 fprintf(hout,")");
             }
             else
             {
                 fprintf(fout,")");
             }
             $$ = $2;
         }
       | TRUE
         {
             if (write_in_c == 0)
                 fprintf(hout," 1 ");
             else
                 fprintf(fout," 1 ");
             $$ = 0;//表明该factor不是常数
             strcpy(type_expression,"int");
         }
       | FALSE
         {
             if (write_in_c == 0)
                 fprintf(hout," 0 ");
             else
                 fprintf(fout," 0 ");
             $$ = 0;//表明该factor不是常数
             strcpy(type_expression,"int");
         }
       ;
sign : ADDOP       
       {
           if (no == ADD)
           {
             if (write_in_c == 0)
                 fprintf(hout," + ");
             else
                 fprintf(fout," + ");
             $$ = 1;
           }
           else if (no == SUB)
           {
             if (write_in_c == 0)
                 fprintf(hout," - ");
             else
                 fprintf(fout," - ");
             $$ = -1;  
           }
       }
     ;
program_body : declarations 
               subproc_declarations 
               {
                   //进入向.c文件写入的状态
                   write_in_c = 1;
               }
               compound_statement
               {
                   fprintf(fout,"\nprintf(\"\\n\");\nsystem(\"pause\");\nreturn 0;\n}\n");
               }
             ;
subproc_declaration : subproc_head
                      declarations
                      compound_statement
                      {
                         
                         fprintf(hout,"\n}\n");
                         l_pop();                                                  
                      }
                      ;

%%
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
			system("pause");
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
			system("pause");
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
        system("pause");
        return 0;
    }
        
    //创建输出.h文件，用于存放函数
    strcpy(c_file,"target.h");
    if ((hout = fopen(c_file, "wt")) == NULL)
    {
        printf("Can't write in the file %s\n",c_file);
        system("pause");
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
        system("pause");
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
	system("pause");		 		
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
            itoa(main_list.table[i].define,number,10);
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
					itoa(main_list.table[i].list->table[j].define,number,10);
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
            itoa(num1 + 1, pad_str + 1, 10);
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


void yyerror(void)
{
    printf("\nLine %d: Error: An error occured.Translation aborted\n",yylineno);
}