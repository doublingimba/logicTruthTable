#define		_CRT_SECURE_NO_WARNINGS
#include	<stdio.h>
#include	<stdlib.h>
#include	<stddef.h>
#include	<string.h>
#include	<math.h>
//测试案例 ~a+b*c*(~d+e)+~(f+g)
#define		MAXSTRLEN 1024

int		priority(char input);	//判断优先级
void	sort();					//用于排序
void	calcuate();				//计算真值
void	translet();				//将中缀表达式转换成后缀表达式
void	getOperate();			//将后缀表达式中的变量提取出来，并去掉重复的部分

int result              =0;     //储存每次计算以及最终结果
char *resultStack		= NULL;	//存储每个中间变量
char *compute			= NULL;	//用于计算的栈
char *digitalNum		= NULL;	//变量二进制值
char *operate			= NULL;	//变量
char *inputStack		= NULL;	//输入
char *operationStack	= NULL;	//操作符
char *mediaStack		= NULL;	//中间栈
char *temp				= NULL;	//临时指针
char *s2				= NULL;	//通用指针
char *s1				= NULL;	//通用指针
int i = 0, j = 0;
int *operateNum			= NULL;	//从最终结果式中提取的变量个数

int main()
{
	printf("说明：本程序可生成任意逻辑表达式（变量不超过26个）的真值表\n");
	printf("格式说明：非：~，与：*，或：+，划分层次：()\n");
	printf("请全部在英文环境中使用\n");
	/*输入并转换为大写*/
	inputStack		=	(char *)malloc(MAXSTRLEN * sizeof(char) + 1);
	memset				(inputStack, 0,MAXSTRLEN * sizeof(char) + 1);

	printf("请输入表达式：");
	scanf				("%s", inputStack);
	_strupr				(inputStack);

	/*初始化操作符栈和中间结果栈*/
	operationStack	=	(char *)malloc(		strlen(inputStack) * sizeof(char) + 1);
	memset				(operationStack, 0, strlen(inputStack) * sizeof(char) + 1);

	mediaStack		=	(char *)malloc(	strlen(inputStack) * sizeof(char) + 1);
	memset				(mediaStack, 0, strlen(inputStack) * sizeof(char) + 1);

	translet();

	/*提取变量到变量栈*/
	operate			=	(char *)malloc(	strlen(mediaStack) * sizeof(char) + 1);
	memset				(operate, 0,	strlen(mediaStack) * sizeof(char) + 1);

	getOperate			();
    sort				();

	/*产生二进制数,并生成真值表*/
	operateNum		=	(int *)malloc(	strlen(operate) * sizeof(int) + 1);
	memset				(operateNum, 0, strlen(operate) * sizeof(int) + 1);

	digitalNum		=	(char *)malloc(	strlen(operate) * sizeof(char) + 1);
	memset				(digitalNum, 0, strlen(operate) * sizeof(char) + 1);

	compute			=	(char *)malloc(	strlen(operate) * sizeof(char) + 1);
	memset				(compute, 0,	strlen(operate) * sizeof(char) + 1);

	/*表头*/
	for (i = 0; i < strlen(operate); i++)
	{
		printf			("%c ", *(operate + i));
	}
	printf				("| X\n");
	/*分隔线*/
	for (i = 0; i<strlen(operate) + 2; i++)
		printf			("--");
	printf				("\n");
	/*真值表部分*/
	for (i = 0; i	<	(int)pow(2, strlen(operate)); i++)
	{
		s1			=	compute;
		_itoa			(i, digitalNum, 2);
		if (strlen(digitalNum)<strlen(operate))
		{
			for (j = 0; j < strlen(operate) - strlen(digitalNum); j++)
			{
				printf	("0 ");
				*s1++ = '0';
			}	
		}
		for (j = 0; j<strlen(digitalNum); j++)
		{
			printf		("%c ", *(digitalNum + j));
			*s1++ = *	(digitalNum + j);
		}
		*s1 = '\0';
		printf("| ");
		calcuate();
		printf("\n");
	}
	
	

	/*统一释放内存和指向NULL*/
	free(inputStack);
	free(digitalNum);
	free(operate);
	free(operateNum);
	free(operationStack);
	free(mediaStack);

	digitalNum		= NULL;
	operate			= NULL;
	inputStack		= NULL;
	operationStack	= NULL;
	mediaStack		= NULL;
	temp			= NULL;
	s2				= NULL;
	s1				= NULL;
	operateNum		= NULL;

	system("pause");
	return 0;
}

