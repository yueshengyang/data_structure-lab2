#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#define N 10000

using namespace std;

struct node//定义哈夫曼树的节点
{
    int weight;
    int parent;
    int lchild;
    int rchild;
    char c;
};

struct HuffmanCode//定义用于存储哈夫曼编码的结构体
{
    char ch;
    string bits;
};

void compress(string passage,string &passage1)//排序，passage里的字符按字典序排序
{
    passage1=passage;
    int len=passage1.length(),minx;
    for (int i=0;i<len;i++)
    {
        minx=i;
        for (int j=i+1;j<len;j++)   //冒泡排序
        {
            if (passage1[minx]>passage1[j]) minx=j;
        }
        swap(passage1[i],passage1[minx]);
    }
}

int WeightingSort(node HuffmanT[N],string PassageSort)//将PassageSort字符串存入到HuffManT,并增加weight权重；；分成单个节点，设置每个节点的权重
{
    int len=PassageSort.length();
    int cnt=0;
    HuffmanT[cnt].c=PassageSort[0];
    HuffmanT[cnt].weight++;
    for (int i=1;i<len;i++)
    {
        if (PassageSort[i]==PassageSort[i-1])
            HuffmanT[cnt].weight++;
        else
        {
            cnt++;
            HuffmanT[cnt].c=PassageSort[i];
            HuffmanT[cnt].weight++;
        }
    }
    return cnt+1;//return HuffmanT的个数
}

void initial(node HuffmanT[N])//初始化
{
    for (int i=0;i<N;i++)
    {
        HuffmanT[i].weight=0;
        HuffmanT[i].lchild=-1;
        HuffmanT[i].rchild=-1;
        HuffmanT[i].parent=-1;
    }
}

void selectMin(node HuffmanT[N],int n,int &p1,int &p2)//选出权重最小的两个树节点的编号作为p1,p2
{
    int cnt;
    p1=-1;
    p2=-1;
    for (int i=0;i<n;i++)
    {
        cnt=i;
        if (HuffmanT[i].parent==-1)
        {
            p1=cnt;
            break;
        }
    }
    for (int i=cnt+1;i<n;i++)
    {
        if (HuffmanT[i].parent==-1&&HuffmanT[i].weight<HuffmanT[p1].weight)
        {
            p1=i;
        }
    }
    HuffmanT[p1].parent=-2;
    for (int i=0;i<n;i++)
    {
        cnt=i;
        if (HuffmanT[i].parent==-1)
        {
            p2=cnt;
            break;
        }
    }
    for (int i=cnt+1;i<n;i++)
    {
        if (HuffmanT[i].parent==-1&&HuffmanT[i].weight<HuffmanT[p2].weight)
        {
            p2=i;
        }
    }
}

void creatHT(node HuffmanT[N],int n)//建立哈夫曼树
{
    for (int i=n;i<2*n-1;i++)//需要n-1次合并
    {
        int p1,p2;
        selectMin(HuffmanT,i,p1,p2);
        HuffmanT[p1].parent=i;
        HuffmanT[p2].parent=i;
        HuffmanT[i].lchild=p1;
        HuffmanT[i].rchild=p2;
        HuffmanT[i].weight=HuffmanT[p1].weight+HuffmanT[p2].weight;
        HuffmanT[i].parent=-1;
    }
}

void Encode(node HuffmanT[N],int n,HuffmanCode Hcode[N])//从叶节点开始对哈夫曼树进行编码
{
    for (int i=0;i<n;i++)
    {
        Hcode[i].ch=HuffmanT[i].c;//将哈夫曼树节点遍历，将节点存的自复制存入编码结构体中
        int m=i;
        while (HuffmanT[m].parent!=-1)//从叶节点向上遍历，如果是左节点则增加‘0’，如果是右节点则增加‘1’
        {
            int p=HuffmanT[m].parent;
            if (HuffmanT[p].lchild==m) Hcode[i].bits+='0';
            else Hcode[i].bits+='1';
            m=p;
        }
        reverse(Hcode[i].bits.begin(),Hcode[i].bits.end());//将得到的编码字符串转置
    }
}

