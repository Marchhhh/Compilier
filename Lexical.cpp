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

typedef vector <string> StrVector;  //�����������������ʷ��� 
typedef vector <char> ChVector;
 map <string,char> myMap;  //�ؼ���ӳ��Ϊ��ĸ��ʾ����ʶ���������������ʾ�� 


int uu=0,vv=0,ww=0,xx=0,yy=0,zz=0;
string KWord[20];  //��ŵ��ʷ��ŵ����飻 
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
const char Monocular_Operator[20]={'+','-','*','/','!','%','~','&','|','^','='};   //��Ŀ�����
const char Binocular_Operator[20][5]={"++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/="}; //˫Ŀ�����
const char Delimiter[20]={',','(',')','{','}',';','<','>','#'}; //���
const char Map[30]={'b','c','a','o','d','n','z','e','y','f','i','h','p','l','m','r','s','t','v','u','w','k'};

FILE* file_source=NULL;
ifstream infile;
ofstream outfile;

char M_Pair(char str[])        //ӳ�䵽һ���ַ�����ʾ
{
	for(map<string,char>::const_iterator it=myMap.begin();it!=myMap.end();it++)
	{
		if(strcmp(str,((*it).first).data())==0)//data����������stringֵתΪchar*ֵ���Ҳ����ַ���β����'\0'; 
		return (*it).second;
	}
}

char state[100];  //������Ų���ʽ��߷��ţ����ս����
int len_state;
char start;
char final[100];  //��������ս��
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
	{                         //����ӳ�䣻 
	 myMap[keyword[t]]=Map[t];
    }
	len_final=0;
	len_state=0;
	for(int i=0;i<100;i++)
	{
		for(int j=0;j<100;j++)
			for(int k=0;k<100;k++){
				move[i][j].set[k]='#';  //move[i][j]��ʾi״̬������j�ַ��󣬿��Ե���k״̬�����Ե����k״̬��len�� 
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
	for(int j=0;j<num_of_keyword;++j) //strcmp���� 
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
int Is_In_state(char temp)//���ս�� 
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
	char str[10];    //������ �ķ� �ұߵ�
	infile.open("D://RegularGrammer.txt");
	infile>>S;  //S���ĵ���ͷ�����֣�ָ������ʽ��������
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
		if(strlen(str)>1) //�жϿ�Ҳ�� 
		{
			move[ch][str[0]].set[move[ch][str[0]].len++]=str[1];//A->aB,A��B֮�����ϣ����ϱ��Ϊa���Ҿ���len��ʾ��ֵ���ܹ����Ļ�����
		}
		else
		{
			move[ch][str[0]].set[move[ch][str[0]].len++]='Y';  //Ĭ����̬��ΪY��
		}
	}
}
int Is_in(NFA_set temp)   //�����е�newset��û���ظ��ģ��оͷ����ظ��ı��
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
				if(temp.set[k]==new_set[i].set[j])//��ʾ��i������new_set����ĵ�j��Ԫ��set��
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

int Is_contained_Y(NFA_set temp)   //�ж��Ƿ�����̬
{
	int i;
	for(i=0;i<temp.len;++i)//����һ���������������û����̬����
	{
		if(temp.set[i]=='Y')
			return 1;
	}
	return 0;
}

