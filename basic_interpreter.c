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
//Pop 함수를 사용하여 스택에서 데이터를 팝
	Pop(&tempNode,stck);
 //출력된 데이터의 필드(exp_data, type, val, line)는 형식화되어 출력
	printf("exp=%c type=%d val=%d 	line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line);
 //while 루프를 사용하여 스택이 비어 있지 않은 동안 반복하여 데이터를 팝하고 출력
	while( (stck->top!=NULL) ) {
		Pop(&tempNode,stck);
		printf("exp=%c type=%d val=%d 				line=%d\n",tempNode.exp_data,tempNode.type,tempNode.val,tempNode.line);
	}
} */

int Priotry(char operator)							//Priority 함수는 주어진 연산자의 우선순위를 결정함, char 형식의 연산자를 인자로 받음
{
	// if 문: 주어진 연산자가 '+' 또는 '-'인 경우
	if ((operator=='+') | (operator=='-'))
		// 반환문: 1을 반환하여 연산자의 우선순위를 1로 표시합니다.
		return 1;
	// else if 문: 주어진 연산자가 '/' 또는 '*'인 경우
	else if ((operator=='/') | (operator=='*'))
		// 반환문: 2를 반환하여 연산자의 우선순위를 2로 표시합니다.
		return 2;
// 반환문: 위의 조건에 해당하지 않는 경우, 0을 반환하여 기본 우선순위를 나타냅니다.
return 0;
}