string Decode(node HuffmanT[N],string secret,int n)//解码函数，返回解码的字符串
{
    int pos=n*2-2;//根节点
    int len=secret.length();
    string DecodePassage;
    for (int i=0;i<len;i++)
    {
        if (secret[i]=='0')//如果为0,往左走
            pos=HuffmanT[pos].lchild;
        else//如果为1,往右走
            pos=HuffmanT[pos].rchild;
        if (HuffmanT[pos].lchild==-1)//到叶节点的话
        {
            DecodePassage+=HuffmanT[pos].c;
            pos=2*n-2;
        }
    }
    return DecodePassage;
}

void menu()//菜单函数
{
   cout<<"1.手动输入英文文章"<<endl;
   cout<<"2.从文件读英文文章"<<endl;
   cout<<"3.输出已输入的文章"<<endl;
   cout<<"4.对英文文章进行编码"<<endl;
   cout<<"5.对编码进行译码"<<endl;
  // cout<<"6.将译码存入文件"<<endl;
  // cout<<"7.从文件读入汇编代码"<<endl;
   cout<<"0.退出"<<endl;
}

int main()
{
    node HuffmanT[N];
    HuffmanCode Hcode[N];
    string passage,PassageSort,secret,DecodePassage,passage1;
    char c;
    int n,len;
    int choice;
    while (1)
    {
        menu();
        cin>>choice;
        if (choice==0) break;
        switch (choice)
        {
            case 1:
                cout<<"请输入英语文章,以^号结束"<<endl;
                do
                {
                    c=cin.get();
                    if (c=='^')
                        break;
                    passage+=c;
                }while(1);
                break;
            case 2:
                {ifstream file;
                file.open("passage.txt");
                while (!file.eof())
                {
                    c=file.get();
                    passage+=c;
                }
                file.close();
                cout<<"文件读取文章成功"<<endl;
                break;}
            case 3:
                cout<<passage<<endl;
                break;
            case 4:
                compress(passage,passage1);//把字母进行字典升序排列
                initial(HuffmanT);
                n=WeightingSort(HuffmanT,passage1);//给每个节点的字母设置权重
                creatHT(HuffmanT,n);//建立哈夫曼树
                Encode(HuffmanT,n,Hcode);//译码
                for(int i=0;i<n;i++)
                {
                    cout<<Hcode[i].ch<<endl;
                    cout<<Hcode[i].bits<<endl;
                }
                len=passage.length();
                for (int i=0;i<len;i++)
                {
                    for (int j=0;j<n;j++)
                    {
                        if (passage[i]==Hcode[j].ch)
                        {
                            secret+=Hcode[j].bits;
                            break;
                        }
                    }
                }
                cout<<"对文章编码如下"<<endl;
                cout<<secret<<endl;
                cout<<secret.length()<<endl;
                break;
            case 5:
                cout<<"对编码解码如下"<<endl;
                DecodePassage=Decode(HuffmanT,secret,n);
                cout<<DecodePassage<<endl;
                break;
            case 6:
                {
                ofstream fp;
                fp.open("decode.txt");
                /*for(int i=0;i<n;i++){
                fp<<HuffmanT[i].c;
                fp<<HuffmanT[i].lchild;
                fp<<HuffmanT[i].parent;
                fp<<HuffmanT[i].rchild;
                fp<<HuffmanT[i].weight;}*/
                fp<<n;
                fp<<secret;
                fp.close();
                cout<<"编码写入成功"<<endl;
                break;
                }
            case 7:
                {
                    //node temp;
                    ifstream file;
                    file.open("decode.txt");
                    file>>n;
                    /*for (int i=0;i<n;i++)
                    {file>>temp.c;
                    file>>temp.lchild;
                    file>>temp.parent;
                    file>>temp.rchild;
                    file>>temp.weight;
                    HuffmanT[i]=temp;
                    }*/
                    file>>secret;
                    file.close();
                    cout<<"编码读入成功"<<endl;
                    break;
                }

        }
    }
    return 0;
}
