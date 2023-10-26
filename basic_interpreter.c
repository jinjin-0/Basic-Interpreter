/* Basic Interpreter by Hüseyin Uslu raistlinthewiz@hotmail.com */
/* Code licenced under GPL */


#include <stdio.h>								//표준 입출력 함수를 사용할 수 있도록 해주는 헤더 파일
#include <conio.h>								//콘솔 입출력 함수를 사용할 수 있도록 해주는 헤더 파일	
#include <string.h>								//문자열 처리 함수를 사용할 수 있도록 해주는 헤더 파일
#include <stdlib.h>								//일반적인 유틸리티 함수를 사용할 수 있도록 해주는 헤더 파일
#include <ctype.h>								//문자형(char) 데이터와 관련된 함수들을 사용할 수 있도록 해주는 헤더 파일

struct node{									//struct node라는 구조체 정의
	int type;								//int 타입의 노드 정수
/* system stack-> 1 for variable, 2 for function , 3 for function call		//시스템 스택 값 지정 1.변수, 2.함수, 3.함수 호출, 4.시작, 5.끝
4 for begin 5 for end */

	char exp_data;								//변수 : 노드와 관련된 데이터를 나타내는 문자(char) exp_data지정
	int val;								//변수 : 노드 정수값 지정 변수나 함수 호출 때 사용됨
	int line;								//변수 : 노드가 속한 코드의 라인 번호 나타냄
	struct node * next;							//변수 : 연결 리스트 데이터 구조를 위한 포인터 - 다음 노드를 가리키며 연결 리스트의 다음 요소를 가리킴

};

typedef struct node Node;							//struct node 라는 구조체를 Node라는 새로운 이름으로 정의하고 선언함.

struct stack{									//struct stack라는 구조체를 정의하고 Node의 포인터인 top 하나만음 멤버로 가짐. 이러한 형태는 스택 자료구조를 표현함
	Node * top;
};

typedef  struct stack Stack;							//struct stack을 Stack이라는 새로운 이름으로 정의하고 선언함.


struct opnode{									//struct opnode라는 구조체를 정의함.
	char op;								//연산자를 나타내는 문자 
	struct opnode * next;							//다음 노드를 가리키는 포인터이며 이 포인터를 사용하여 연결 리스트를 형성함
};
typedef struct opnode opNode;							//struct opnode를 opNode이라는 새로운 이름으로 정의하고 선언함.

