#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <stack>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

typedef vector <string> StrVector;  //用正向迭代器输出单词符号 
typedef vector <char> ChVector;
 map <string,char> myMap;  //关键字映射为字母表示；标识符则用整数编码表示； 


int uu=0,vv=0,ww=0,xx=0,yy=0,zz=0;
string KWord[20];  //存放单词符号的数组； 
string IDent[20];
string Const[20];
string Opera1[20];
char Opera2[20];
char Delim[30];

void show1(const StrVector &iv){
	for(StrVector::const_iterator it=iv.begin();it!=iv.end();it++)
	   cout<<*it<<" ";
	   cout<<endl;
	   cout<<endl; 
}
void show2(const ChVector &iv){
	for(ChVector::const_iterator it=iv.begin();it!=iv.end();it++)
	   cout<<*it<<" ";
	   cout<<endl; 
	   cout<<endl;
}


const int num_of_keyword=22;
const int num_of_MO=11;
const int num_of_BO=12;
const int num_of_D=9;

const char keyword[50][12]={"break","case","char","continue","do","default","double",
"else","float","for","if","int","include","long","main","return","switch","typedef","void","unsigned","while","iostream"};
const char Monocular_Operator[20]={'+','-','*','/','!','%','~','&','|','^','='};   //单目运算符
const char Binocular_Operator[20][5]={"++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/="}; //双目运算符
const char Delimiter[20]={',','(',')','{','}',';','<','>','#'}; //界符
const char Map[30]={'b','c','a','o','d','n','z','e','y','f','i','h','p','l','m','r','s','t','v','u','w','k'};

FILE* file_source=NULL;
ifstream infile;
ofstream outfile;

char M_Pair(char str[])        //映射到一个字符来表示
{
	for(map<string,char>::const_iterator it=myMap.begin();it!=myMap.end();it++)
	{
		if(strcmp(str,((*it).first).data())==0)//data（）函数将string值转为char*值，且不在字符串尾部加'\0'; 
		return (*it).second;
	}
}

char state[100];  //用来存放产生式左边符号（非终结符）
int len_state;
char start;
char final[100];  //用来存放终结符
int len_final;
bool is_final[150];
int len_str;

struct NFA_set
{
	char set[100];
	int len;
};

NFA_set move[100][100];

char N_state[100];
int N_len_state;
char N_start;
char N_final[100];
int N_len_final;
bool N_is_final[100];

NFA_set new_set[100];
int num_new_set=0;
int dfa[150][150];

void NFA_Init()
{ 
	for(int t=0;t<22;t++)
	{                         //构建映射； 
	 myMap[keyword[t]]=Map[t];
    }
	len_final=0;
	len_state=0;
	for(int i=0;i<100;i++)
	{
		for(int j=0;j<100;j++)
			for(int k=0;k<100;k++){
				move[i][j].set[k]='#';  //move[i][j]表示i状态下输入j字符后，可以到达k状态，可以到达的k状态有len种 
                move[i][j].len=0; 
			}
	}
} 

int Is_Letter(char temp)
{
	if(temp>='a' && temp<='z')
		return 1;
	if(temp>='A' && temp<='Z')
		return 1;
	return 0;
}
int Is_Keyword(char temp[])
{
	for(int j=0;j<num_of_keyword;++j) //strcmp函数 
	{
		if(strlen(keyword[j])==strlen(temp))
		{
			if(strcmp(keyword[j],temp)==0)
				return 1;
		}
	}
	return 0;
}
int Is_MO(char temp)
{
	for(int i=0;i<num_of_MO;++i)
	{
		if(Monocular_Operator[i]==temp)
			return 1;
	}
	return 0;
}
int Is_BO(char temp[])
{
	for(int i=0;i<num_of_BO;++i)
	{
		if(strcmp(Binocular_Operator[i],temp)==0)
			return 1;
	}
	return 0;
}
int Is_Delimiter(char temp)
{
	for(int i=0;i<num_of_D;++i)
	{
		if(Delimiter[i]==temp)
			return 1;
	}
	return 0;
}
int Is_In_state(char temp)//非终结符 
{
	for(int i=0;i<len_state;++i)
	{
		if(temp==state[i])
			return 1;
	}
	return 0;
}
int Is_In_final(char temp)
{
	for(int i=0;i<len_final;++i)
	{
		if(temp==final[i])
			return 1;
	}
	return 0;
}
int Is_in_set(char t,NFA_set temp)
{
	for(int i=0;i<temp.len;++i)
	{
		if(t==temp.set[i])
			return 1;
	}
	return 0;
}