void get_closure(NFA_set &temp)  
{
	for(int i=0;i<temp.len;++i)  //��temp�����ÿ��Ԫ���ƶ�������@����Ȼ���¼
	{
		for(int j=0;j<move[temp.set[i]]['@'].len;++j)
		{
			if(!Is_in_set(move[temp.set[i]]['@'].set[j],temp))//�ַ�set[j]û�г��ֹ������temp�ıհ������� 
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
	stack<NFA_set> s;//ʵ�ֺ����ȳ�����push��x����Ԫ�ؼ���β����top��������β��Ԫ�أ�pop������β��Ԫ��ɾ����
	get_closure(work_set);//�ȴӿ�ʼ���������ıհ���
	s.push(work_set);//��һ���ṹ��ѹ���ջ�У�
	new_set[num_new_set++]=work_set;//�ȸ�ֵ����������
	for(int i=0;i<150;++i)
	{
		for(int j=0;j<150;++j)
		{
			dfa[i][j]='-1';
		}
	}
	for(int i=0;i<150;++i)
		is_final[i]=false;//��ʼ���Ӽ����õ��ļ���ȫ�Ƿ���̬ 
	if(Is_contained_Y(work_set))
		is_final[num_new_set-1]=true;//���жϿ�ʼ���ŵıհ�������û�к�����̬Y,������еĻ������԰�is_final��0����Ϊtrue��
	while(!s.empty())
	{
		work_set=s.top();//top����β��Ԫ��
		s.pop();//ɾ��β��Ԫ�أ�
		for(int i=0;i<len_final;++i)//��move���� 
		{
			for(int j=0;j<work_set.len;++j)//������Ԫ�ط���β������Ҫ��һ��հ���
			{
				for(int k=0;k<move[work_set.set[j]][final[i]].len;++k)
				{
					if(move[work_set.set[j]][final[i]].set[k]!='#' && move[work_set.set[j]][final[i]].set[k]!='Y' && !Is_in_set(move[work_set.set[j]][final[i]].set[k],worked_set))
					{
						worked_set.set[worked_set.len++]=move[work_set.set[j]][final[i]].set[k];
					}
					if(move[work_set.set[j]][final[i]].set[k]=='Y' && !Is_in_set(move[work_set.set[j]][final[i]].set[k],worked_set))
					{
						worked_set.set[worked_set.len++]='Y';    //��Y��ʾ��̬
					}
				}
			}
			get_closure(worked_set);
			if(worked_set.len>0 && Is_in(worked_set)==-1)
			{
				dfa[num_new_set-1][final[i]]=num_new_set;//�൱�ڸ������ɵ��Ӽ����
				s.push(worked_set);
				new_set[num_new_set++]=worked_set;
				if(Is_contained_Y(worked_set))
				{
					is_final[num_new_set-1]=true;
				}
			}
			if(worked_set.len>0 && Is_in(worked_set)>-1 && final[i]!='@')
			{				
				dfa[Is_in(work_set)][final[i]]=Is_in(worked_set);//���������ɵ��Ӽ���ԭ�����ɵ��Ӽ��ظ�����������γ���ѭ����
			}
			worked_set.len=0;
		}
	}
	/*for(int i=0;i<150;++i)��ʾDFA 
	{
		for(int j=0;j<150;++j)
		{
			if(dfa[i][j]!='-1')
				cout<<i<<" "<<char(j)<<":"<<dfa[i][j]<<endl;
		}
	}*/
}

int DFA(char str[])//����Ҫ��С��DFA 
{
	char now_state=0;
	for(int i=0;i<strlen(str);++i )  
	{
		now_state=dfa[now_state][str[i]];// dfa�����൱��һ��ת������
		if(now_state==-1)  //bbac����ַ�������󣬿��ܲ��ܱ�dfaʶ��
			return 0;
	}
	if(is_final[now_state]==true)  //���������һ��״̬��������̬���������һ��״̬����ʶ��ɹ����ñ�ʶ���ǣ�
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
		if(ch>='0'&&ch<='9')     //�жϸ��������Ƿ��Ǹ�����12345.������
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
				cout<<str<<" "<<"���ǳ���"<<endl;
			}
			point=0;
			flag=-1;
		}
		if(Is_Letter(ch))
		{
			flag=2;
			str[point++]=ch;
			ch=fgetc(file_source);
			while(Is_Letter(ch) || (ch>='0'&&ch<='9'))//���ֻ�����ĸ��������һ�������ԣ�
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
					cout<<str<<" "<<"���Ǳ�ʶ��"<<endl;
				}
			}
			point=0;
			flag=-1;
		}
	
		if(Is_Delimiter(ch)) //delimeter�����
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
	cout<<"�ؼ���"<<":"<<endl;
	show1(iv1);
	StrVector iv2(IDent,IDent+sizeof(IDent)/sizeof(string));
	cout<<"��ʶ��"<<":"<<endl;
	show1(iv2);
	StrVector iv3(Const,Const+sizeof(Const)/sizeof(string));
	cout<<"����"<<":"<<endl;
	show1(iv3);
	StrVector iv4(Opera1,Opera1+sizeof(Opera1)/sizeof(string));
	cout<<"�����"<<":"<<endl;
	show1(iv4);
	ChVector iv5(Opera2,Opera2+sizeof(Opera2)/sizeof(char));
	show2(iv5);
	ChVector iv6(Delim,Delim+sizeof(Delim)/sizeof(char));
	cout<<"���"<<":"<<endl;
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