struct opstack{									//struct opstack이라는 구조체를 정의함
	opNode * top;								//opNode 포인터인 top을 가지고 이 포인터는 스택의 맨 위 노드를 가리킴.

typedef struct opstack OpStack;							// "opstack"이라는 구조체를 "OpStack"으로 typedef합니다.

struct postfixnode{								// "postfixnode" 구조체를 정의합니다. 이 구조체는 정수 값을 저장하고, 다음 노드를 가리키는 포인터를 가집니다.
	int val;
	struct postfixnode * next;
};

typedef struct postfixnode Postfixnode;						// "postfixnode" 구조체를 "Postfixnode"로 typedef합니다.

struct postfixstack{								// "postfixstack" 구조체를 정의합니다. 이 구조체는 후입선출 (LIFO) 스택을 나타냅니다. "top" 멤버는 스택의 맨 위 항목을 가리키는 포인터입니다.
	Postfixnode * top;
	};

typedef struct postfixstack PostfixStack;					// "postfixstack" 구조체를 "PostfixStack"으로 typedef합니다.





int GetVal(char,int *,Stack *);							//함수 "GetVal"를 선언하며 char,int *,Stack *를 인자로 받아 정수(int)값을 반환함
int GetLastFunctionCall(Stack*);						//함수 "GetLastFunctionCall"를 선언하며 Stack *를 인자로 받아 정수(int)값을 반환함
	
Stack* FreeAll(Stack *);							//함수 "FreeAll"를 선언하며 Stack *를 인자로 받아 Stack*을 반환함

Stack * Push(Node sNode,Stack *stck)						//Push 함수를 정의하고, Node 타입의 sNode와 스택 포인터 stck를 인자로 받으며, 스택 포인터 Stack *를 반환함
{
Node *newnode;
// 새로운 노드를 할당하고 메모리가 할당되지 않으면 오류 메시지 출력
if ((newnode=(Node*)malloc(sizeof(Node)))==NULL) {
printf("ERROR, Couldn't allocate memory...");
return NULL;
}
else
{
	 // 새로운 노드에 sNode의 필드 값을 복사
newnode->type=sNode.type;
newnode->val=sNode.val;
newnode->exp_data=sNode.exp_data;
newnode->line=sNode.line;
	   // 새로운 노드를 스택의 맨 위로 추가
newnode->next=stck->top;
stck->top=newnode;
	 // 업데이트된 스택 포인터를 반환
return stck;
}
}

OpStack * PushOp(char op,OpStack *opstck)					//char 타입의 연산자 op와 연산자 스택 포인터 opstck를 인자로 받으며, 연산자 스택 포인터 OpStack *을 반환함
{
opNode *newnode;
	  // 새로운 노드를 할당하고 메모리가 할당되지 않으면 오류 메시지 출력
if ((newnode=(opNode*)malloc(sizeof(opNode)))==NULL) {
printf("ERROR, Couldn't allocate memory...");
return NULL;
}
else
{
	 // 새로운 노드에 연산자(op)를 설정
newnode->op=op;
	 // 새로운 노드를 연산자 스택의 맨 위에 추가
newnode->next=opstck->top;
opstck->top=newnode;
	  // 업데이트된 연산자 스택 포인터를 반환
return opstck;
}
}

char PopOp(OpStack *opstck)							//연산자 스택에서 연산자를 팝(pop)하는 PopOp 함수의 구현, 이 함수는 OpStack * 타입의 연산자 스택 포인터 opstck를 인자로 받으며, 팝된 연산자를 char로 반환함
{
opNode *temp;
char op;
	// 스택이 비어있는 경우
if (opstck->top == NULL )
{
printf("ERROR, empty stack...");
}
else
{
	// 팝할 연산자 값을 저장
op=opstck->top->op;
	 // 현재 맨 위의 노드를 임시 포인터 'temp'에 저장
temp=opstck->top;
	  // 스택의 맨 위 노드를 다음 노드로 업데이트
opstck->top=opstck->top->next;
	// 이전 맨 위 노드의 메모리를 해제
free(temp);
	// 팝된 연산자를 반환
return op;
}
	// 스택이 비어있을 경우 NULL을 반환
return NULL;
}


PostfixStack * PushPostfix(int val,PostfixStack *poststck)			 //후위 표기식 스택에 정수를 추가하는 PushPostfix 함수의 구현, 이 함수는 int 타입의 정수 val과 후위 표기식 스택 포인터 poststck를 인자로 받으며, 후위 표기식 스택 포인터 PostfixStack *를 반환
{
Postfixnode *newnode;
	// 새로운 노드를 할당하고 메모리가 할당되지 않으면 오류 메시지 출력
if ((newnode=(Postfixnode*)malloc(sizeof(Postfixnode)))==NULL) {
printf("ERROR, Couldn't allocate memory...");
return NULL;
}
else
{
	        // 새로운 노드에 정수(val)를 설정
newnode->val=val;
	        // 새로운 노드를 후위 표기식 스택의 맨 위에 추가
newnode->next=poststck->top;
poststck->top=newnode;
	// 업데이트된 후위 표기식 스택 포인터를 반환
return poststck;
}
}

char PopPostfix(PostfixStack *poststck)						//후위 표기식 스택에서 정수를 팝(pop)하는 PopPostfix 함수의 구현, 이 함수는 PostfixStack * 타입의 후위 표기식 스택 포인터 poststck를 인자로 받으며, 팝된 정수를 int로 반환
{
Postfixnode *temp;
int val;
	// 스택이 비어 있는지 확인하기 위해 `poststck->top`을 검사합니다.
if (poststck->top == NULL )
{
printf("ERROR, empty stack...");
}
else
{
	// 팝할 정수 값을 저장
val=poststck->top->val;
	// 현재 맨 위의 노드를 임시 포인터 'temp'에 저장
temp=poststck->top;
	// 스택의 맨 위 노드를 다음 노드로 업데이트
poststck->top=poststck->top->next;
	// 이전 맨 위 노드의 메모리를 해제
free(temp);
	// 팝된 정수 값을 반환
return val;
}
	// 스택이 비어 있을 경우 NULL을 반환
return NULL;
}





void Pop(Node * sNode,Stack *stck)						//스택에서 요소를 팝(pop)하여 요소의 값을 sNode에 복사하는 Pop 함수의 구현,. 이 함수는 sNode에 대한 포인터와 스택 포인터 stck를 인자로 받으며, 스택에서 팝된 요소의 값을 sNode에 복사
{
Node *temp;
	 // 스택이 비어 있는지 확인하기 위해 `stck->top`을 검사합니다.
if (stck->top == NULL )
{
printf("ERROR, empty stack...");
}
else
{
	// 팝된 요소의 필드 값을 `sNode`에 복사합니다
sNode->exp_data=stck->top->exp_data;
sNode->type=stck->top->type;
sNode->line=stck->top->line;
sNode->val=stck->top->val;
	// 현재 맨 위의 노드를 임시 포인터 'temp'에 저장
temp=stck->top;
	// 스택의 맨 위 노드를 다음 노드로 업데이트
stck->top=stck->top->next;
	// 이전 맨 위 노드의 메모리를 해제
free(temp);
}
}

int isStackEmpty(OpStack *stck)							//isStackEmpty 함수는 스택이 비어 있는지를 확인하는 함수, OpStack 구조체 포인터를 매개변수로 받음.
{
	 // if 문: 스택의 'top' 멤버가 0인 경우
	if (stck->top==0)
		// 반환문: 스택이 비어 있지 않다면 0을 반환하여 표시합니다.
		return 1;
// 반환문: 스택이 비어 있지 않다면 0을 반환하여 표시합니다.
return 0;
}
//주석 처리되어 실행되지않음
/*void printAllStack(Stack * stck)						// printAllStack라는 함수를 정의 스택 내부의 데이터를 출력하는 것
{
	//Node 구조체의 tempNode 변수를 선언
	Node tempNode;
 //printf를 사용하여 스택 내부의 데이터를 출력합니다.
	printf("\n------------------\n");
	printf("dumping the stack...\n");
	Pop(&tempNode,stck);
	printf("exp=%c type=%d val=%d 	line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line);
	while( (stck->top!=NULL) ) {
		Pop(&tempNode,stck);
		printf("exp=%c type=%d val=%d 				line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line);
	}
} */

int Priotry(char operator)
{
	if ((operator=='+') | (operator=='-'))
		return 1;
	else if ((operator=='/') | (operator=='*'))
		return 2;
return 0;
}

int main(int argc,char ** argv)
{
	char line[4096];
	char dummy[4096];
	char lineyedek[4096];

	char postfix[4096];

	char * firstword;



//	int i;
	int val1;
	int val2;


	int LastExpReturn;
	int LastFunctionReturn=-999;
	int CalingFunctionArgVal;

	Node tempNode;


	OpStack * MathStack;

	FILE *filePtr;

	PostfixStack * CalcStack;

	int resultVal;

	Stack * STACK;

	int curLine=0;
	int foundMain=0;
        int WillBreak=0;


	MathStack->top=NULL;
	CalcStack->top=NULL;
	STACK->top=NULL;
	clrscr();

	if (argc!=2)
	{
		/* if argument count is =1 */
		printf("Incorrect arguments!\n");
		printf("Usage: %s <inputfile.spl>",argv[0]);
		return 1;
	}





	/* open the file */

	if ( ( filePtr=fopen(argv[1],"r") ) == NULL )
	{
		printf("Can't open %s. Check the file please",argv[1]);
		return 2;
	}


	while (!feof(filePtr))
	{

		int k=0;

		fgets(line,4096,filePtr); /* read the file by Line by Line */
		/* scan for /t characters. get rid of them! */
		while(line[k]!='\0')
		{
		 if (line[k]=='\t')
		 {
			line[k]=' ';
		 }

		 k++;
		}

		strcpy(lineyedek,line);

		curLine++;
		tempNode.val=-999;
		tempNode.exp_data=' ';
		tempNode.line=-999;
		tempNode.type=-999;



		if (!strcmpi("begin\n",line) | !strcmpi("begin",line))
		{
			if (foundMain)
			{
				tempNode.type=4;
				STACK=Push(tempNode,STACK);
			}
		}
		else if (!strcmpi("end\n",line) | !strcmpi("end",line) )
		{
			if (foundMain)
			{
				int sline;


				tempNode.type=5;
				STACK=Push(tempNode,STACK);

				sline=GetLastFunctionCall(STACK);
				if (sline==0)
				{
					/* WE FOUND THE RESULT! */
					printf("Output=%d",LastExpReturn);

				}
				else
				{
					int j;
					int foundCall=0;
					LastFunctionReturn=LastExpReturn;
					/* get to the last line that have been a function calling */

						fclose(filePtr);
							filePtr=fopen(argv[1],"r");
							curLine=0;
							/* file reversed to start postion */
							/* now go codeline lines to go, to the functions line */
							for(j=1;j<sline;j++)
							{
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */
								curLine++;
							}


						/* clear all the stack up to the last function call */
						while(foundCall==0)
						{
							Pop(&tempNode,STACK);
							if (tempNode.type==3)
							{
								foundCall=1;
							}
						}

				}


			}
		}
		else
		{
			//we need to tokinize
			firstword=strtok(line," ");


			if (!strcmpi("int",firstword))
			{
				if (foundMain)
				{
					tempNode.type=1; /*integer*/
					firstword=strtok(NULL," ");
					tempNode.exp_data=firstword[0];


					firstword=strtok(NULL," ");

					/* check for = */
					if (!strcmpi("=",firstword))
					{
						firstword=strtok(NULL," ");
					}

					tempNode.val=atoi(firstword);
					tempNode.line=0;
					STACK=Push(tempNode,STACK);
				}
			}
			else if (!strcmpi("function",firstword))
			{
				tempNode.type=2;
				firstword=strtok(NULL," ");
				tempNode.exp_data=firstword[0];
				tempNode.line=curLine;
				tempNode.val=0;
				STACK=Push(tempNode,STACK);

				if ( (firstword[0]=='m') & (firstword[1]=='a') & (firstword[2]=='i') & (firstword[3]=='n') )
				{
					/*printf("Found function main() in line %d. Starting to running the script...\n",curLine);*/
					foundMain=1;


				}
				else
				{
					if (foundMain)
					{
						firstword=strtok(NULL," ");
						tempNode.type=1;
						tempNode.exp_data=firstword[0];
						tempNode.val=CalingFunctionArgVal;
						tempNode.line=0;
						STACK=Push(tempNode,STACK);



					}

				}


			}
			else if (firstword[0]=='(')
			{

			if (foundMain)
				{

				int i=0;
				int y=0;


				MathStack->top=NULL;
				/* now make the postfix calculcation */





				while(lineyedek[i]!='\x0')
				{
					/* evulate the function */
					if (isdigit(lineyedek[i])) {
						postfix[y]=lineyedek[i];
						y++;
					}
				     /*	else if (lineyedek[i]=='(')
					{
						MathStack=PushOp(lineyedek[i],MathStack);
					}*/

					else if (lineyedek[i]==')')
					{
						if (!isStackEmpty(MathStack))
						{
							postfix[y]=PopOp(MathStack);
							y++;
						}
					}
					else if ((lineyedek[i]=='+') | (lineyedek[i]=='-') | (lineyedek[i]=='*') | (lineyedek[i]=='/'))
					{
						/*operators*/
						if (isStackEmpty(MathStack))
						{
					/* if stack empty push the operator to stack */

							MathStack=PushOp(lineyedek[i],MathStack);
						}
						else
						{
							/* check for presedence */
							if (Priotry(lineyedek[i]) <= Priotry(MathStack->top->op) )
							{
								/* higher presedence for example + < *  */
								/* pop the last operator */

								/* add the poped operator to the postfix */
								postfix[y]=PopOp(MathStack);
								y++;


								MathStack=PushOp(lineyedek[i],MathStack);


							}
							else
							{
								/* lower presedence for example / > + */
								/* push to stack */

								MathStack=PushOp(lineyedek[i],MathStack);
							}

						}
					}
					else if (isalpha(lineyedek[i])>0)
					{
						int codeline=0;
						int dummyint=0;
						/*look if it's a variable or function call 						*/
						int retVal=0;
						retVal=GetVal(lineyedek[i],&codeline,STACK);
						if ((retVal!=-1) & (retVal!=-999))
						{
							/* if variable */
							postfix[y]=retVal+48; /* in ascii table numeric values start from 48 */
							y++;

						}
						else
						{

						    if (LastFunctionReturn==-999)
						    {

							/* if function */
							/* add to our system stack that we are making a call to function */
							int j;
							tempNode.type=3;
							tempNode.line=curLine;
							STACK=Push(tempNode,STACK);

							/* get function's arguments value */
							CalingFunctionArgVal=GetVal(lineyedek[i+2],&dummyint,STACK);



							fclose(filePtr);
							filePtr=fopen(argv[1],"r");
							curLine=0;
							/* file reversed to start postion */
							/* now go codeline lines to go, to the functions line */


/* !!!!!!!!!!!!!!! j <= codeline olabilir */
							for(j=1;j<codeline;j++)
							{
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */
								curLine++;
							}

							WillBreak=1;
							break;
						    }
						    else

						    {


							postfix[y]=LastFunctionReturn+48; /* in ascii table numeric values start from 48 */
							y++;
							i=i+3;
							LastFunctionReturn=-999;


						    }
						}
					}

					i++;
				}

				if (WillBreak==0)
				{
				/* get out items left in the mathstack */
				while (isStackEmpty(MathStack)==0)
				{

					/* add the poped operator to the postfix */
					postfix[y]=PopOp(MathStack);
					y++;
				}

				postfix[y]='\0';

				//MathStack=FreeAll(MathStack);

				/* now calculate the postfix */
				/*printf("\nCURRENT POSTFIX=%s\n",postfix);*/

				i=0;

				CalcStack->top=NULL;
				while(postfix[i]!='\x0')
				{
					if (isdigit(postfix[i])) {
						/* push to stack */
						CalcStack=PushPostfix(postfix[i]-'0',CalcStack);
					}
					else if ((postfix[i]=='+') | (postfix[i]=='-') | (postfix[i]=='*') | (postfix[i]=='/'))
					{
						val1=PopPostfix(CalcStack);

						val2=PopPostfix(CalcStack);


						switch (postfix[i])
						{
							case '+': resultVal=val2+val1;break;
							case '-': resultVal=val2-val1;break;
							case '/': resultVal=val2/val1;break;
							case '*': resultVal=val2*val1;break;
						}

						CalcStack=PushPostfix(resultVal,CalcStack);
					}
					i++;
				}

				//CalcStack=FreeAll(CalcStack);
				LastExpReturn=CalcStack->top->val;


				}
				WillBreak=0;
				}
			}
		}


	}

	fclose(filePtr);

	//printAllStack(STACK);
	STACK=FreeAll(STACK);

	printf("\nPress a key to exit...");
	getch();
	return  0;
}

Stack * FreeAll(Stack * stck)
{
Node * temp;
Node * head;

	if (stck->top != NULL )
	{
		head=stck->top;
		do
		{

			temp=head;
			head=head->next;
			free(temp);

		} while (head->next!=NULL);
	}

return NULL;
}
int GetLastFunctionCall(Stack *stck)
{
Node * head;

	if (stck->top == NULL )
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head=stck->top;
		do
		{
			if ( head->type==3 )
			{
				return head->line;
			}
			else
			{
				head=head->next;
			}
		} while (head->next!=NULL);
	}

	return 0;



}

int GetVal(char exp_name,int * line,Stack *stck)
{

Node * head;
*line=0;
	if (stck->top == NULL )
	{
		printf("ERROR, empty stack...");
	}
	else
	{
		head=stck->top;
		do
		{
			if ( head->exp_data==exp_name )
			{

				if (head->type==1)
				{
					/* return the variables value */
					return head->val;
				}
				else if (head->type==2)
				{
					*line=head->line;
					return -1;
					/* it's a function so return -1 */
				}

			}
			else
			{
				head=head->next;
			}
		} while (head->next!=NULL);
		/* check agin once more */
		if (head->exp_data==exp_name)
		{

				if (head->type==1)
				{
					/* return the variables value */
					return head->val;
				}
				else if (head->type==2)
				{
					*line=head->line;
					return -1;
					/* it's a function so return -1 */
				}


		}
	}

	return -999;
}

