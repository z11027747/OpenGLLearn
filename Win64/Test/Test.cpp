// Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

using namespace std;;

bool lengthCompare(const string& s1, const string& s2) {
	return size(s1) > s2.size() ? true : false;
}

bool display(const string& s1, const string& s2, bool (*pf) (const string&, const string&)) {

	if (pf(s1, s2)) {
		return true;
	}
	else {
		return false;
	}
}


int main()
{
	//函数指针
	bool (*pf) (const string&, const string&);

	pf = lengthCompare;
	bool result = pf("test", "test2");

	cout << result;

	//函数指针当参数
	display("test", "test2", pf);

	return 0;
}

class Test {

	//友元函数，直接访问
	friend void print(Test& test);

public:

	//重载操作符
	Test operator+(const Test& test);
	//友元函数重载操作符
	friend Test operator+ (const Test& t1, const Test& t2);
	//前置自增
	Test operator++();
	//重载输出运算符
	friend ostream& operator<< (ostream& os, const Test& t);


private:
	int a;
	int b;
};

void print(Test& test)
{
	test.a;
	test.b;
}

void test() {
	Test a;
	Test b;

	//调用方式都行
	a + b;
	a.operator+(b);
	operator+(a, b);
}

Test operator+(const Test& t1, const Test& t2)
{
	Test result;
	result.a = t1.a + t2.a;
	return result;
}
Test Test::operator+(const Test& test)
{
	Test result;
	result.a = this->a + test.a;
	return result;
}
Test Test::operator++()
{
	Test result;
	result.a = this->a + 1;
	return result;
}
ostream& operator<<(ostream& os, const Test& t)
{
	os << "我是输出";
	return os;
}

class A {
public:
	//不定义virtual只会调用父类，无法实现多台
	//除非是用作用域 A::print() 
	virtual void print() {
		cout << "A";
	}

	//纯虚函数
	virtual void test() = 0;

	int a;
};

class B : public A {
public:
	virtual void print() {
		cout << "B";
	}
};

//函数模板
//当被调用时，会生成相应的模板函数
template<typename T>
T max(T a, T b)
{
	return a > b ? a : b;
}

//类模板
template<typename T>
class TTest {

public :
	T t;
	T get();

};

//模板说明
template<typename T>
T TTest<T>::get()
{
	return T();
}
