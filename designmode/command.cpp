// MyProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//公众号：程序员速成 ，内含一辈子都让你感激自己的优质视频教程，欢迎关注

#include <iostream>
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
	//厨师类
	class Cook
	{
	public:
		//做红烧鱼
		void cook_fish()
		{
			cout << "做一盘红烧鱼菜品" << endl;
		}
		
		//做锅包肉
		void cook_meat()
		{
			cout << "做一盘锅包肉菜品" << endl;
		}
		//做其他各种菜品......略
	};

	//----------------------
	//厨师做的每样菜品对应的抽象类
	class Command
	{
	public:
		Command(Cook* pcook)
		{
			m_pcook = pcook;
		}
		//做父类时析构函数为虚函数
		virtual ~Command() 
		{
			if (m_pcook != nullptr)
			{
				delete m_pcook;
				m_pcook = nullptr;
			}
		}
		virtual void Execute() = 0;
		//virtual void Undo() = 0;
	protected:
		Cook* m_pcook; //子类需要访问
	};

	//做红烧鱼菜品命令
	class CommandFish :public Command
	{
	public:
		CommandFish(Cook* pcook) :Command(pcook) {}
		virtual void Execute()
		{
			m_pcook->cook_fish();
		}
	};

	//做锅包肉菜品命令
	class CommandMeat :public Command
	{
	public:
		CommandMeat(Cook* pcook) :Command(pcook) {}
		virtual void Execute()
		{
			m_pcook->cook_meat(); //将一个动作封装成了一个对象
		}
	};

	//-----------------
	/*
	//服务员类
	class Waiter
	{
	public:
		void SetCommand(Command* pcommand) //顾客把便签交给服务员
		{
			m_pcommand = pcommand;
		}
		void Notify() //服务器员将便签交到厨师手里让厨师开始做菜
		{
			m_pcommand->Execute();
		}
	private:
		Command* m_pcommand; //服务员手中握着顾客书写的菜品便签
	};*/

	//服务员类
	class Waiter
	{
	public:
		//将顾客的便签增加到便签列表中，即便一个便签中包含多个菜品，这也相当于一道一道菜品加入到列表中
		void AddCommand(Command* pcommand)
		{
			m_commlist.push_back(pcommand);
		}

		//如果顾客想撤单则将便签从列表中删除
		void DelCommand(Command* pcommand)
		{
			m_commlist.remove(pcommand);
		}

		void Notify() //服务员将所有便签一次性交到厨师手里让厨师开始按顺序做菜
		{
			//依次让厨师做每一道菜品
			for (auto iter = m_commlist.begin(); iter != m_commlist.end(); ++iter)
			{
				(*iter)->Execute();
			}
		}
		
	private:
		//一个便签中可以包含多个菜品甚至可以一次收集多个顾客的便签，达到一次性通知厨师做多道菜的效果
		std::list<Command*> m_commlist; //菜品列表，每道菜品作为一项，如果一个便签中有多个菜品，则这里将包含多项
	};

	//--------------------
	//实习服务器员类
	class Traineewaiter
	{
	public:
		Traineewaiter(Command* pcommand) :m_pcommand(pcommand) {} //构造函数
		void Notify() //实习服务员将便签交到厨师手里让厨师开始做菜
		{
			m_pcommand->Execute();
		}
		~Traineewaiter() //析构函数
		{
			if (m_pcommand != nullptr)
			{
				delete m_pcommand;
				m_pcommand = nullptr;
			}
		}

	private:
		Command* m_pcommand; //实习服务员手中握着顾客书写的菜品便签
	};

	//-------------------------
	//宏命令：把多个命令组合
	class CommandMacro :public Command
	{
	public:
		void AddCommand(Command* pcommand)
		{
			m_commlist.push_back(pcommand);
		}
		virtual void Execute()
		{
			for (auto iter = m_commlist.begin(); iter != m_commlist.end(); ++iter)
			{
				(*iter)->Execute();
			}
		}
	private:
		std::list<Command*> m_commlist;
	};
}

class TC
{
public:
	void operator()(int tv)
	{
		cout << "TC::Operator(int tv)执行了，tv=" << tv << endl;
	}
	int operator()(int tv1, int tv2)
	{
		cout << "TC::Operator(int tv1, int tv2)执行了，tv1=" << tv1 << "tv2=" << tv2 <<  endl;
		return 1;
	}
};

