#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

stack<char>gra_st;//输入串存储栈 
stack<char>ana_st;//分析栈 

char final[100];
char nonfinal[100];

FILE *file_1=NULL;
ifstream infile;
ofstream first_file;
ofstream follow_file;
ofstream select_file;
ofstream pre_tab_file;

int num_pro;  //产生式个数； 
int num_final;
int num_nonfinal; 
char pro[40][40];
int E_non[40];
char temp[40][40]; //暂时保存输入的产生式 
char ch;
char start;
char first[200][200];
char follow[200][200];
char select[200][200];
int  predict[200][200];

char Add_non(){  //从26个大写字母中产生一个未在非终结符表中出现的字母；
    char a='A';
    int flag6=1;
    for(int i=0;i<26;i++){
		for(int j=0;j<num_nonfinal;j++){
			if(a==nonfinal[j]){
			flag6=2;
			break;
		}
		}
		if(flag6==1) return a;
		a=char(a+1);
		flag6=1;
    } 
	
}
int Map(char a){   //非终结符与其在nonfinal集合中的序号对应 ； 
	for(int q=0;q<strlen(nonfinal);q++){
		if(nonfinal[q]==a&&a!=' ')
		return q;
	}
}

int Map_final(char a){ //终结符与其在final集合中对应的序号； 
	for(int q=0;q<strlen(final);q++){
		if(final[q]==a&&a!=' ')
		return q;
	}
	if(a=='#') return strlen(final);
}

int Map_pro(char a){ //终结符与最近的产生式对应 
	for(int i=0;i<num_pro;i++){
		if(a==temp[i][0])  return i;
	}
}
 
void Delete_Pro(char a){  //删除产生式 
	for(int j=0;j<num_pro;j++){
				if(pro[j][0]==a)
		   for(int l=0;l<strlen(pro[j]);l++)
				 pro[j][l]=' ';
}
}
bool is_final(char a){
	for(int i=0;i<num_final;i++){
		if(a==final[i])
		return true;
	}
	return false;
}

bool is_nonfinal(char a){
    for(int i=0;i<num_nonfinal;i++){
		if(a==nonfinal[i])
		return true; 
    }	
    return false;
} 
void delete_dup(char a[]){  //消除重复函数； 
   char tem[200];
   int m=0,flag;
   for(int i=0;i<strlen(a);i++){
		flag=1;
		for(int j=0;j<m;j++){
		if(a[i]==tem[j]||a[i]==' ') {
			flag=2;
			break;
		}
		}
		if(flag==1&&a[i]!='@'&&a[i]!=' ') tem[m++]=a[i];//过滤掉空格 
   }
    for(int k=0;k<m;k++){
		a[k]=tem[k];
    } 
    a[m]='\0';
} 

void _strcat(int i,int f,int j){ 
	char cc[20];
	for(int e=0;e<20;e++){
		cc[e]=follow[Map(temp[i][f])][e];}
		char dd[20];	
	for(int e=0;e<20;e++){
		dd[e]=follow[Map(temp[i][j])][e];	
		 }
	strcat(dd,cc); 
	for(int e=0;e<strlen(dd);e++){
	follow[Map(temp[i][j])][e]=dd[e];	
	}
	follow[Map(temp[i][j])][strlen(dd)]='\0';
}

bool pro_right_em(int i){//判断一个产生式右部能否推出空集；用于select集合求解 
     int flag3=1;
     int j=3;
     if(temp[i][j]=='@') return true;
     while(flag3==1){
			if(is_final(temp[i][j])) return false;
			if(temp[i][j]=='\0'||temp[i][j]==' ')
			return true;
			if(E_non[temp[i][j]]==0) return false;
			
			j++;
     }
}

bool compare(int m,int n){  //ll1文法判断select集合是否相交； 
	int i,j;
	for(i=0;i<strlen(select[i]);i++){
		for(j=0;j<strlen(select[j]);j++){
			if(select[m][i]==select[n][j]){
				return false;
			}
		}
	}
	return true;
} 

