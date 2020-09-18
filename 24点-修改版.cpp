#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
typedef struct Node
{
	struct Node* left;//������
	struct Node* right;//������
	char ch[3];//���������������
}Node;
void inorder(Node* root,char **s)//�������
{
	if (root->left)//��������
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
void free_node(Node * root, Node **atom)//�ݹ��ͷŶ�̬�ڴ棬���ͷ����������ͷŸ��ڵ�
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
void judge(double* nums, int length, Node **node, Node **atom,int *count, char (*result)[100])//����
{
	if (length == 1)//��ֻ��һ����ʱ���ж��Ƿ����24
	{
		if (fabs(*nums - 24.0) < 0.0001)//�������ıȽϲ���ֱ���õ���
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
			for (int j = i + 1; j != length; j++)//��ٽ���������������λ�õ��������
			{

				if (i == j)
					continue;
				double t[4];//�洢������
				Node * _t[4];//�洢���
				for (int k = 0; k != length; k++)//�����ʼ���ݣ����ڻָ�ԭ��
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
					double op[4] = { op1 + op2,op1 - op2,op1 * op2 };//�Ӽ���
					if (op2)//������Ϊ0���������
						op[n++] = op1 / op2;
					for (int i = 0; i != n; i++)//ͨ����������ټӼ��˳���Ĳ�ͬ���
					{
						Node* p = (Node*)malloc(sizeof(Node));
						switch (i)
						{
						case 0://��
							strcpy(p->ch, "+");
							break;
						case 1://��
							strcpy(p->ch, "-");
							break;
						case 2://��
							strcpy(p->ch, "*");
							break;
						case 3://��
							strcpy(p->ch, "/");
							break;
						}
						p->left = _op1;//�洢������
						p->right = _op2;
						node[length - 2] = p;
						nums[length - 2] = op[i];
						judge(nums, length - 1, node, atom, count, result);//�жϽ��
					}
					op1 = t[j];
					op2 = t[i];
					_op1 = _t[j];
					_op2 = _t[i];
				}
				for (int k = 0; k != length; k++)//�ָ�ԭ��
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
	printf("==================^-^^-^^-^^-^      ��ӭ����24����Ϸ      ^^-^^-^^-^^-^==================\n");
	printf("=========================================================================================\n");
	printf("=========================================================================================\n");
	printf("����\n    ���˿���������ȡ�������ƣ��������ϵ��ĸ�����ʹ������ļӼ��˳�\n��ÿ���������ϵ����ֶ�ֻ��ʹ��һ�Σ����õ�24��Ϊʤ�������У�J����11��\nQ����12��K����13��A����1)\n");
	printf("=========================================================================================\n");
	printf("��������Ϸѡ�\n\n");
	printf("  1.�ֶ������ĸ���   \t   2.������ɼ�����   \t   3.�˳���Ϸ\n\n");
	printf("������ѡ�  ");
	while (1)
	{
		scanf("%d", &choice);
		if (choice == 1 || choice == 2)
		{
			double cards[4];
			if (choice == 1)
			{
				printf("������4��0~13������\n");
				for (int i = 0; i != 4; i++)//ͨ�����̴���4��������
					scanf("%lf", cards + i);
			}
			else
			{
				srand(time(NULL));
				for (int i = 0; i != 4; i++)
					cards[i] = (double)(rand() % 13 + 1);//�������������
			}
			for (int i = 0; i != 4; i++)
			{
				node[i] = (Node*)malloc(sizeof(Node));//���붯̬�ڴ�
				node[i]->left = NULL;
				node[i]->right = NULL;
				if (cards[i] < 10.0)//���������ַ��������ʽ�洢����
				{
					node[i]->ch[0] = (int)cards[i] + '0';//���������ַ�ASCII��������ԭ�������ֱ�Ϊ�ַ�
					node[i]->ch[1] = '\0';//�ַ����ս��
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
			judge(cards, 4, node, node, &count,result);//���ú���
			if (!result)
				printf("�޽�\n");
			else
			{
				printf("��%d�ֽ�\n", count);
				for (int i = 0; i != count; i++)
				{
					printf("%s\n", result[i]);
				}
			}
			printf("\n���ٴ�������Ϸѡ��\n");
		}
		else
		{
			printf("\n�����˳���Ϸ��лл\n");
			return 0;
		}
	}
	
}