void ProNFA()
{

	int S;
	bool flag=true;   
	char ch;    
	char nouse;
	char str[10];    //用来读 文法 右边的
	infile.open("D://RegularGrammer.txt");
	infile>>S;  //S是文档开头的数字，指明产生式的数量；
	while(S--)
	{
		infile>>ch>>nouse>>nouse>>str;
		if(flag)
		{
			start=ch;
			flag=false;
		}
		if(!Is_In_state(ch))
		{
			state[len_state++]=ch;
		}
		if(!Is_In_final(str[0]))
		{
			final[len_final++]=str[0];
		}
		if(strlen(str)>1) //判断空也行 
		{
			move[ch][str[0]].set[move[ch][str[0]].len++]=str[1];//A->aB,A到B之间连上，弧上标记为a；我觉得len表示的值是总共划的弧数，
		}
		else
		{
			move[ch][str[0]].set[move[ch][str[0]].len++]='Y';  //默认终态设为Y；
		}
	}
}
int Is_in(NFA_set temp)   //和已有的newset有没有重复的，有就返回重复的编号
{
	bool flag[100];
	bool flag1;
	for(int i=0;i<temp.len;++i)
	{
		flag[i]=false;
	}
	for(int i=0;i<num_new_set;++i)
	{
		for(int k=0;k<temp.len;++k)
		{
			for(int j=0;j<new_set[i].len;++j)
			{
				if(temp.set[k]==new_set[i].set[j])//表示第i个集合new_set里面的第j个元素set；
				{
					flag[k]=true;
				}
			}
		}
		flag1=true;
		for(int m=0;m<temp.len;++m)
		{
			if(flag[m]==false)
			{
				flag1=false;
				break;
			}
		}
		if(flag1==true)
			return i;
		for(int m=0;m<temp.len;++m)
		{
			flag[m]=false;
		}
	}
	return -1;
}

int Is_contained_Y(NFA_set temp)   //判断是否是终态
{
	int i;
	for(i=0;i<temp.len;++i)//遍历一下这个集合里面有没有终态符；
	{
		if(temp.set[i]=='Y')
			return 1;
	}
	return 0;
}

void get_closure(NFA_set &temp)  
{
	for(int i=0;i<temp.len;++i)  //让temp里面的每个元素移动若干条@弧，然后记录
	{
		for(int j=0;j<move[temp.set[i]]['@'].len;++j)
		{
			if(!Is_in_set(move[temp.set[i]]['@'].set[j],temp))//字符set[j]没有出现过则加入temp的闭包集合中 
			{
				temp.set[temp.len++]=move[temp.set[i]]['@'].set[j];
			}
		}
	}
}

void NFA_T_DFA()
{
	num_new_set=0;
	NFA_set work_set;
	NFA_set worked_set;
	work_set.set[work_set.len++]=start;
	worked_set.len=0;
	stack<NFA_set> s;//实现后入先出，用push（x）将元素加入尾部，top（）访问尾部元素，pop（）将尾部元素删除；
	get_closure(work_set);//先从开始符号求它的闭包；
	s.push(work_set);//把一个结构体压入堆栈中；
	new_set[num_new_set++]=work_set;//先赋值，后自增；
	for(int i=0;i<150;++i)
	{
		for(int j=0;j<150;++j)
		{
			dfa[i][j]='-1';
		}
	}
	for(int i=0;i<150;++i)
		is_final[i]=false;//初始化子集法得到的集合全是非终态 
	if(Is_contained_Y(work_set))
		is_final[num_new_set-1]=true;//即判断开始符号的闭包集合有没有含有终态Y,如果含有的话，可以把is_final（0）置为true；
	while(!s.empty())
	{
		work_set=s.top();//top访问尾部元素
		s.pop();//删除尾部元素；
		for(int i=0;i<len_final;++i)//求move集合 
		{
			for(int j=0;j<work_set.len;++j)//新增的元素放在尾部，仍要做一遍闭包；
			{
				for(int k=0;k<move[work_set.set[j]][final[i]].len;++k)
				{
					if(move[work_set.set[j]][final[i]].set[k]!='#' && move[work_set.set[j]][final[i]].set[k]!='Y' && !Is_in_set(move[work_set.set[j]][final[i]].set[k],worked_set))
					{
						worked_set.set[worked_set.len++]=move[work_set.set[j]][final[i]].set[k];
					}
					if(move[work_set.set[j]][final[i]].set[k]=='Y' && !Is_in_set(move[work_set.set[j]][final[i]].set[k],worked_set))
					{
						worked_set.set[worked_set.len++]='Y';    //用Y表示终态
					}
				}
			}
			get_closure(worked_set);
			if(worked_set.len>0 && Is_in(worked_set)==-1)
			{
				dfa[num_new_set-1][final[i]]=num_new_set;//相当于给新生成的子集编号
				s.push(worked_set);
				new_set[num_new_set++]=worked_set;
				if(Is_contained_Y(worked_set))
				{
					is_final[num_new_set-1]=true;
				}
			}
			if(worked_set.len>0 && Is_in(worked_set)>-1 && final[i]!='@')
			{				
				dfa[Is_in(work_set)][final[i]]=Is_in(worked_set);//这是新生成的子集与原来生成的子集重复的情况，则形成自循环；
			}
			worked_set.len=0;
		}
	}
	/*for(int i=0;i<150;++i)显示DFA 
	{
		for(int j=0;j<150;++j)
		{
			if(dfa[i][j]!='-1')
				cout<<i<<" "<<char(j)<<":"<<dfa[i][j]<<endl;
		}
	}*/
}