void Pro_Init(){
  num_final=0;
  num_nonfinal=0;
  num_pro=0;
  int i,j,k,l,m,n;
  int flag;
  if((file_1=fopen("D://production.txt","r"))==NULL){
		cout<<"指定目录下找不到文件！"<<endl;
		exit(0); 
  }
   ch=fgetc(file_1);
   start=ch;
   n=0;
   for(i=0;ch!=EOF;i++){
		num_pro++;
		nonfinal[i]=ch; 
	for(j=0;ch!='\n'&&ch!=EOF;j++){
		pro[i][j]=ch;
		ch=fgetc(file_1);
	}
		pro[i][j]='\0';  //'/0'为字符串结束符 
		ch=fgetc(file_1);	
   }
   for(k=0;k<num_pro;k++) //分析每一个产生式，得到终结符和非终结符； 
   {
	for(l=3;l<strlen(pro[k]);l++)	{
		  flag=0;
		if(pro[k][l]!='|'&&pro[k][l]!='@'){
			for(m=0;m<num_pro;m++){
				if(pro[k][l]==nonfinal[m])
				 flag=1;
			}
			if(flag==0) final[n++]=pro[k][l];
		}
	}	
	}
	delete_dup(final);
	delete_dup(nonfinal);
	/*cout<<"终结符有：";
	for(i=0;i<strlen(final);i++)
		cout<<final[i]<<" ";   //输出所有终结符
	cout<<endl<<"非终结符有：";
	for(i=0;i<strlen(nonfinal);i++)
		cout<<nonfinal[i]<<" ";  //输出所有非终结符
	cout<<endl;*/
	num_nonfinal=strlen(nonfinal);
	num_final=strlen(final);
 
}
void del_dre_lef(){//消除显示左递归和隐式左递归；显示左公共因子和隐式左公共因子 
    int i,j,k;     
  for(i=0;i<num_pro;i++){//消除直接左递归 A->Ac；A->d；=》A'->cA’，A‘->@,A->dA' 
    if(pro[i][0]==pro[i][3]){//这里产生式还是存放在pro数组里面
       char t=pro[i][0];
	   char s[2];
	   s[0]=Add_non();
	   s[1]='\0';
	   num_nonfinal++;
	   pro[i][0]=s[0];
	   for(k=3;k<strlen(pro[i])-1;k++){
			pro[i][k]=pro[i][k+1];
	   }
	   pro[i][k+1]=s[0];
	   pro[num_pro][0]=s[0];
	   pro[num_pro][3]='@'; 
	   num_pro++;
	   for(j=0;j<num_pro;j++){
			if(j!=i&&pro[j][0]==t&&pro[j][3]!=t){
				strcat(pro[j],s);
			}
	   }
    }
    }	 
}
void del_all_lef(){  //间接左递归的消除，先把间接左递归转为直接左递归，再调用直左函数 
  int p=0;
	for(int i=0;i<num_nonfinal;i++){
		for(int j=0;j<i-1;j++){ //
		for(int k=0;k<num_pro;k++){
			for(int l=0;l<num_pro;l++){
		if(pro[k][0]==nonfinal[i]&&pro[k][3]==pro[l][0]&&Map(pro[l][0]<Map(pro[k][0])))	
		{   
			char temp[20];
			for(int o=4;o<strlen(pro[k]);o++){
				temp[0]=pro[k][o];
			}
			temp[strlen(pro[k])-4]='\0';
			for(int m=3;m<strlen(pro[l]);m++){
				pro[k][m]=pro[l][m];
				p=m;
			}
			for(int n=4;n<strlen(pro[k]);n++){
				pro[k][p++]=temp[n-4];
			}
			}
	}
	}		
		}
		}

	del_dre_lef();
}
//$的非终结符集合
void E_final(){
	int X[strlen(nonfinal)];//X的123顺序与nonfinal里的非终结符的顺序一一对应；
	int i,j,k,l;
	int flag1=3;
	for(i=0;i<num_pro;i++){
		for(j=0;j<strlen(pro[i]);j++){
		temp[i][j]=pro[i][j];
	    }
	}
	for(i=0;i<strlen(nonfinal);i++){
		X[i]=-1;
	}

	for(i=0;i<num_pro;i++){
		for(k=3;k<strlen(pro[i]);k++){
			for(j=0;j<strlen(final);j++){
				if(pro[i][k]==final[j]) 
				{
				for(l=0;l<strlen(pro[i]);l++)
				  pro[i][l]=NULL;
			}
			}
		}
	}

	int flag=1;
	for(i=0;i<strlen(nonfinal);i++){
		for(j=0;j<num_pro;j++){
		  if(nonfinal[i]==pro[j][0])
		  flag=0;	
		}
		if(flag==1) X[i]=0;
		flag=1;
	}
	for(i=0;i<num_pro;i++){
		if(pro[i][3]=='@'){
		   X[Map(pro[i][0])]=1;
		Delete_Pro(pro[i][0]);
	}
	}
	
	for(i=0;i<num_pro;i++){
		for(j=3;j<strlen(pro[i]);j++){ 
			if(X[Map(pro[i][j])]==1&&pro[i][j]!=' '){
				pro[i][j]=' ';
				if(strlen(pro[i])==3){
					X[Map(pro[i][0])]=1;
		        Delete_Pro(pro[i][0]);
				}
				if(strlen(pro[i])!=3){
					break;
				}
			}

			if(X[Map(pro[i][j])]==0&&pro[i][j]!=' '){
				for(int ii=0;ii<num_pro;ii++){
					if(pro[ii][0]==pro[i][0]&&ii!=i)
					flag1=2;
				}
				if(flag1==3)  
				{
					X[Map(pro[i][0])]=0;
		        }
				for(k=0;k<strlen(pro[i]);k++){
					pro[i][k]=' ';
				} 
				flag1=3;
				break;
			}

		}
	} 
	for(i=0;i<strlen(nonfinal);i++){
		E_non[i]=X[i];
		//cout<<E_non[i]<<" ";
	}
} 
//非终结符的first集合 
void F_first(char a,int k,int n){//k为产生式序号，n为产生式右边符号序号，从3开始；
        if(k==num_pro) return;
		if(temp[k][3]=='@'){
			strcat(first[Map(a)],"@");//字符串拼接函数strcat  
			if(temp[k+1][0]==temp[k][0]&&k+1!=num_pro)   
			F_first(a,++k,n);  
		}
		if(is_final(temp[k][3])){
			char te[2];
			te[0]=temp[k][3];
			te[1]='\0';
			strcat(first[Map(a)],te);
			if(temp[k+1][0]==temp[k][0]&&k+1!=num_pro)
			F_first(a,++k,n);
		}
		if(is_nonfinal(temp[k][3])){  //递归函数怎么写；
			if(E_non[Map(temp[k][3])]==1){
				int flag=1;
				while(flag==1){
		            F_first(a,Map_pro(temp[k][n]),n);
					//F_first(a,Map_pro(temp[k][n+1]),n);
					if(is_final(temp[k][++n])||E_non[Map(temp[k][++n])]||temp[k][++n]=='\0'){
					F_first(a,Map_pro(temp[k][n]),n);
					flag=2;
				}
				}
			}
			if(E_non[Map(temp[k][3])]==0) {
          	F_first(a,Map_pro(temp[k][3]),n);
			}
        }
       
}
void F_first_all(){
	int i,j,k,n;
	for(i=0;i<num_pro;i++){
		n=3;
		F_first(temp[i][0],i,n);
	}
    for(i=0;i<num_nonfinal;i++)
    delete_dup(first[i]);
    for(i=0;i<num_nonfinal;i++){
		if(E_non[i]==1)
		strcat(first[i],"@");
    }
   for(i=0;i<num_nonfinal;i++){
		    first_file<<nonfinal[i]<<' ';
			for(j=0;j<strlen(first[i]);j++){
				first_file<<first[i][j];
			}
			first_file<<'\n';
	  }
}
void F_follow(){  //开始符号是P； 
     int i,j,k;
     	  strcat(follow[Map(start)],"#");
	 for(i=0;i<num_pro;i++) {
		for(j=3;j<strlen(temp[i]);j++){
			if(is_nonfinal(temp[i][j])){
				int f=j+1;
				if(temp[i][f]=='\0'||temp[i][f]=='\n'||temp[i][f]==' ') 
			   _strcat(i,0,j);
				if(is_final(temp[i][f])&&temp[i][f]!=' ') {
					char t[2];
					t[0]=temp[i][f];
					t[1]='\0';
				strcat(follow[Map(temp[i][j])],t);
				}
				if(is_nonfinal(temp[i][f])){  
					if(E_non[Map(temp[i][f])]==0){
				   	char cc[20];
							for(int e=0;e<20;e++){
							cc[e]=first[Map(temp[i][f])][e];}
					char dd[20];
					      for(int e=0;e<20;e++){
							dd[e]=follow[Map(temp[i][j])][e];	
					      }
					strcat(dd,cc);
					for(int e=0;e<20;e++){
							follow[Map(temp[i][j])][e]=dd[e];	
					      }
				    }
					if(E_non[Map(temp[i][f])]==1) 
					{   
						int flag2;
						flag2=1;
						int g=f;
						while(flag2==1){
						strcat(follow[Map(temp[i][j])],first[Map(temp[i][g])]);
							g=g+1;
						if(is_final(temp[i][g])&&temp[i][g]!=' ')	{
						char tt[2];
					    tt[0]=temp[i][g];
					    tt[1]='\0';
				        strcat(follow[Map(temp[i][j])],tt);
						flag2=2;
						}
						if(temp[i][g]=='\0'||temp[i][g]=='\n'||temp[i][g]==' ') {
						 _strcat(i,0,j);
						flag2=2;
					    }
					    if(is_nonfinal(temp[i][g])&&E_non[Map(temp[i][g])]==0){
							strcat(follow[Map(temp[i][j])],first[Map(temp[i][g])]);
							flag2=2;
					    }
					
						}
					}
				
				}
			}
		}
	}
   for(i=0;i<num_nonfinal;i++)
      delete_dup(follow[i]);
   for(i=0;i<num_nonfinal;i++){
		follow_file<<nonfinal[i]<<' ';
			for(j=0;j<strlen(follow[i]);j++){
				follow_file<<follow[i][j];
			}
			follow_file<<'\n';
	  }
}
void F_select(){// 各产生式的select集合
    int i,j,k;
    for(i=0;i<num_pro;i++){
		if(pro_right_em(i)){
		int flag4=1;    
		int ss=3;	
		if(temp[i][ss]=='@'){
			flag4=2;
		} 
		while(flag4==1){
			strcat(select[i],first[Map(temp[i][ss])]);
			ss++;
			if(temp[i][ss]=='\0'||temp[i][ss]==' ')
			flag4=2;
			}
			strcat(select[i],follow[Map(temp[i][0])]);
    }
    if(pro_right_em(i)==0) {
		int flag5=1;
		int ee=3;
		while(flag5==1){
			if(temp[i][ee]==' '||temp[i][ee]=='\0') flag5=2;
			if(is_final(temp[i][ee])) {
				char kk[2];
				kk[0]=temp[i][3];
				kk[1]='\0';
				strcat(select[i],kk);
				flag5=2;
			}
			if(is_nonfinal(temp[i][ee])){
			if(E_non[Map(temp[i][ee])]==0){
				strcat(select[i],first[Map(temp[i][ee])]);
				flag5=2;
			}
			if(E_non[Map(temp[i][ee])]==1){
			strcat(select[i],first[Map(temp[i][ee])]);	
			}
		}
		ee++;
		}
    }
} 
   for(i=0;i<num_pro;i++)   
	  delete_dup(select[i]);      
   for(i=0;i<num_pro;i++){
		select_file<<i<<' ';
			for(j=0;j<strlen(select[i]);j++){
				select_file<<select[i][j];
			}
			select_file<<'\n';
	  }    
   
}
 void is_LL(){
		int i,j;
		for(i=0;i<num_pro;i++){
			for(j=i+1;j<num_pro;j++){
				if(temp[i][0]==temp[j][0]){
					if(!compare(i,j)){
						cout<<"该文法不是LL（1）文法！"<<endl;
						exit(0); 
					}
				}
			}
		}
 }	