int priority(char input)
{
	switch (input)
	{
	case '~':return 3; break;
	case '*':return 2; break;
	case '+':return 1; break;
	case '(':return 0; break;
	case ')':return 0; break;
	default:return 0;
	}
}

void translet()
{
	temp	= inputStack;
	s2		= mediaStack;
	s1		= operationStack;
	while (*temp != '\0')
	{
		if (*temp >= 'A'&&*temp <= 'Z')
			*s2++ = *temp;
		else if (*s1 == '\0')
			*s1 = *temp;
		else if (*temp == '(')
			*++s1 = *temp;
		else if (*temp == ')')
		{
			while (*s1 != '(')
			{
				*s2++ = *s1;
				*s1-- = '\0';
			}
			if (s1 == operationStack)*s1 == '\0'; else *s1-- = '\0';
		}
		else JUMP:if (priority(*temp)>priority(*s1))
			if (s1 == operationStack && *s1 == '\0')*s1 = *temp; else *++s1 = *temp;
		else
		{
			if (s1 == operationStack)
			{
				*s2++ = *s1;
				*s1 = '\0';
			}
			else
			{
				*s2++ = *s1;
				*s1-- = '\0';
			}
			goto JUMP;
		}
		temp++;
	}
	while (s1 + 1 != operationStack)
	{
		*s2++ = *s1;
		*s1-- = '\0';
	}
	*s2 = '\0';
	s1 = NULL;
	s2 = NULL;
}

void getOperate()
{
	temp = mediaStack;
	s1 = operate;
	while (*temp)
	{
		if (*temp >= 'A'&&*temp <= 'Z')
		{
			for (i = 0; *(operate + i) != '\0'; i++)
				if (*(operate + i) == *temp)
					goto NEXT;
			*s1++ = *temp;
		}
	NEXT:temp++;
	}
	*s1 = '\0';
	s1 = NULL;
	temp = NULL;
}


void calcuate()
{
	resultStack = (char *)malloc(strlen(mediaStack)*sizeof(char)+1);
	memset(resultStack,0,strlen(mediaStack)*sizeof(char)+1);
	temp = (char *)malloc(strlen(mediaStack) * sizeof(char) + 1);
	memset(temp, 0, strlen(mediaStack) * sizeof(char) + 1);

	int x = 0, y = 0;
	strcpy(temp, mediaStack);
	s1 = temp;

	for (x = 0; x < *(mediaStack + x) != '\0';x++)
	{
		for (y = 0; y < *(operate + y) != '\0';y++)
		{
			if (*s1 == *(operate + y))
				*s1 = *(compute + *(operate + y) - 'A');
		}
		s1++;
	}

	s1 = temp;
	s2 = resultStack;

	for (x = 0; x < *(mediaStack+x)!='\0'; x++,s1++)
	{
		if (*s1 == '0' || *s1 == '1')
		{
			*s2++ = *s1;
			*s2 = '\0';
		}
		else
		{
			if (*s1 == '~')
			{
				*(s2 - 1) = '0' + (*(s2 - 1) - '0' == 0 ? 1 : 0);
			}
			else if(*s1=='*')
			{
				*(s2 - 2) = '0' + (unsigned char)((*(s2 - 1) - '0')&(*(s2 - 2) - '0'));
				*--s2 = '\0';
			}
			else if (*s1 == '+')
			{
				*(s2 - 2) = '0' + (unsigned char)((*(s2 - 1) - '0')|(*(s2 - 2) - '0'));
				*--s2 = '\0';
			}
		}
	}
	printf("%s\n",resultStack);
	free(temp);
	free(resultStack);
	temp = NULL;
	resultStack = NULL;
}

void sort()
{
	char myChar = *operate;
	int minNum = 0;
	int x = 0, y = 0;
    int z=strlen(operate);
	for (x = 0;x<z; x++)
	{
        myChar=*(operate+x);
        minNum=x;
		for (y = x; y < z; y++)
		{
			if (*(operate + y) < myChar)
			{
				myChar = *(operate + y);
                minNum=y;
			}
		}
        *(operate+minNum)=*(operate+x);
        *(operate+x)=myChar;
	}
}
