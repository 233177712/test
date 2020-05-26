#include <iostream>
#include <sstream>
#include <string.h>
#include <stack>
#include<vector>
using namespace std;

long fact(int num)  //求阶乘
{
	int res = 0;
	if (0 == num)// 0的阶乘等于1
	{
		res = 1;
	}
	else if (1 == num) //边界条件
	{
		res = 1;
	}
	else if (num > 1)  //递归公式
	{
		res = num * fact(num - 1);
	}
	else
	{
		throw -1;
	}
	return res;
}
bool isMatch(string s) { //检查输入的表达式是否括号匹配
	stack<char> st;
	int len = s.size();
	for (int i = 0; i < len; i++) {
		if (s[i] == '(') {

			st.push('(');

		}
		else if (s[i] == ')') {
			if (st.empty()) {
				return false;
			}
			else {
				char ch = st.top();
				st.pop();
				if (ch != '(') {
					return false;
				}
			}

		}
		else {
		}
	}

	if (st.size() != 0) {
		return false;
	}
	return true;
}
bool isNum(string digit) {//判断是不是数字
	char c;
	bool flag;
	for (int i = 0; i < digit.size(); ++i) {
		c = digit[i];
		if (c != '.' && (c <'0' || c > '9'))
			return false;
	}
	return true;
}
string preProcess(string str)//-2:输入非法；-3：括号不匹配
{
	int len_str = str.size();
	char* p = (char*)malloc(sizeof(char) * (len_str + 1));
	int i = 0, j = 0;
	while (i < len_str) {
		if (str[i] == ' ') {
			++i;
			continue;
		}//输入中如果包含了非0-9，+， -， *， /，^，！，(，)，#的任何字符，抛出异常
		if (str[i] != '!' && str[i] != '^' && str[i] != '.' && 
			(str[i] <'0' || str[i] > '9') && str[i] != '+' && 
			str[i] != ')' && str[i] != '(' && str[i] != '-' && 
			str[i] != '*' && str[i] != '/' && str[i] != '#') {
			throw -2;
		}
		p[j++] = str[i++];
	}
	p[j] = '\0';
	if (isMatch(p))
		return p;
	else
		throw -3;
}
double convertFromString(string str) { //将string类型转换为double类型
	istringstream iss(str);
	double x;
	if (iss >> x)
		return x;
	return 0.0;
}
vector<string> separation(string str) { //把字符串分割为数字 每个数字为一个string对象 
	string t = "";
	vector<string> exp;
	for (int i = 0; i < str.size(); ++i) {
		if (isdigit(str[i]) || str[i] == '.') {
			t += str[i];
		}
		else {
			if (t.size() == 0) { //第一个字符不是数字的情况 
				t = str[i];
				exp.push_back(t);
			}
			else {
				exp.push_back(t);//把数字加入exp 
				t = str[i];
				exp.push_back(t);//把非数字加入exp 
			}
			t = "";
		}
	}
	if (t.size() > 0) { //最后一组是数字的时候需要特殊处理 
		exp.push_back(t);
	}

	vector<string> res;
	for (int i = 0; i < exp.size(); ++i) {
		if (exp[i] == "-") {
			if (i > 0 && (isNum(exp[i - 1]) || exp[i - 1] == ")"))
				res.push_back(exp[i]);//表示减号 
			else
				res.push_back("#");//表示负号 
		}
		else {
			res.push_back(exp[i]);
		}
	}
	return res;
}
vector<string> swap(string s)//swap函数将中缀表达式转换为后缀表达式
{
	vector<string> v, v_after; //v是读入的中缀表达式 v_after用来储存后缀表达式
	v = separation(s);
	vector<string>::iterator iter = v.begin();
	stack<char> operators;
	operators.push('#');
	bool flag = true;
	while (iter != v.end() && flag)
	{
		string t = *iter;
		//如果是操作符，就和栈顶元素比较，如果优先级高，就入栈，否则将栈顶元素输出，再将该操作符压入栈中
		//如果是操作数，就直接输出。
		switch (t[0])
		{
		case '(':
		{
			operators.push(t[0]);
			break;
		}
		case ')':
		{
			char c = operators.top();
			operators.pop();
			while (c != '(')
			{
				string s(1, c);
				v_after.push_back(s);
				c = operators.top();
				operators.pop();
			}
			break;
		}
		case '!': 
		{
			//只要栈顶元素不是'!'，就要入栈
			char c = operators.top();
			operators.pop();
			while (c == '!')
			{
				string s(1, c);
				v_after.push_back(s);
				c = operators.top();
				operators.pop();
			}
			operators.push(c);
			operators.push(t[0]);
			break;
		}
		case '^':
		{
			//只要栈顶元素不是'^'和'!'，就要入栈
			char c = operators.top();
			operators.pop();
			while (c == '^' || c == '!')
			{
				string s(1, c);
				v_after.push_back(s);
				c = operators.top();
				operators.pop();
			}
			operators.push(c);
			operators.push(t[0]);
			break;
		}
		case '*':
		case '/':
		{
			//只要栈顶元素不是'*','/','^','!'，就要入栈
			char c = operators.top();
			operators.pop();
			while (c == '*' || c == '/' || c == '^' || c == '!')
			{
				string s(1, c);
				v_after.push_back(s);
				c = operators.top();
				operators.pop();
			}
			operators.push(c);
			operators.push(t[0]);
			break;
		}
		case '+':
		case '-':
		{
			//只要栈顶元素不是'('和'#'，就要先将栈顶元素出栈
			char c = operators.top();
			operators.pop();
			while (c != '(' && c != '#')
			{
				string s(1, c);
				v_after.push_back(s);
				c = operators.top();
				operators.pop();
			}
			operators.push(c);
			operators.push(t[0]);
			break;
		}
		case '#':
		{
			//输入结束，将栈中元素统统出栈
			char c = operators.top();
			operators.pop();
			while (c != '#')
			{
				string s(1, c);
				v_after.push_back(s);
				c = operators.top();
				operators.pop();
			}
			flag = false;
			break;
		}
		default:
		{
			//操作数
			v_after.push_back(t);
			break;
		}
		}
		iter++;
	}
	return v_after;
}
double operate(double first, double second, char op)//进行双目运算  -3：0做分母
{
	double res = 0.00;
	switch (op)
	{
	case '+':
		res = first + second;
		break;
	case '-':
		res = first - second;
		break;
	case '*':
		res = first * second;
		break;
	case '/':
		if (second == 0.0) throw -3;
		res = first / second;
		break;
	case '^':
		res = pow(first, second);
	default:
		break;
	}
	return res;
}
double operate(double first, char op) {//进行单目运算
	double res = 0.00;
	switch (op)
	{
	case '!':
		res = fact(first);
		break;
	}
	return res;
}
double calculate(vector<string> v)
{
	stack<double> st;//操作数堆栈
	vector<string>::iterator iter = v.begin();
	while (iter != v.end())
	{
		string c = *iter;
		if (isNum(c))//如果是数字就压栈
		{
			st.push(convertFromString(c));
		}
		else if (c[0] == '!')
		{
			double a = st.top();
			st.pop();
			st.push(operate(a, c[0]));
		}
		else//遇到字符就弹出两个操作数进行运算
		{
			double a = st.top();
			st.pop();
			double b = st.top();
			st.pop();
			st.push(operate(b, a, c[0]));
		}
		iter++;
	}
	return st.empty() ? 0 : st.top();//最后的结果为栈顶元素
}
template<class T>
void print_vector(vector<T> v)//打印vector的函数
{
	for (const T& k : v)
		cout << k;
	cout << endl;
}
void main() {
	string c = "";
	vector<string> c_after;
	printf("请输入一个表达式(以’#‘作为结束符):\n");
	while (1) {
		cin >> c;
		c = preProcess(c);
		if (c == "error1")
			printf("您输入的表达式含有非法字符，请重新输入：\n");
		else if (c == "error2")
			printf("您输入的表达式括号不匹配，请重新输入：\n");
		else {
			c_after = swap(c);
			cout << "对应的后缀表达式为：";
			print_vector(c_after);
			cout << " = " << calculate(swap(c)) << "\n";
			char flag;
			printf("继续计算表达式请输入Y，退出请输入任意其他字符");
			cin >> flag;
			if (flag != 'Y') break;
		}
	}
}