template <class T> //T代表可调用对象的类型
void functmptest(T callobj) //callobj是个可调用对象
{
	callobj(100);
	return;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	//第9章  命令（Command）模式:行为型模式。
	//（1）通过一个范例引出命令模式代码编写方法
	//红烧鱼，锅包肉
	//（2）引入命令（Command）模式：五种角色
	//a)Receiver（接收者类）：Cook类，cook_fish,cook_meat；
	//b)Invoker（调用者类）：Waiter类。
	//c)Command（抽象命令类）：Command类。
	//d)ConcreteCommand（具体命令类）：CommandFish类和CommandMeat类。
	//e)Client（客户端）
	//定义：将一个请求或命令封装为一个对象,l以便这些请求可以以对象的方式通过参数进行传递，对象化了的请求还可以排队执行或者
	   //根据需要将这些请求录入日志供查看和排错，以及支持请求执行后的可撤销操作。
	//能力：对请求进行封装，命令对象将动作和接收者包裹到了对象中并只暴露了一个Execute方法让接收者执行动作。
	//（3）命令模式用途研究:异步执行、延迟执行、排队执行、撤销、执行过程中增加日志记录等，是命令模式主要应用场景
	//（3.1）改造范例增加对象使用时的独立性
	//（3.2）命令模式使用场景谈与特点总结
	//a)在一些绘图软件中
	//b)遥控器实现对控制设备的解耦
	//c)任务的定期调度执行：Task Scheduler
	//d)游戏中时光倒流系统和回放系统的实现

	//命令模式的特点：
	//a)
	//b)
	//c)
	//问题思考：
	//a)命令对象作为回调函数的替代:Command模式是回调机制的一个面向对象的替代品。
	//b)极端情形：不引入调用者类；Command子类自己实现相关功能不引入接收者。
	//c)命令模式中命令对象与现代C++中可调用对象的比较






	/*
	_nmsp1::Cook* pcook = new _nmsp1::Cook();
	pcook->cook_fish();
	pcook->cook_meat();

	//释放资源
	delete pcook;
	*/

	/*
	_nmsp1::Cook cook;
	_nmsp1::Command* pcmd1 = new _nmsp1::CommandFish(&cook);
	pcmd1->Execute(); //做红烧鱼

	_nmsp1::Command* pcmd2 = new _nmsp1::CommandMeat(&cook);
	pcmd2->Execute(); //做锅包肉

	//释放资源
	delete pcmd1;
	delete pcmd2;
	*/

	/*
	_nmsp1::Cook cook;
	_nmsp1::Waiter *pwaiter = new _nmsp1::Waiter();

	_nmsp1::Command* pcmd1 = new _nmsp1::CommandFish(&cook);
	pwaiter->SetCommand(pcmd1);
	pwaiter->Notify(); //做红烧鱼

	_nmsp1::Command* pcmd2 = new _nmsp1::CommandMeat(&cook);
	pwaiter->SetCommand(pcmd2);
	pwaiter->Notify(); //做锅包肉

	//释放资源
	delete pcmd1;
	delete pcmd2;
	delete pwaiter;
	*/

	/*
	//_nmsp1::Cook cook;
	//一次性在便签上写下多道菜品
	//_nmsp1::Command* pcmd1 = new _nmsp1::CommandFish(&cook);
	//_nmsp1::Command* pcmd2 = new _nmsp1::CommandMeat(&cook);
	_nmsp1::Command* pcmd1 = new _nmsp1::CommandFish(new _nmsp1::Cook());
	_nmsp1::Command* pcmd2 = new _nmsp1::CommandMeat(new _nmsp1::Cook());

	_nmsp1::Waiter *pwaiter= new _nmsp1::Waiter();
	//把多道菜品分别加入到菜品列表
	pwaiter->AddCommand(pcmd1);
	pwaiter->AddCommand(pcmd2);

	//服务员一次性通知厨师做多道菜
	pwaiter->Notify();

	//释放资源
	delete pcmd1;
	delete pcmd2;
	delete pwaiter;
	*/

	_nmsp1::Traineewaiter* pwaitersx1 = new _nmsp1::Traineewaiter(new _nmsp1::CommandFish(new _nmsp1::Cook()));
	pwaitersx1->Notify(); //做红烧鱼

	_nmsp1::Traineewaiter* pwaitersx2 = new _nmsp1::Traineewaiter(new _nmsp1::CommandMeat(new _nmsp1::Cook()));
	pwaitersx2->Notify(); //做锅包肉

	delete pwaitersx1;
	delete pwaitersx2;


	TC tc;
	tc(20); //调用的是()操作符，这就是可调用对象。等价于tc.operator()(20);
	tc(30,50); 
	functmptest(tc);

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

