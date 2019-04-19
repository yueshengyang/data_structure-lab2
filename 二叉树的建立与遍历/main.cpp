#include <iostream>
#include <stack>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  N  100

using namespace std;

//定义树节点
struct BiTNode
{
    BiTNode *lchild;
    BiTNode *rchild;
    char    data;
};
typedef BiTNode* BITree;

/*
 *前序中相邻AB在树中有三种位置关系：
 *1.根节点-左子树：...B(...)A...
 *2.根节点-右子树：...AB...
 *3.左子树-右子树：...A...B...
*/

//建立树 根据前序和中序 非递归
void createBiTree2(BITree &rT, string preStr, string inStr)
{
    BITree T=NULL;
    stack <BITree> sta;
    int index1=0,index2=0;
    int *preStrflag=NULL;
    preStrflag=new int(preStr.length());
    int i=0,j=0,k=0,m=0;
    preStr = preStr + '#';      //由于先序序列采用字符串形式，末尾加上#便于处理

    while (preStr[j] != '#')
    {   /*while循环将先序对应的数组全部标记为0*/
        preStrflag[j]=0;
        j++;
    }

    if (preStr.length() == 0 || inStr.length() == 0)
    {   /*处理特殊情况*/
        T = NULL;
        return;
    }

    else
    {  /*依次对先序序列中的每个字符进行存储，若一个*/
        T = new BiTNode;T->lchild=NULL;T->rchild=NULL;
        rT=T;
        T->data = preStr[i];
        preStrflag[i]=1;
        i++;
        sta.push(T);
        while (preStr[i] != '#')
        {
            preStrflag[i]=1;
            if((index1 = inStr.find(preStr[i]))==-1)
            {
                cout<<"数据错误！"<<endl;
                exit(0);
            }
            if((index2 = inStr.find(preStr[i-1]))==-1)
            {
                cout<<"数据错误！"<<endl;
                exit(0);
            }
            if (index1 < index2)
            {/*处理：如果在先序中为...AB...,中序中为..B..A..,即为根节点与左子树*/
                T->lchild = new BiTNode;
                T=T->lchild;T->lchild=NULL;T->rchild=NULL;
                T->data = preStr[i];
                sta.push(T);
            }
            else if (index1 == index2 + 1)
            {/*处理：如果在先序中为...AB...,中序中为..AB..,即根节点与右子树，且无左子树*/
                T->rchild = new BiTNode;
                T=T->rchild;T->lchild=NULL;T->rchild=NULL;
                T->data = preStr[i];
                sta.push(T);
            }
            else
            {/*处理：如果在先序中为...AB...,中序中为..A..CB..*/
                k = inStr.find(preStr[i])-1;    //中序中 C 的位置
                m = preStr.find(inStr[k]);      //
                while (preStrflag[m] == 0)      //该字符尚未存入树，而是当前字符的左子树
                {/*在中序中依次往前遍历，查找第一个已存在树中的字符，该字符即为根节点*/
                    k--;
                    m = preStr.find(inStr[k]);
                }
                while (inStr[k] != T->data)     //从栈中找到C，并将当前节点更新为C
                {
                    T= sta.top();
                    sta.pop();
                }
                T->rchild = new BiTNode;
                T=T->rchild;T->lchild=NULL;T->rchild=NULL;
                T->data = preStr[i];
                sta.push(T);
            }
            i++;
        }
    }
}

//  栈与队列
typedef struct sstack {
    BiTNode *db[N];
    int top;
} sstack;
typedef struct QueueNode {
    BiTNode* data;
    struct QueueNode* next;
}QueueNode;

//  链队列的定义，包含队头和队尾指针
typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
}LinkQueue;
// 链队列的初始化
LinkQueue* InitQueue()
{
    LinkQueue* Que = (LinkQueue*)malloc(sizeof(LinkQueue));
    if (!Que)
    {
        printf("init Que error!\n");
        exit(0);
    }
    Que->front = (QueueNode*)malloc(sizeof(QueueNode));
    Que->front->next = NULL;
    Que->rear = Que->front;
    return Que;
}

