
#include <iostream>


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
	//抽象的控件类
	class Control
	{
	public:
		virtual void draw() = 0; //draw方法，用于将自身绘制到屏幕上。
	public:
		virtual ~Control() {} //做父类时析构函数应该为虚函数
	};

	//列表控件类
	class ListCtrl :public Control
	{
	public:
		virtual void draw()
		{
			cout << "绘制普通的列表控件！" << endl; //具体可以用DirectX或OpenGL来绘制
		}
	};

	//抽象的装饰器类
	class Decorator :public Control
	{
	public:
		Decorator(Control* tmpctrl) :m_control(tmpctrl) {} //构造函数
		virtual void draw()
		{
			m_control->draw(); //虚函数，调用的是哪个draw，取决于m_control指向的对象
		}
	private:
		Control* m_control; //需要被装饰的其他控件，这里用的是Control *；
	};

	//----------------------
	//具体的“边框”装饰器类
	class BorderDec :public Decorator
	{
	public:
		BorderDec(Control* tmpctrl) :Decorator(tmpctrl) {} //构造函数
		virtual void draw()
		{
			Decorator::draw(); //调用父类的draw方法以保持以往已经绘制出的内容
			drawBorder(); //也要绘制自己的内容
		}
	private:
		void drawBorder()
		{
			cout << "绘制边框!" << endl;
		}
	};

	//具体的“垂直滚动条”装饰器类
	class VerScrollBarDec :public Decorator
	{
	public:
		VerScrollBarDec(Control* tmpctrl) :Decorator(tmpctrl) {} //构造函数
		virtual void draw()
		{
			Decorator::draw(); //调用父类的draw方法以保持以往已经绘制出的内容
			drawVerScrollBar(); //也要绘制自己的内容
		}
	private:
		void drawVerScrollBar()
		{
			cout << "绘制垂直滚动条!" << endl;
		}
	};

	//具体的“水平滚动条”装饰器类
	class HorScrollBarDec :public Decorator
	{
	public:
		HorScrollBarDec(Control* tmpctrl) :Decorator(tmpctrl) {} //构造函数
		virtual void draw()
		{
			Decorator::draw(); //调用父类的draw方法以保持以往已经绘制出的内容
			drawHorScrollBar(); //也要绘制自己的内容
		}
	private:
		void drawHorScrollBar()
		{
			cout << "绘制水平滚动条!" << endl;
		}
	};
}
namespace _nmsp2
{
	//抽象饮料类
	class Beverage
	{
	public:
		virtual int getprice() = 0; //获取价格
	public:
		virtual ~Beverage() {}
	};

	//水果饮料类
	class FruitBeverage : public Beverage
	{
	public:
		virtual int getprice()
		{
			return 10; //一杯单纯的水果饮料，售价为10元
		}
	};

	//抽象的装饰器类
	class Decorator :public Beverage
	{
	public:
		Decorator(Beverage* tmpbvg) :m_pbvg(tmpbvg) {} //构造函数
		virtual int getprice()
		{
			return m_pbvg->getprice();
		}
	private:
		Beverage* m_pbvg;
	};

	//具体的“砂糖”装饰器类
	class SugarDec :public Decorator
	{
	public:
		SugarDec(Beverage* tmpbvg) :Decorator(tmpbvg) {} //构造函数
		virtual int getprice()
		{
			return Decorator::getprice() + 1; //额外加多1元，要调用父类的getprice方法以把以往的价格增加进来
		}
	};

	//具体的“牛奶”装饰器类
	class MilkDesc :public Decorator
	{
	public:
		MilkDesc(Beverage* tmpbvg) :Decorator(tmpbvg) {} //构造函数
		virtual int getprice()
		{
			return Decorator::getprice() + 2; //额外加多2元，要调用父类的getprice方法以把以往的价格增加进来
		}
	};

