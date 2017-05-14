
摘 要
 
   本次软件课程设计的课题是设计并实现词法和语法分析器。输入源程序，进行词法分析，输出单词符号。语法分析方法通常分为两大类： 
1.自顶向下法（推导法） 
2.自底向上法（规约法） 
关键技术分别为推导和规约。
使用Devc++编译环境以及c++语言编写。通过设计和实现词法和语法分析器，提高我们对源程序编译的理解能力，从而培养我们分析、解决问题的能力。

关键字：Grammer   Lexical  DNA  NFA 自动识别 first()  follow()  select() 预测分析表 正规文法 2型文法 扫描器


1.设计目的
（1）熟悉并实现一个扫描器（词法分析程序）。
（2）熟悉自动机识别原理及其过程。
（3）加深对数据结构和算法设计的理解
（4）熟悉对字符串的语法分析以及2型文法的first集、follow集、select集求法。
（5）掌握NFA构建以及NFA转DFA算法，DFA最小化算法。
2.设计内容
 2.1词法分析器
创建一个词法分析程序，支持分析常规语法。该程序使用DFA（确定性有限自动机）或NFA（非确定性有限自动机）来实现扫描功能。 
程序的输入是两个文本文档，一个是一组3型文法（常规语法），一个是源代码文本文档，包含要重新认证的一组字符串。程序的输出是单词符号表，由5种类型的单词符号组成：标识符（变量名，常量名等），常量，关键字（if，end等），运算符和界符（逗号，括号等），它们对应的整型编码分别是2、3、1、4、5。
单词符号的输出通常采用以下二元式表示（单词种别，单词自身的值），或者直接输出。该程序执行完会生成一个输出文档，文档中含有字符串，用于语法分析程序的输入，文档中标识符和常量分别用编码2、3表示，关键字用其映射的单个字符表示，界符以及运算符则用本身表示。
 2.2语法分析器
     创建一个使用LL（1）方法分析输入字符串的语法分析程序。程序的输入是两个文本文档，包括一组2型语法（上下文无关语法）和任务1生成的单词符号表。该程序的输出是YES或NO。 即源代码字符串符合这种2型语法，或者源代码字符串不符合该2型语法。
程序内容包括空字符的非终结符集合求解、first集、follow集、select集、LL（1）文法判别预测分析表的构建等内容
3. 设计原理及设计步骤	
3.1 设计原理	
   （1）源程序需经过以下操作才能得到目标程序：词法分析、语法分析、语义分析、中间代码生成、代码优化、目标代码生成。其中词法分析是从左到右读入源程序的每个字符，对构成源程序的字符流进行扫描和分解，从而识别出一个个单词（也叫单词符号或符号）。单词是具有独立意义的最小语法单位。语法分析是依据语言的语法规则，确定源程序的输入串是否构成一个语法上正确的程序。最终将单词序列分解成各类语法短语（也叫语法单位），如“程序”、“语句”、“表达式”等。语法是由程序语言基本符号组成程序中各个语法成分的一组规则。
   （2）有限自动机（FA）是一个识别装置，用于识别正规集（正规语言），其核心是变换的实现技术，通过将输入的产生式初始化为NFA，再由子集法将NFA转为DFA，并最小化DFA，能够实现字符串的自动识别。DFA的一个状态可能对应NFA的一个或一组状态。DFA同样记录在NFA上读入某个VT后可能到达的所有状态。
3.2 设计步骤	
3.2.1 词法分析设计基本步骤	
（1）基本思路
1）从词法分析_文法.txt中读入预先写好的文法，并将其转换为NFA，存储NFA的数据结构为二维结构体，结构体定义如下
struct NFA_set
{
	char set[100];
	int len=0;
};
2）用子集法将NFA转化为DFA。转化过程与课本中子集法的转化过程是一致的。
程序中，int Is_in(NFA_set temp)函数用于判断新产生的一个子集是否是已经存在的，防止重复。不重复则返回-1，重复则返回重复的编号。
3）void get_closure(NFA_set &temp)函数用于计算ε-closure。
bool Is_contained_Y(NFA_set temp)函数用于判断该状态是否是终态。
转化过程是具体实现是：做一个NFA_set类型的栈，新状态不停的进栈，每次从堆栈中弹出一个，计算其转换的结果，是新状态就进栈，重复则丢弃，只记录好dfa的转换，直到栈空。最后成功构造出DFA。

 （2）程序流图

