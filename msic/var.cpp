// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdarg.h>
using namespace std;

#define VA_START(args,type)  (args = (char*)&type + sizeof(type))
#define VA_ARG(args,type)  (*(type*) ( (args += sizeof(type)) - sizeof(type) ))
#define VA_END(args)  ( args = nullptr)

void test(int count, ...)
{
	int i = 0;
	char* c = 0;
	char *start = nullptr;
	VA_START(start, count);
	for( i = 0; i < count;i++)
	{
		cout << VA_ARG(start, char*) << '\n';
	}
	//VA_END(start);
}

int putchar(int ch);

void print_int(int value) {
	char temp[30] = { 0 };
	int cnt = 0;
	while (value) {
		temp[cnt++] = value % 10;
		value = value / 10;
	}
	while (cnt) {
		cnt--;
		putchar(temp[cnt] + '0');
	}
}

void print_string(const char *value) {
	while (*value) {
		putchar(*value);
		value++;
	}
}

void simple_print(const char *control, ...) {
	va_list parg;
	va_start(parg, control);
	while (*control) {
		if (*control == '%') {
			char type = *(control + 1);
			if (type == 'd') {
				int value = va_arg(parg, int);
				print_int(value);
			}
			else if (type == 'c') {
				int value = va_arg(parg, int);
				putchar(value);
			}
			else if (type == 's') {
				const char *value = va_arg(parg, const char *);
				print_string(value);
			}
			if (type == 'd' || type == 'c' || type == 's') {
				control++;
			}
		}
		else {
			putchar(*control);
		}
		control++;
	}
	va_end(parg);
}

int main()
{
	test(4, "11", "22", "33", "44");
    std::cout << "Hello World!\n";
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