int DFA(char str[])//不需要最小化DFA 
{
	char now_state=0;
	for(int i=0;i<strlen(str);++i )  
	{
		now_state=dfa[now_state][str[i]];// dfa里面相当于一个转换矩阵；
		if(now_state==-1)  //bbac这个字符串输入后，看能不能被dfa识别；
			return 0;
	}
	if(is_final[now_state]==true)  //如果最后的那一个状态是属于终态数组里面的一个状态，则识别成功，该标识符是；
		return 1;
	return 0;
}
void ExeScan()
{
	char str[100]; 
	char ch;
	int i,j,point,flag;
	ch=fgetc(file_source);
	bool finish=false;
	while(!finish)
	{
	 	flag=-1;
		point=0;
		if(ch>='0'&&ch<='9')     //判断给定数组是否是个整数12345.。。；
		{
			flag=1;
			str[point++]=ch;
			ch=fgetc(file_source);
			while(Is_Letter(ch) ||(ch>='0'&&ch<='9')|| ch=='.' || ch=='+' || ch=='-')
			{
				flag=1;
				str[point++]=ch;
				ch=fgetc(file_source);
			}
			str[point]='\0';
		}
		if(flag==1)
		{
			if(DFA(str))
			{
				Const[ww++]=(string)str; 
				outfile<<3;
			}
			else
			{
				cout<<str<<" "<<"不是常量"<<endl;
			}
			point=0;
			flag=-1;
		}
		if(Is_Letter(ch))
		{
			flag=2;
			str[point++]=ch;
			ch=fgetc(file_source);
			while(Is_Letter(ch) || (ch>='0'&&ch<='9'))//数字或者字母其中任意一个均可以；
			{
				flag=2;
				str[point++]=ch;
				ch=fgetc(file_source);
			}
			str[point]='\0';
		}
		if(flag==2)
		{
			if(Is_Keyword(str))
			{
				KWord[uu++]=(string)str;
				outfile<<M_Pair(str);
			}
			else
			{
				if(DFA(str))
				{
					IDent[vv++]=(string)str;
					outfile<<2;
				}
				else
				{
					cout<<str<<" "<<"不是标识符"<<endl;
				}
			}
			point=0;
			flag=-1;
		}
	
		if(Is_Delimiter(ch)) //delimeter界符；
		{
            Delim[zz++]=ch;
			if(ch=='#')
				outfile<<'*'; 
			else
				outfile<<ch;
			if((ch=fgetc(file_source))==EOF)
			{
				finish=true;
				break;
			}
		}
		if(Is_MO(ch))
		{
			str[point++]=ch;
			if((ch=fgetc(file_source))==EOF)
			{
				finish=true;
			}
			str[point++]=ch;
			str[point]='\0';
			if(finish==false && Is_BO(str))
			{
			    Opera1[xx++]=(string)str;
				outfile<<4;
				ch=fgetc(file_source);
			}
			else
			{
				Opera2[yy++]=str[0];
				outfile<<str[0];
			}
			point=0;
		}

		if(ch==' ' || ch=='\n' || ch=='\t')
		{
			if((ch=fgetc(file_source))==EOF)
			{
				finish=true;
				break;
			}
			continue;
		}
	}
	outfile<<'#';
}

void show_word(){
	StrVector iv1(KWord,KWord+sizeof(KWord)/sizeof(string));
	cout<<"关键字"<<":"<<endl;
	show1(iv1);
	StrVector iv2(IDent,IDent+sizeof(IDent)/sizeof(string));
	cout<<"标识符"<<":"<<endl;
	show1(iv2);
	StrVector iv3(Const,Const+sizeof(Const)/sizeof(string));
	cout<<"常量"<<":"<<endl;
	show1(iv3);
	StrVector iv4(Opera1,Opera1+sizeof(Opera1)/sizeof(string));
	cout<<"运算符"<<":"<<endl;
	show1(iv4);
	ChVector iv5(Opera2,Opera2+sizeof(Opera2)/sizeof(char));
	show2(iv5);
	ChVector iv6(Delim,Delim+sizeof(Delim)/sizeof(char));
	cout<<"界符"<<":"<<endl;
	show2(iv6);	
}
int main(void)
{   
	NFA_Init();
	ProNFA();
	NFA_T_DFA();
	file_source=fopen("D://code.txt","r+");
	outfile.open("D://output.txt");
	ExeScan();
	show_word();
	fclose(file_source);
	outfile.close();
	return 0;
}