// 链队列的销毁，注意先进去的是队列头，后进去的是队列尾
void DestoryQueue(LinkQueue* Que)
{
    while (Que->front)
    {
        Que->rear = Que->front->next;
        free(Que->front);
        Que->front = Que->rear;
    }
}
//入队
void EnQueue(LinkQueue* Que, BiTNode* node)
{
    QueueNode* queueNode = (QueueNode*)malloc(sizeof(QueueNode));
    queueNode->data = node;
    queueNode->next = NULL;
    Que->rear->next = queueNode;
    Que->rear = queueNode;
}
//出队
BiTNode* DeQueue(LinkQueue* Que)
{
    if (Que->front == Que->rear)//队列为空
        return NULL;
    QueueNode* p = Que->front->next;
    BiTNode* node = p->data;
    Que->front = p;
    return node;
}

void LayerOrderBiTree(struct BiTNode* root)
{
    int curLayerCount = 0;      //当前层中的节点数
    int nextLayerCount = 0;     //下一层中的节点数
    struct Queue* Que = InitQueue();
    EnQueue(Que, root);
    curLayerCount++;
    struct BiTNode* p;
    while ((p = DeQueue(Que)))
    {
        curLayerCount--;
        printf("%c ", p->data);
        if (p->lchild)
        {
            EnQueue(Que, p->lchild);
            nextLayerCount++;
        }
        if (p->rchild)
        {
            EnQueue(Que, p->rchild);
            nextLayerCount++;
        }
        if (curLayerCount == 0)//一层已经遍历完毕
        {
            curLayerCount = nextLayerCount;
            nextLayerCount = 0;
            printf("\n");
        }
    }
}
//非递归后序遍历二叉树
void postTraverse(BiTNode *t)
{
    BiTNode *p, *pre;
    p = t;
    pre = NULL;
    // 初始化栈
    sstack *s = (sstack *)malloc(sizeof(sstack));
    s->top = 0;

    while (p || s->top > 0) {
        if (p)
        {
            s->db[s->top ++] = p;
            p = p->lchild;
        }
        else
        {
            p = s->db[-- s->top];
            if (p->rchild != NULL && p->rchild != pre)
            { // p为相对根节点
                s->db[s->top ++] = p;
                p = p->rchild;
            }
            else
            {
                printf("%c ", p->data);
                pre = p;
                p = NULL;
            }
        }
    }
}
//非递归先序遍历
void feipreorderTraverse(BiTNode*root)
{
    BiTNode *p = root;
    // 初始化栈
    sstack *s = (sstack *)malloc(sizeof(sstack));
    s->top = 0;
    while (p || s->top > 0) {
        if (p)
        {
            printf("%c ", p->data);
            s->db[s->top ++] = p;
            p = p->lchild;
        }
        else
        {
            p = s->db[-- s->top];
            p = p->rchild;
        }
    }
}
//非递归中序遍历
void feiinorderTraverse(BiTNode *root)
{
    BiTNode *p = root;
    // 初始化栈
    sstack *s = (sstack *)malloc(sizeof(sstack));
    s->top = 0;
    while (p || s->top > 0) {
        if (p)
        {
            s->db[s->top ++] = p;
            p = p->lchild;
        }
        else
        {
            p = s->db[-- s->top];
            printf("%c ", p->data);
            p = p->rchild;
        }
    }
}
//递归先序遍历
void preOrder(BiTNode*root)
{
    if (root==NULL)
    {
        return;
    }
    printf("%c ",root->data);
    preOrder(root->lchild);
    preOrder(root->rchild);
}
void postorder(BiTNode*root)
{
    if (root==NULL)
    {
        return;
    }
    postorder(root->lchild);
    postorder(root->rchild);
    printf("%c ",root->data);
}
//中序遍历
void inOrder(BiTNode*root)
{
    if (root==NULL)
    {
        return;
    }
    inOrder(root->lchild);
    printf("%c ",root->data);
    inOrder(root->rchild);
}

/************************************************************************/
/* 算法
1、通过先序遍历找到根结点A，再通过A在中序遍历的位置找出左子树，右子树
2、在A的左子树中，找左子树的根结点（在先序中找），转步骤1
3、在A的右子树中，找右子树的根结点（在先序中找），转步骤1                                                                     */
/************************************************************************/

