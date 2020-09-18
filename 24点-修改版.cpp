#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
typedef struct Node
{
	struct Node* left;//左子树
	struct Node* right;//右子树
	char ch[3];//操作数或者运算符
}Node;
void inorder(Node* root,char **s)//中序遍历
{
	if (root->left)//生成括号
	{
		if (root->left->left || root->left->right)
			*s += sprintf(*s, "(");
		inorder(root->left,s);
		if (root->left->left || root->left->right)
			*s += sprintf(*s, ")");
	}
	*s += sprintf(*s, "%s", root->ch);
	if (root->right)
	{
		if (root->right->left || root->right->right)
			*s += sprintf(*s, "(");
		inorder(root->right,s);
		if (root->right->left || root->right->right)
			*s += sprintf(*s, ")");
	}
}
void free_node(Node * root, Node **atom)//递归释放动态内存，先释放子树，再释放根节点
{
	if (!root)
		return;
	for (int i = 0; i != 4; i++)
	{
		if (root == atom[i])
			return;
	}
	free_node(root->left, atom);
	free_node(root->right, atom);
	free(root);
}
void judge(double* nums, int length, Node **node, Node **atom,int *count, char (*result)[100])//计算
{
	if (length == 1)//当只有一个数时，判断是否等于24
	{
		if (fabs(*nums - 24.0) < 0.0001)//浮点数的比较不能直接用等于
		{
			
			char exp[100];
			char* t = exp;
			inorder(*node, &t);
			int flag = 1;
			for (int i = 0; i != *count; i++)
			{
				if(!strcmp(exp,result[i]))
				{
					flag = 0;
					break;
				}
			}
			if (flag)
			{
				strcpy(result[(*count)++], exp);
			}
		}
		free_node(*node, atom);
	}
	else
	{
		for (int i = 0; i != length - 1; i++)
		{
			for (int j = i + 1; j != length; j++)//穷举交换任意两个数组位置的所有组合
			{

				if (i == j)
					continue;
				double t[4];//存储操作数
				Node * _t[4];//存储结点
				for (int k = 0; k != length; k++)//保存初始数据，便于恢复原样
				{
					t[k] = nums[k];
					_t[k] = node[k];
				}
				double op1 = nums[i];
				double op2 = nums[j];
				Node* _op1 = node[i];
				Node* _op2 = node[j];
				for (int k = i + 1; k != j; k++)
				{
					node[k - 1] = node[k];
					nums[k - 1] = nums[k];
				}
				for (int k = j + 1; k != length; k++)
				{
					node[k - 2] = node[k];
					nums[k - 2] = nums[k];
				}
				int cnt = 2;
				while (cnt--)
				{
					int n = 3;
					double op[4] = { op1 + op2,op1 - op2,op1 * op2 };//加减乘
					if (op2)//除数不为0，可以相除
						op[n++] = op1 / op2;
					for (int i = 0; i != n; i++)//通过迭代，穷举加减乘除后的不同结果
					{
						Node* p = (Node*)malloc(sizeof(Node));
						switch (i)
						{
						case 0://加
							strcpy(p->ch, "+");
							break;
						case 1://减
							strcpy(p->ch, "-");
							break;
						case 2://乘
							strcpy(p->ch, "*");
							break;
						case 3://除
							strcpy(p->ch, "/");
							break;
						}
						p->left = _op1;//存储操作数
						p->right = _op2;
						node[length - 2] = p;
						nums[length - 2] = op[i];
						judge(nums, length - 1, node, atom, count, result);//判断结果
					}
					op1 = t[j];
					op2 = t[i];
					_op1 = _t[j];
					_op2 = _t[i];
				}
				for (int k = 0; k != length; k++)//恢复原样
				{
					nums[k] = t[k];
					node[k] = _t[k];
				}
			}
		}
	}
}
int main()
{
	int choice;
	Node* node[4];
	printf("=========================================================================================\n");
	printf("==================^-^^-^^-^^-^^-^^-^^-^^-^^-^^-^^-^^-^^-^^-^^============================\n");
	printf("==================^-^^-^^-^^-^      欢迎来到24点游戏      ^^-^^-^^-^^-^==================\n");
	printf("=========================================================================================\n");
	printf("=========================================================================================\n");
	printf("规则：\n    从扑克牌中任意取出四张牌，对牌面上的四个数字使用任意的加减乘除\n（每张牌牌面上的数字都只能使用一次），得到24者为胜。（其中，J代表11、\nQ代表12、K代表13、A代表1)\n");
	printf("=========================================================================================\n");
	printf("以下是游戏选项：\n\n");
	printf("  1.手动输入四个数   \t   2.随机生成几个数   \t   3.退出游戏\n\n");
	printf("请输入选项：  ");
	while (1)
	{
		scanf("%d", &choice);
		if (choice == 1 || choice == 2)
		{
			double cards[4];
			if (choice == 1)
			{
				printf("请输入4个0~13的整数\n");
				for (int i = 0; i != 4; i++)//通过键盘传入4个浮点数
					scanf("%lf", cards + i);
			}
			else
			{
				srand(time(NULL));
				for (int i = 0; i != 4; i++)
					cards[i] = (double)(rand() % 13 + 1);//利用随机数函数
			}
			for (int i = 0; i != 4; i++)
			{
				node[i] = (Node*)malloc(sizeof(Node));//申请动态内存
				node[i]->left = NULL;
				node[i]->right = NULL;
				if (cards[i] < 10.0)//将数字以字符数组的形式存储起来
				{
					node[i]->ch[0] = (int)cards[i] + '0';//利用数字字符ASCII码连续的原理，将数字变为字符
					node[i]->ch[1] = '\0';//字符串终结符
				}
				else
				{
					node[i]->ch[0] = '1';
					node[i]->ch[1] = ((int)cards[i])%10 + '0';
					node[i]->ch[2] = '\0';
				}
			}
			char result[100][100];
			int count = 0;
			judge(cards, 4, node, node, &count,result);//调用函数
			if (!result)
				printf("无解\n");
			else
			{
				printf("有%d种解\n", count);
				for (int i = 0; i != count; i++)
				{
					printf("%s\n", result[i]);
				}
			}
			printf("\n请再次输入游戏选项\n");
		}
		else
		{
			printf("\n您已退出游戏，谢谢\n");
			return 0;
		}
	}
	
}