void pre_table(){//predict[i][j]=m;其中i为非终结符序号，nonfinal[i];j为终结符序号；m为产生式序号，temp[m];
	 int i,j;
	 for(i=0;i<200;i++){
			for(j=0;j<200;j++){
				predict[i][j]=99;
			}
	 }
	 for(i=0;i<num_pro;i++){
			for(j=0;j<strlen(select[i]);j++){
				int t1=Map(temp[i][0]);
				int t2=Map_final(select[i][j]);
				predict[t1][t2]=i;
				
			}
	 }  
	  for(i=0;i<num_nonfinal;i++){
			for(j=0;j<num_final;j++){
		       pre_tab_file<<predict[i][j]<<" ";
	        }
	        pre_tab_file<<'\n';
    }
} 
void F_analysis(){
	int i,j,k;
	char grammer[100]; 
	int num_gra=0;
	infile.open("D://output.txt");
	if(!infile){
		cout<<"不能打开output.txt文件！"<<endl;
		exit(0);
	}
	while(!infile.eof()){
		infile>>grammer[num_gra];
		num_gra++;
	}
	for(i=num_gra-1;i>=0;i--) {
		gra_st.push(grammer[i]);
	}
	ana_st.push('#');
	ana_st.push(start); 
	while(ana_st.top()!='#'){
		if(ana_st.top()==gra_st.top()){
			ana_st.pop();
			gra_st.pop();
		}
		if(is_nonfinal(ana_st.top())){
		int pro=predict[Map(ana_st.top())][Map_final(gra_st.top())];
		if(pro!=99){
			int now=strlen(temp[pro]);
			ana_st.pop();
			for(j=now-1;j>=3;j--){
				if(temp[pro][j]!='@'){
				ana_st.push(temp[pro][j]);
			}
			}
		}
	   } 
	}
	if(ana_st.top()=='#'&&gra_st.top()=='#'){
		cout<<"YES"<<endl;
		return;
	}
	cout<<"NO"<<endl;
}
int main(){
	Pro_Init();
	del_all_lef(); 
	first_file.open("D://first.txt");
	follow_file.open("D://follow.txt");
	select_file.open("D://select.txt"); 
	pre_tab_file.open("D://pre_table.txt");
    E_final();
    F_first_all();
    F_follow();
    F_select();
    is_LL();
    pre_table();
    F_analysis();
	fclose(file_1);
	pre_tab_file.close();
	select_file.close();
	follow_file.close();
	first_file.close();
	infile.close();
	return 0;
}
