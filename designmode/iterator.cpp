#include <iostream>
#include <vector>
#include <list>


#ifdef _DEBUG   //只在Debug（调试）模式下
#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__) //重新定义new运算符
#define new DEBUG_NEW
#endif
#endif

//#include <boost/type_index.hpp>
using namespace std;
//#pragma warning(disable : 4996) 

namespace _nmsp1
{	
	//template <typename T>
	//class myVector
	//{
	//	//容器功能，比如插入数据等。。。
	//	//迭代器功能，比如遍历数据等。。。
	//	//......其他各种功能
	//};

	//抽象迭代器类模板
	template <typename T>
	class myIter
	{
	public:
		virtual void First() = 0; //指向容器中第一个元素
		virtual void Next() = 0;  //指向下一个元素
		virtual bool IsDone() = 0; //是否遍历完
		virtual T& CurrentItem() = 0; //获取当前的元素
		virtual ~myIter() {} //做父类时析构函数应该为虚函数
	};

	//抽象容器类模板
	template <typename T>
	class myContainer
	{
	public:
		virtual myIter<T>* CreateIterator() = 0; //创建迭代器
		virtual T& getItem(int index) = 0; //获取当前元素
		virtual int getSize() = 0; //容器元素数量
		virtual ~myContainer() {} //做父类时析构函数应该为虚函数
	};

	//具体迭代器类模板，为简单起见，本迭代器针对的是大小为10个元素的数组
	template <typename T>
	class myVectorIter :public myIter<T>
	{
	public:
		myVectorIter(myContainer<T>* tmpc) :myvector(tmpc)
		{
			m_current = 0;
		}
		virtual void First()
		{
			m_current = 0; //容器（数组）中的第一个元素下标为0
		}
		virtual void Next()
		{
			m_current++; //下标+1，意味着数组中的下一个元素
		}
		virtual bool IsDone()
		{
			if (m_current >= myvector->getSize())
			{
				return true;
			}
			return false;
		}
		virtual T& CurrentItem()
		{
			return myvector->getItem(m_current);
		}
	private:
		myContainer<T>* myvector;
		int m_current; //记录数组的当前下标（迭代器在当前容器中的位置）
	};

	//具体容器类模板
	template <typename T>
	class myVector :public myContainer<T>
	{
	public:
		myVector()
		{
			//将数组中元素进行初始化
			for (int i = 0; i < 10; ++i)
			{
				m_elem[i] = i;
			}
		}
		virtual myIter<T>* CreateIterator()
		{
			//工厂模式，注意实参传递进去的是该容器的指针this
			return new myVectorIter<T>(this); //要考虑在哪里释放的问题
		}
		virtual T& getItem(int index)
		{
			return m_elem[index];
		}
		virtual int getSize()
		{
			return 10; //为简化代码，返回固定数字
		}
	private:
		//为了简化代码，将容器实现为固定装入10个元素的数组
		T m_elem[10];
	};
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口
	
	//第10章 迭代器（Iterator）模式：行为型模式
	//（1）容器和迭代器的简单范例:vector，list
	//迭代器模式正式用来设计和书写迭代器 。
	//将迭代器单独实现为一个类模板。

	//（2）单一职责原则(Single Responsibility Principle：简称SRP)
	//（2.1）单一职责原则的定义：一个类应该只有一个引起变化的原因，通俗的说就是一个类的职责应该单一，应该只做一类事情或者对外只提供一种功能。
	//（2.2）内聚与耦合
	//内聚性：用于衡量一个模块（类）紧密的达到单一目的特性，指的是从功能角度来度量模块内的联系。模块只支持一组功能——高内聚。
	  //如果支持一组不相关的功能——低内聚。
	//耦合性：模块之间相互关联的程度。取决于两者间的接口的复杂程度，相互间的调用方式以及调用时哪些信息需要交互。
	//耦合性几种类型（从高到低）： 
	   //a)内容耦合：耦合程度最高，应该避免使用
	   //b)公共耦合：
	   //c)外部耦合：
	   //d)控制耦合：
	   //e)标记耦合：
	   //f)数据耦合：耦合程度最低 
	//尽量使用数据耦合，少用控制耦合，限制公共耦合的范围，尽量避免使用内容耦合。
	//程序设计：高内聚 低耦合
	//一个类的职责越单一，内聚度就越高，而要达到低耦合的目的，也要尽量遵循依赖倒置原则——高层组件和底层组件都依赖于抽象层或者说尽量使用抽象耦合来代替具体耦合。

	//（3）迭代器模式的范例及现代C++中的迭代器
	//（3.1）迭代器模式范例
	//迭代器设计模式的定义：提供一种方法顺序访问一个聚合对象（容器）中各个元素，而又不暴露该对象的内部表示（实现代码）；
	//4种角色：
	//a)Iterator(抽象迭代器）：MyIter类模板。
	//b)ConcreteIterator(具体迭代器）：MyVector类模板。
	//c)Aggregate（抽象聚合）：MyContainer类模板。
	//d)ConcreteAggregate(具体聚合)：MyVector类模板。
	//（3.2）现代C++中的迭代器









	/*
	std::vector<int> msgVector;
	msgVector.push_back(1); //往末尾插入1，vector中内容：1
	msgVector.push_back(2);//1,2
	msgVector.push_back(3); //1,2,3

	for (std::vector<int>::iterator pos = msgVector.begin(); pos != msgVector.end(); ++pos)
	{
		cout << *pos << endl;
	}
	cout << "----------------------" << endl;
	std::list<int>  msgList;
	msgList.push_back(1); //1
	msgList.push_front(2); //2,1
	msgList.push_back(3); //2,1,3
	for (std::list<int>::iterator pos = msgList.begin(); pos != msgList.end(); ++pos)
	{
		cout << *pos << endl;
	}
	*/

	_nmsp1::myContainer<int>* pcontainer = new _nmsp1::myVector<int>();
	//_nmsp1::myIter<int>* iter = pcontainer->CreateIterator();
	_nmsp1::myVectorIter<int> iter(pcontainer);
	//遍历容器的元素
	//for (iter->First(); !iter->IsDone(); iter->Next()) //多态机制的遍历，效率上不好，尽量考虑栈机制
	for (iter.First(); !iter.IsDone(); iter.Next())
	{
		//cout << iter->CurrentItem() << endl;
		cout << iter.CurrentItem() << endl;
	}

	//释放资源
	//delete iter;
	delete pcontainer;



	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

