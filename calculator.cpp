#include<stdio.h>
int push(char* top,char c);
int pop(char* top);
int priority(char c);
void cal(char*);
int main(void)
{
	/*
		(char *):
		in_stack		stdin_stack
		f_stack			formula_stack
		o_stack 		operator_stack
		
		(int):
		f_top
		o_top
	*/
	
	
	//stack[char] simulation
	while(1)
	{
		printf("press a formula,press q to quit\n");
		char in_stack[100];
		scanf("%[^'\n']s",in_stack);	//get formula and transform it to RPN (reverse polish notation)
		getchar();
		fflush(stdin);
								//clear '\n' in case of infinite loop
		//printf("%s\n",in_stack);  debug(test the sort of cases)
		if(in_stack[0]=='q')
			break;
		
		char f_stack[100],o_stack[100];
		
		int f_top,o_top,scan;
		f_top=0,o_top=0,scan=0;
		
		while(in_stack[scan]!='\0')
		{
//			printf("-------\n%c\n",in_stack[scan]);
//			int situation=0;
			if(in_stack[scan]>='0'&&in_stack[scan]<='9')
			{
//				situation=1;
				push(&f_stack[f_top],in_stack[scan]);	
				f_top++;
			}
			if(in_stack[scan]=='+'||in_stack[scan]=='-'||in_stack[scan]=='*'||in_stack[scan]=='/')
			{
				if(o_top==0)
				{
//					situation=2;
					push(&o_stack[o_top],in_stack[scan]);
					o_top++;
				}
				else
				{
					if(priority(in_stack[scan])>priority(o_stack[o_top-1]))
					{
//						situation=3;
						push(&o_stack[o_top],in_stack[scan]);
						o_top++;
					}
					else
					{
//						situation=4;
						push(&f_stack[f_top],in_stack[scan]);
						f_top++;
					}
				}
			}
			if(in_stack[scan]=='(')
			{
				push(&o_stack[o_top],in_stack[scan]);
				o_top++;
			}
			if(in_stack[scan]==')')
			{
//				situation=5;
				while(o_stack[o_top-1]!='(')
				{
					char op=o_stack[o_top-1];
					pop(&o_stack[o_top-1]);
					o_top--;
					push(&f_stack[f_top],op);
					f_top++;
				}
				if(o_stack[o_top-1]=='(')
				{
					pop(&o_stack[o_top-1]);
					o_top--;
				}
			}
//			printf("%d-------\n\n",situation);
			scan++;
		}
		while(o_top!=0)
		{
			char op=o_stack[o_top-1];
			pop(&o_stack[o_top]);
			o_top--;
			push(&f_stack[f_top],op);
			f_top++;
		}
		printf("RPN is %s\n",f_stack);
		cal(f_stack);
	}
	return 0;
}

void cal(char* formula)
{
	int result;
	result=0;
	
	int d_stack[100];		//data_stack
	char op_stack[100];
	int op_top,d_top;
	op_top=0,d_top=0;
	int i=0;
	while(*(formula+i)!='\0')
	{
		char ch=*(formula+i);
		if(ch<='9'&&ch>='0')
		{
			d_stack[d_top]=(int)ch-'0';
			d_top++;
		}
		else if(ch=='+'||ch=='-'||ch=='*'||ch=='/')
		{
			op_stack[op_top]=ch;
			op_top++;
		}
		//calculate begin
		if(op_top>0&&d_top>1) //at least contains 2 num and 1 operator
		{
			int op_temp;
			op_top--;
			op_temp=op_stack[op_top];
			op_stack[op_top]='\0';
			
			
			d_top--;
			
			if(op_temp=='+')
			{
				d_stack[d_top-1]=d_stack[d_top-1]+d_stack[d_top];
			}
			else if(op_temp=='-')
			{
				d_stack[d_top-1]=d_stack[d_top-1]-d_stack[d_top];
			}
			else if(op_temp=='*')
			{
				d_stack[d_top-1]=d_stack[d_top-1]*d_stack[d_top];
			}
			else if(op_temp=='/')
			{
				d_stack[d_top-1]=d_stack[d_top-1]/d_stack[d_top];
			}
			
			d_stack[d_top]=0;
		}	
		i++;
	}
	printf("result is %d\n\n",d_stack[0]);
}

int push(char* top,char c)
{
	*top=c;
	return 1;
}
int pop(char* top)
{
	*top='\0';
	return 1;
}
int priority(char c)
{
	if(c=='+'||c=='-')
	{
		return 1;
	}
	else if(c=='*'||c=='/')
	{
		return 2;
	}
	else
		return 0;
}

// "Protocol: Mozilla" 
// HUSE CSE cyberodyssey
// nickname: ChangShan YeLao
// code in 2022/3

/*
	Chinese sentences are as follows (needs UTF-8 code)
	{
		1.扫描中缀式
		2.如果是数字，直接进入待输出
		3.如果是运算符, 若运算栈空，则直接入栈
		否则　若优先级高于栈内元素，则入栈
			　否则直接进入待输出 
		4.遇到括号如何解决
		'('的优先级最高
		遇到')'，则说明这是一个子式，将符号栈中最近的'('及以上的所有符号出栈，且去掉'(' 
	}
	EN
	{
		1.scan the formula
		2.if scan a num, push the num to stack_wait_to_print
		3.if scan a operator
			if operaor_stack is empty
				push the operator to o_stack
			else
				compare priority the lastest element of o_stack with the element which is being scanned
				{
					if the scanned element's priority is upper(only contains a>b) than the lastest element in stack's
						push the scanned element to o_stack
					else
						push the element to f_stack
				]
		4.if scan '(' or ')'
			if scan '('
				'(' is the element with uppest priority	, push it to o_stack
			if scan ')'
				')' means a sub formula, pop all elements between the lastest '(' a nd the top of stack, and push them to f_stack one by one
				tips:  while popping '(', don't push it to f_stack
	}
	RPN		reverse polish notation
		
	method:
		1+(2+3)*4
			
		formula		operator_stack	mark of ')'
		1
		1			+
		1			+ (					
		1 2			+ ( +
		1 2 3		+ ( + )			*
		123+		+ *
	 	123+4 		+ *
	 	EOF
	 	123+4*+
	 	
	 	caculate
	 	
	 	123+4*+
	 		23+		->5
		154*+
			54*		->20
		1 20 +
			1 20+	->21
		stack empty
		21	
		


*/