//根据先序遍历和中序遍历递归创建二叉树
BiTNode* createBiTree(char *pre, char *in, int n)
{
    int i = 0;
    int n1 = 0,n2 = 0;
    int m1 = 0,m2 = 0;
    BiTNode*node = NULL;
    char lpre[N],rpre[N];
    char lin[N],rin[N];
    int l=0,m;
    for (l=0;l<n;l++)
    {
        m=0;
        while(1)
        {
            if (pre[l]!=in[m])
                m++;
            if (pre[l]==in[m])
            {
                break;
            }
            if (m>=n)
            {
                printf("数据错误\n");
                exit(0);
            }
        }
    }

    if (n == 0)
    {
        return NULL;
    }
    node = (BiTNode*)malloc(sizeof(BiTNode));
    if (node==NULL)
    {
        return NULL;
    }
    memset(node,0,sizeof(BiTNode));//初始化
    //先序序列的第一个元素必为根结点
    node->data = pre[0];
    //根据根结点将中序序列分为左子树和右子数
    for (i = 0;i<n;i++)
    {
        if ((i<=n1)&&(in[i]!=pre[0]))
        {
            lin[n1++] = in[i];//找到根节点在中序中的位置从而确定左子树，并按中序顺序写入数组lin
        }
        else if(in[i]!=pre[0])
        {
            rin[n2++] = in[i];//确定右子树，并按中序顺序写入数组rin
        }
    }
    //根据树的先序序列的长度等于中序序列的长度
    //且先序遍历是先左子树再后子树,无论先序还是中序 左子树和右子树的长度都是固定的
    //主意 从i=1开始 因为先序遍历的第一个是根
    for (i = 1;i < n;i++)
    {
        if (i< (n1+1))//n1代表了左子树的长度
        {
            lpre[m1++] = pre[i];//按前序顺序将左子树的值写入数组lpre
        }
        else
        {
            rpre[m2++] = pre[i];//按前序顺序将右子树的值写入数组rpre
        }
    }
    node->lchild = createBiTree(lpre,lin,n1);//递归过程
    node->rchild = createBiTree(rpre,rin,n2);
    return node;
}

int main()
{
    char preNode[N];
    char inNode[N];
    int n;
    char ch='\0';
    BiTNode* root=NULL;

    int flag=1;     //标志树状态
    int opt=-1;     //选择序号
    while(1)
    {
        putchar('\n');
        printf("*****************菜单栏*******************\n");
        if(flag==1)
        {
        printf("    1.根据先序和中序序列递归建立树\n");
        printf("    2.根据先序和中序序列非递归建立树\n");
        printf("    0.退出\n");
        }
        if(flag==2)
        {
        printf("    3.先序遍历（递归）\n");
        printf("    4.中序遍历（递归）\n");
        printf("    5.后序遍历（递归）\n");
        printf("    6.先序遍历（非递归）\n");
        printf("    7.中序遍历（非递归）\n");
        printf("    8.后序遍历（非递归）\n");
        printf("    9.层序遍历\n");
        printf("    10.置空树\n");
        printf("    0.退出\n");
        }
        printf("******************************************\n");

        printf("请输入序号：");
        scanf("%d",&opt);

        if(flag==1)
        {
            if( opt==1 || opt==2 )
            {
                fflush(stdin);
                n = 0;
                printf("请输入先序序列：\n");
                while((ch = getchar())&&ch!='\n')
                    preNode[n++] = ch;
                printf("请输入中序序列：\n");
                n = 0;
                while((ch = getchar())&&ch!='\n')
                    inNode[n++] = ch;
                preNode[n]='\0';
                inNode[n]='\0';
            }

            switch(opt)
            {
            case 1:
                root=createBiTree(preNode,inNode,n);
                flag=2;
                printf("\n建树成功\n");
                break;
            case 2:
                createBiTree2(root,preNode,inNode);
                flag=2;
                printf("\n建树成功\n");
                break;
            case 0:
                exit(0);
            default:
                printf("请输入合法序号！\n");
                break;
            }
        }
        else if(flag==2)
        {
            switch(opt)
            {
            case 3:
                preOrder(root);
                putchar('\n');
                break;
            case 4:
                inOrder(root);
                putchar('\n');
                break;
            case 5:
                postorder(root);
                putchar('\n');
                break;
            case 6:
                feipreorderTraverse(root);
                putchar('\n');
                break;
            case 7:
                feiinorderTraverse(root);
                putchar('\n');
                break;
            case 8:
                postTraverse(root);
                putchar('\n');
                break;
            case 9:
                LayerOrderBiTree(root);
                putchar('\n');
                break;
            case 10:
                root=NULL;
                flag=1;
                cout<<"已置空!"<<endl;
                break;
            case 0:
                exit(0);
            default:
                printf("请输入合法序号！\n");
                putchar('\n');
                break;
            }
        }
    }

    return 0;
}