int main(int argc,char ** argv)		//C 프로그램의 시작점인 main 함수, 이 함수는 커맨드 라인 매개변수(argc, argv)를 사용하여 실행됨
{
	char line[4096];		//4096개의 문자로 구성된 문자열을 저장하기 위한 것
	char dummy[4096];		//4096개의 문자로 구성된 문자열을 저장하기 위한 배열로, 임시 데이터나 중간 결과를 저장
	char lineyedek[4096];		//다른 4096개의 문자로 구성된 문자열을 저장하는 배열로, 아마도 원래 line 배열의 백업 복사본으로 사용

	char postfix[4096];		//4096개의 문자로 구성된 문자열을 저장하기 위한 배열입니다. 이 배열은 아마도 후위 표기식을 저장

	char * firstword;		//문자열을 가리키는 포인터로, 아마도 line 배열의 첫 번째 단어를 가리킴



//	int i;				//루프 또는 반복 작업에서 사용되는 카운터 또는 반복 변수로 사용,코드의 루프에서 반복 횟수를 추적하는 데 사용
	int val1;			//프로그램의 연산 중에 두 개의 숫자 값을 저장하거나 연산 결과를 보관하는 데 사용
	int val2;


	int LastExpReturn;		//변수는 이전 표현식의 결과 값을 저장하는 데 사용될 수 있음
	int LastFunctionReturn=-999;	//이전 함수 호출의 반환 값을 저장하는 데 사용 -999의 의믜는 아직 반환값이 없음
	int CalingFunctionArgVal;	//현재 호출된 함수에 전달된 인수(인자)의 값을 저장하는 데 사용

	Node tempNode;			//사용자 정의된 Node 구조체를 나타내며, 프로그램에서 노드 또는 데이터 구조를 임시로 저장하는 데 사용


	OpStack * MathStack;		//OpStack 타입의 포인터 변수 MathStack을 선언

	FILE *filePtr;			//파일을 다루기 위한 파일 포인터 filePtr을 선언

	PostfixStack * CalcStack;	//PostfixStack 타입의 포인터 변수 CalcStack을 선언

	int resultVal;			//정수형 변수 resultVal을 선언

	Stack * STACK;			//Stack 타입의 포인터 변수 STACK을 선언

	int curLine=0;			//정수형 변수 curLine을 0으로 초기화
	int foundMain=0;		//정수형 변수 foundMain을 0으로 초기화
        int WillBreak=0;		//정수형 변수 WillBreak을 0으로 초기화


	MathStack->top=NULL;		//MathStack가 가리키는 OpStack 구조체의 top 멤버를 NULL로 초기화
	CalcStack->top=NULL;		//alcStack가 가리키는 PostfixStack 구조체의 top 멤버를 NULL로 초기화
	STACK->top=NULL;		//STACK이 가리키는 Stack 구조체의 top 멤버를 NULL로 초기화
	clrscr();			//화면을 지우는 함수인 clrscr()를 호출합니다. 화면을 지워서 콘솔을 깨끗한 상태로 초기화

	if (argc!=2)			// 만약 커맨드 라인 인수의 개수가 2개가 아니면 다음을 수행
	{
		/* if argument count is =1 */
		printf("Incorrect arguments!\n");		//오류 메시지를 출력하고, 정확한 사용법을 사용자에게 알려주는 메시지를 출력
		printf("Usage: %s <inputfile.spl>",argv[0]);	//argv는 커맨드 라인 인수를 저장하는 문자열 배열입니다. argv[0]는 프로그램의 이름
		return 1;					//프로그램을 1로 종료
	}





	/* open the file */

	if ( ( filePtr=fopen(argv[1],"r") ) == NULL )				//fopen(argv[1], "r")은 argv[1]에 지정된 파일을 읽기 모드로 열려고 시도,argv[1]은 커맨드 라인 인수로 받은 파일의 경로를 나타냄
										//fopen 함수가 파일을 성공적으로 열면 파일 포인터를 반환하고, 열지 못하면 NULL을 반환
	{									//파일 포인터 filePtr가 NULL인 경우 실행
		printf("Can't open %s. Check the file please",argv[1]);		//오류 메시지를 출력하고, 파일 이름을 포함한 오류 메시지를 출력한 후
		return 2;							//프로그램을 2로 종료
	}


	while (!feof(filePtr))
		// 파일 끝까지 반복적으로 실행
	{

		int k=0;

		fgets(line,4096,filePtr); /* read the file by Line by Line */ // 파일을 한 줄씩 읽어옴
		/* scan for /t characters. get rid of them! */
		// 탭 문자를 공백으로 변환
		while(line[k]!='\0')
		{
		 if (line[k]=='\t')
		 {
			line[k]=' ';
		 }

		 k++;
		}
		
		strcpy(lineyedek,line);		// lineyedek 배열에 현재 줄(line)을 복사

		curLine++;	// 현재 라인 번호 증가

		// 임시 노드(tempNode)를 초기화합니다
		tempNode.val=-999;		
		tempNode.exp_data=' ';
		tempNode.line=-999;
		tempNode.type=-999;


	`	// "begin" 또는 "begin\n"을 찾았을 때
		if (!strcmpi("begin\n",line) | !strcmpi("begin",line))
		{
			if (foundMain)
			{
				tempNode.type=4;	//"begin"을 나타내는 타입 코드를 할당
				STACK=Push(tempNode,STACK);	// 스택에 노드를 푸시
			}
		}
		// "end" 또는 "end\n"을 찾았을 때
		else if (!strcmpi("end\n",line) | !strcmpi("end",line) )
		{
			if (foundMain)
			{
				int sline;
				tempNode.type=5;			// "end"를 나타내는 타입 코드를 할당
				STACK=Push(tempNode,STACK);		// 스택에 노드를 푸시
				sline=GetLastFunctionCall(STACK);	// 이전 함수 호출 라인을 찾습니다
				if (sline==0)
				{
					/* WE FOUND THE RESULT! */	// 결과를 찾았을 때
					printf("Output=%d",LastExpReturn);

				}
				else
				{
					int j;
					int foundCall=0;
					LastFunctionReturn=LastExpReturn;
					/* get to the last line that have been a function calling */	 // 파일을 다시 열고 해당 라인까지 이동

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


						/* clear all the stack up to the last function call */// 함수 호출과 관련된 스택 처리
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
			//we need to tokinize	// 변수 또는 함수 정의 라인 처리
			firstword=strtok(line," ");


			if (!strcmpi("int",firstword))
			{
				if (foundMain)
				{
					// 정수형 변수 처리
					tempNode.type=1;  // integer를 나타내는 타입 코드를 할당
					firstword=strtok(NULL," ");
					tempNode.exp_data=firstword[0];


					firstword=strtok(NULL," ");

					/* check for = */ 
					if (!strcmpi("=",firstword))	// "=" 검사
					{
						firstword=strtok(NULL," ");
					}

					tempNode.val=atoi(firstword);
					tempNode.line=0;
					STACK=Push(tempNode,STACK);  	// 스택에 노드를 푸시
				}
			}
			else if (!strcmpi("function",firstword))
			{
				// 함수 정의 처리
				tempNode.type=2;	// 함수를 나타내는 타입 코드를 할당
				firstword=strtok(NULL," ");
				tempNode.exp_data=firstword[0];
				tempNode.line=curLine;
				tempNode.val=0;
				STACK=Push(tempNode,STACK);	// 스택에 노드를 푸시

				if ( (firstword[0]=='m') & (firstword[1]=='a') & (firstword[2]=='i') & (firstword[3]=='n') )
				{
					/*printf("Found function main() in line %d. Starting to running the script...\n",curLine);*/
					foundMain=1; 		// main 함수를 찾았을 때 플래그를 설정
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
						STACK=Push(tempNode,STACK);	// 스택에 노드를 푸시
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

				while(lineyedek[i]!='\x0')	// 후위 표기식 계산
				{
					/* evulate the function */	// 숫자 처리
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
						/*operators*/	// 연산자 처리
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
							/* if variable */ // 변수 처리
							postfix[y]=retVal+48; /* in ascii table numeric values start from 48 */
							y++;
						}
						else
						{
						    if (LastFunctionReturn==-999)
						    {
							/* if function */ // 함수 호출 처리
							/* add to our system stack that we are making a call to function */
							int j;
							tempNode.type=3;
							tempNode.line=curLine;
							STACK=Push(tempNode,STACK);

							/* get function's arguments value */ 
							CalingFunctionArgVal=GetVal(lineyedek[i+2],&dummyint,STACK);



							fclose(filePtr); // 파일을 다시 열고 해당 라인까지 이동
							filePtr=fopen(argv[1],"r");
							curLine=0;
							/* file reversed to start postion */
							/* now go codeline lines to go, to the functions line */


/* !!!!!!!!!!!!!!! j <= codeline olabilir */
							for(j=1;j<codeline;j++)
							{
								// 파일을 줄 단위로 읽어오기
								fgets(dummy,4096,filePtr); /* read the file by Line by Line */
								curLine++;
							}

							WillBreak=1;	// 이 플래그를 설정하여 현재 루프를 중지하고 나중에 이어서 실행
							break;
						    }
						    else
						    {
							// 이전 함수 호출 결과를 사용
							// 숫자를 문자로 변환해서 후위 표기식에 추가
							postfix[y]=LastFunctionReturn+48; /* in ascii table numeric values start from 48 */
							y++;
							i=i+3;				// i를 3 증가시켜서 함수 호출 표기부분을 넘어가기
							LastFunctionReturn=-999;	// 이전 함수 호출 결과를 초기화
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
					// 남은 항목을 MathStack에서 가져와서 후위 표기식에 추가
					postfix[y]=PopOp(MathStack);
					y++;
				}

				postfix[y]='\0';	// 후위 표기식 문자열을 종료

				//MathStack=FreeAll(MathStack);

				/* now calculate the postfix */
				/*printf("\nCURRENT POSTFIX=%s\n",postfix);*/
				// 후위 표기식을 계산
				int i=0;
				CalcStack->top=NULL;
				while(postfix[i]!='\x0')
				{
					if (isdigit(postfix[i])) {
						/* push to stack */ // 숫자인 경우 스택에 푸시
						CalcStack=PushPostfix(postfix[i]-'0',CalcStack);
					}
					else if ((postfix[i]=='+') | (postfix[i]=='-') | (postfix[i]=='*') | (postfix[i]=='/'))
					{]
						// 연산자인 경우 스택에서 숫자를 팝하여 계산하고 결과를 스택에 푸시
						val1=PopPostfix(CalcStack);
						val2=PopPostfix(CalcStack);
						
						switch (postfix[i])
						{
							case '+': resultVal=val2+val1;break;
							case '-': resultVal=val2-val1;break;
							case '/': resultVal=val2/val1;break;
							case '*': resultVal=val2*val1;break;
						}
						// 최종 계산 결과를 LastExpReturn에 저장
						CalcStack=PushPostfix(resultVal,CalcStack);
					}
					i++;
				}
				//CalcStack=FreeAll(CalcStack); // 여기에서 후위 표기식이 계산
				LastExpReturn=CalcStack->top->val ;
				}
				WillBreak=0;	//WillBreak 플래그를 초기화
				}
			}
		}
	}

	fclose(filePtr);	// 파일을 닫기

	//printAllStack(STACK);	// 주석 처리된 코드, 스택의 내용을 출력하는 함수를 호출
	STACK=FreeAll(STACK);	// 스택 메모리를 해제하고 스택을 초기화

	printf("\nPress a key to exit...");	// 화면에 메시지를 출력
	getch();				// 사용자의 키 입력을 대기합니다.
	return  0;				// 프로그램을 종료하고 0을 반환합니다.
}

Stack * FreeAll(Stack * stck)		//FreeAll함수는 스택을 메모리에서 해제하고 스택 포인터를 초기화하는 역할을 함
{
Node * temp;
Node * head;

	if (stck->top != NULL )		// 스택의 맨 위 노드를 가리키는 포인터를 head로 설정
	{
		head=stck->top;
		do
		{
			temp=head; 	// 현재 노드를 temp에 저장
			head=head->next;// 다음 노드로 이동
			free(temp);	// 현재 노드를 메모리에서 해제
		} while (head->next!=NULL);	// 스택의 끝까지 반복
	}
return NULL;					// 스택을 모두 해제하고 포인터를 초기화하고 NULL을 반환
}

int GetLastFunctionCall(Stack *stck)		//GetLastFunctionCall함수는 스택에서 마지막 함수 호출의 라인 번호를 검색
{
Node * head;

	if (stck->top == NULL )
	{
		printf("ERROR, empty stack...");	// 에러 메시지를 출력하고 스택이 비어있을 때 -1을 반환
	}
	else
	{
		head=stck->top;				// 스택의 맨 위 노드를 가리키는 포인터를 head로 설정
		do
		{
			if ( head->type==3 )	
			{
				return head->line;	// 스택에서 마지막으로 발견된 함수 호출의 라인 번호를 반환
			}
			else
			{
				head=head->next;	// 다음 노드로 이동
			}
		} while (head->next!=NULL);		// 스택의 끝까지 반복
	}
	return 0;					// 함수 호출이 발견되지 않으면 0을 반환
}
int GetVal(char exp_name,int * line,Stack *stck)	//etVal 함수는 스택에서 변수 또는 함수 호출의 값을 가져오는 역할을 함
{							//exp_name: 가져올 변수 또는 함수 호출의 이름, line: 함수 호출의 경우 함수의 라인 번호를 저장하는 포인터
Node * head;
*line=0;						// line 변수를 0으로 초기화
	if (stck->top == NULL )
	{
		printf("ERROR, empty stack..."); 	// 에러 메시지를 출력하고 스택이 비어있을 때 -1을 반환
	}
	else
	{
		head=stck->top;				// 스택의 맨 위 노드를 가리키는 포인터를 head로 설정
		do
		{
			if ( head->exp_data==exp_name )
			{
		
				if (head->type==1)
				{
					/* return the variables value */
					/* 변수인 경우 변수의 값을 반환합니다. */
					return head->val;
				}
				else if (head->type==2)
				{
					*line=head->line;	// 함수 호출인 경우 함수의 라인 번호를 설정
					return -1;		// 함수 호출인 경우 -1을 반환
					/* it's a function so return -1 */
				}
			}
			else
			{
				head=head->next;		// 다음 노드로 이동
			}
		} while (head->next!=NULL);			// 스택의 끝까지 반복
		/* check agin once more */	/* 한 번 더 확인 */
		if (head->exp_data==exp_name)
		{

				if (head->type==1)
				{
					/* return the variables value */
					/* 변수인 경우 변수의 값을 반환합니다. */
					return head->val;
				}
				else if (head->type==2)
				{
					*line=head->line;	// 함수 호출인 경우 함수의 라인 번호를 설정
					return -1;		 // 함수 호출인 경우 -1을 반환
					/* it's a function so return -1 */
				}


		}
	}
	return -999;	// 변수나 함수 호출이 발견되지 않으면 -999를 반환
}