	//具体的“珍珠”装饰器类
	class BubbleDesc :public Decorator
	{
	public:
		BubbleDesc(Beverage* tmpbvg) :Decorator(tmpbvg) {} //构造函数
		virtual int getprice()
		{
			return Decorator::getprice() + 2; //额外加多2元，要调用父类的getprice方法以把以往的价格增加进来
		}
	};
}


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//程序退出时检测内存泄漏并显示到“输出”窗口

	//第6章  装饰（Decorator）模式：装饰器模式/包装模式，结构型模式
	//（1）问题的提出
	 //UI（用户接口）界面。
	   //a)ListCtrl类代表普通列表控件，提供draw方法
	   //b)BorderListCtrl类，继承自ListCtrl，增加了边框的列表控件，提供draw方法
	   //c)VerScBorderListCtrl类继承自BorderListCtrl，表示增加了边框又增加了垂直滚动条的列表控件，提供draw方法
	   //d)HorScVerScBorderListCtrl类，继承自VerScBorderListCtrl，表示增加了边框，垂直、水平滚动条的列表控件，提供draw方法
	//继承 改为 组装方式来解决，防止类泛滥
	   //a)ListCtrl类代表普通列表控件，提供draw方法
	   //b)增加边框->带边框的列表控件
	   //c)增加垂直滚动条->带纯质滚动条的列表控件，再给这个带垂直滚动条的列表控件增加一个水平滚动条->既带垂直滚动条又带水平滚动条的列表控件。

	//这种通过装饰方式将一个类的功能不断增强的思想（动态的增加新功能），就是装饰模式的核心设计思想。
	//public继承：is -a 关系，组合关系和委托关系。

	//（2）引入装饰（Decorator）模式
	//组合复用原则（Composite Reuse Principle，CRP），也称为合成复用原则/聚合复用原则。
	  //若两个使用继承进行设计，则父类代码的修改可能影响子类的行为，而且，可能父类中的很多方法子类是用不上的，这显然是一种浪费， 
	   //若使用组合进行设计，则可以大大降低两个类之间的依赖关系，也不会存在因继承关系导致的浪费行为，所以
	    //如果继承和组合都能达到设计目的，优先考虑使用组合（组合优于继承）。
	//“装饰”设计模式的定义（实现意图）：动态的给一个对象添加一些额外的职责。就增加功能来说，该模式相比生成子类更加灵活。
	//装饰模式包含的四种角色：
	 //a：Control（抽象构件）：draw，让调用者以一致的方式处理未被修饰的对象以及经过修饰之后的对象，实现客户端的透明操作。
	 //b：ListCtrl（具体构件）：实现抽象构件定义的接口，此后，装饰器就可以给该构件增加额外的方法（职责）；
	 //c：Decorator（抽象装饰器类）：
	 //d：BorderDec、VerScrollBarDec、HorScrollBarDesc（具体装饰器类）：增加了一些新方法，然后通过对draw接口的扩展，达到最终的修饰目的。
	
	//（3）另一个装饰模式的范例:计算水果饮料最终价格
	//a)一杯单纯的水果饮料，售价为10元。
	//b)如果向饮料中增加砂糖，则额外要加多1元。
	//c)如果向饮料中增加牛奶，则额外要加多2元。
	//d)如果向饮料中增加珍珠，则额外要加多2元。
	//e)又加珍珠又加砂糖，10+2+1 = 13






	/*
	//(1)创建一个又带边框，又带垂直滚动条的列表控件
	//首先绘制普通的列表控件
	_nmsp1::Control* plistctrl = new _nmsp1::ListCtrl(); //本体

	//接着“借助普通的列表控件”，可以通过边框装饰器绘制出一个“带边框的列表控件”
	_nmsp1::Decorator* plistctrl_b = new _nmsp1::BorderDec(plistctrl); //_nmsp1::Decorator*用成_nmsp1::Control *

	//接着“借助带边框的列表控件”，就可以通过垂直滚动条装饰器绘制出一个“带垂直滚动条又带边框的列表控件”
	_nmsp1::Decorator* plistctrl_b_v = new _nmsp1::VerScrollBarDec(plistctrl_b);
	plistctrl_b_v->draw(); //这里完成最终绘制

	cout << "--------------------" << endl;
	//(2)创建一个只带水平滚动条的列表控件
	//首先绘制普通的列表控件
	_nmsp1::Control* plistctrl2 = new _nmsp1::ListCtrl(); //本体

	//接着“借助普通的列表控件”，可以通过水平滚动条装饰器绘制出一个“带水平滚动条的列表控件”
	_nmsp1::Decorator* plistctrl2_h = new _nmsp1::HorScrollBarDec(plistctrl2);
	plistctrl2_h->draw();

	//(3)释放资源
	delete plistctrl_b_v;
	delete plistctrl_b;
	delete plistctrl;

	delete plistctrl2_h;
	delete plistctrl2;
	*/

	//创建一杯单纯的水果饮料，价格10元：
	_nmsp2::Beverage* pfruit = new _nmsp2::FruitBeverage();
	//向饮料中增加珍珠，价格多加了2元
	_nmsp2::Decorator *pfruit_addbubb = new _nmsp2::BubbleDesc(pfruit);
	//再向饮料中增加砂糖，价格又加多了1元
	_nmsp2::Decorator* pfruit_addbubb_addsugar = new _nmsp2::SugarDec(pfruit_addbubb);
	//输出最终的价格
	cout << "加了珍珠又加了砂糖的水果饮料最终价格是：" << pfruit_addbubb_addsugar->getprice() << "元人民币" << endl;

	//释放资源
	delete pfruit_addbubb_addsugar;
	delete pfruit_addbubb;
	delete pfruit;

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