3.2.2 语法分析设计基本步骤	
（1）基本思路
 1）从文件读取2型文法，并存在二维数组pro中，遍历产生式取出终结符与非终结符，存放在一维数组中。
    void Pro_Init(){
  num_final=0; num_nonfinal=0;num_pro=0; int i,j,k,l,m,n;int flag;
  if((file_1=fopen("D://production.txt","r"))==NULL){
		cout<<"指定目录下找不到文件！"<<endl;
		exit(0); 
  }ch=fgetc(file_1);
2）根据产生式以及非终结符、终结符，求出空字符串的非终结符集，消除左递归。
 （2）程序流图

主程序：

分析子程序：

4. 数据结构设计	
4.1 词法数据结构	
（1）单词符号与编码对应表
关键字	标识符	常量	界符	运算符
3	1	2	5	4

（2）一些已知常量字符数组：
关键字表：{"break","case","char","continue","do","default","double",
"else","float","for","if","int","include","long","main","return","switch","typedef","void","unsigned","while","iostream"};
单目运算表：{'+','-','*','/','!','%','~','&','|','^','='};  
双目运算符表：{"++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/="};
界符表：{',','(',')','{','}',';','<','>','#'}; 
关键字与单个字母对应表格：{'b','c','a','o','d','n','z','e','y','f','i','h','p','l','m','r','s','t','v','u','w','k'};

（3）主要函数：
void createNFA()从文本文件中创建文法带程序中。
int Is_in(NFA_set temp)和已有的newset有没有重复的，有就返回重复的编号
void get_closure(NFA_set &temp)得到一个完整的子集 
bool Is_contained_Y(NFA_set temp)判断是否是终态
void NFA_T_DFA() NFA转换为DFA的关键函数
bool DFA(char str[])返回str串是否可以由DFA表示出来
void exescan()扫描源程序，给出词法分析的结果。

4.2 语法数据结构	
（1）变量：
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
char pro[40][40];//不需要对这个数组初始化； 
int E_non[40];
char temp[40][40]; //暂时保存输入的产生式 
char ch;
char start;
char first[200][200];
char follow[200][200];
char select[200][200];
int  predict[200][200];
（2）E_final();
    F_first_all();
    F_follow();
    F_select();
    is_LL();
    pre_table();
    F_analysis();
5. 关键代码分析（带注释）及运行结果	
5.1 词法关键代码及结果分析	
5.1.1 词法关键代码	
（1）typedef vector <string> StrVector;  //用正向迭代器输出单词符号 
typedef vector <char> ChVector;
 map <string,char> myMap;  //关键字映射为字母表示；标识符则用整数编码表示；
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
（2）void NFA_Init()
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
				move[i][j].set[k]='#';  
                move[i][j].len=0; 
			}
	}
}
（3）void ExeScan()
{
	char str[100]; 
	char ch;
	int i,j,point,flag;
	ch=fgetc(file_source);//ch仍为开始字符；
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
				cout<<str<<" "<<"出错，不是常量"<<endl;
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
					cout<<str<<" "<<"出错，不是标识符"<<endl;
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
5.1.2 词法运行结果分析	
（1）运行结果

（2）输出文档

5.2 语法关键代码及结果分析	
5.2.1 语法关键代码
（1）int Map(char a) 
	for(int q=0;q<strlen(nonfinal);q++){
		if(nonfinal[q]==a&&a!=' ')
		return q;
	}
}
  代码作用为将非终结符与其在非终极符数组中的序号对应，类似的函数还有，Map_final(char a)(终极符与其在终结符数组中的序号对应),Map_pro(char a)（终结符与从0开始的最近的产生式的序号对应）。

（2）delete_dup(final);
	delete_dup(nonfinal);调用delete_dup函数消除重复出现的字符，让数组中的值唯一。

（3）if(k==num_pro) return;
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
 求单个字符的first集合时需要用到递归函数，通过判断下一个字符的属性来决定是否递归。（4）	while(ana_st.top()!='#'){
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
  字符串分析时，通过查看分析栈的首字符与字符输入栈的首字符来决定pop和push操作对象，进行正确的归约和置换。
（5）void F_select(){// 各产生式的select集合
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
（6）int i,j;
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
（7）bool is_final(char a){
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
（8）消除左递归：
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
5.2.2 语法运行结果
（1）运行结果如下图：

（2）各个文件的内容：

 .
 

6. 总结与分析	
6.1 程序的优点
课程设计的要求都基本达到，能够正常运行，程序输出清晰，使用文档保存了中间数据。	
6.2 程序的缺点	
  重复代码还存在，变量命名仍不够规范。
6.3 个人感想	
   通过本次软件课程设计，让我对编译原理这门课程有了更深的理解，现在的计算机尚不能直接执行高级语言程序。执行一个高级语言程序大体上要分两步：第一步，把高级语言的源程序编译成低级语言的目标程序；第二步，运行这个目标程序。编译程序的典型工作过程是：输入源程序，对它进行加工处理，输出目标程序。加工处理是非常复杂的过程，它又可划分成以下几个阶段： 源程序→词法分析→语法分析→产生中间代码→优化→目标代码生成→目标程序。学习没有止境，需要我学习和探索的问题还有很多，我一直学习下去。
7. 问题	
7.1 词法分析问题	
（1）同语法分析乱码问题。
（2）将标识符与常量编码写进文档，而不是具体值，有利于语法分析。
7.2 语法分析问题
（1）文件中出现乱码问题

问题出在数组te没有以‘\0’做为结束符：
if(is_final(temp[k][3])){
			char te[2];
			te[0]=temp[k][3];
			te[1]='\0';
			strcat(first[Map(a)],te);
			if(temp[k+1][0]==temp[k][0]&&k+1!=num_pro)
			F_first(a,++k,n);
添加te[1]='\0';即可。
（2）LL(1)分析法是指从左到右扫描(第一个 L)、最左推导(第二个L)和只查看一个当
前符号(括号中的1)之意； 
LL(1)分析法又称预测分析法，属于自顶向下确定性语法分析方法。 
LL(1)分析法的基本要点有三： 
1）利用一个分析栈，记录分析过程； 
2）利用一个分析表，登记如何选择产生式的知识； 
3）此分析法要求文法必须是LL(1)文法。
（3）死循环问题出现stract函数的使用，字符串拼接导致访问内存出错，改写strcat函数，转为两个字符数组拼接，即可避免死循环问题。
8.注意事项
  （1）编程环境为Devc++，语言为c++。
  （2）本程序中产生式中的空字符串用‘@’代替。
  （3）词法分析中默认终态为‘Y’。
   



