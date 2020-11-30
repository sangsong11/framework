#include <iostream>
#include <string>
using namespace std;
class mystring
{
    public:
       mystring(const char* tmpstr = ""):pvalue(new stringvalue(tmpstr))//构造函数
       {
           
       }

       mystring(const mystring& tmpstr)//: pvalue(tmpstr.pvalue)   //拷贝构造函数
       {
           if(tmpstr.pvalue->shareable)
           {
                pvalue=tmpstr.pvalue;
                ++pvalue->refcount;
           }
           else
           {
               //复制一份
               pvalue = new stringvalue(tmpstr.pvalue->point);
           }

       }
       mystring& operator=(const mystring& tmpstr) //构造赋值运算符
       {
           if(this == &tmpstr)   //增加是否是自己的判断
           {
               return *this;
           }
           --pvalue->refcount;
           if(pvalue->refcount == 0)
           {
               delete pvalue;
           }
           pvalue = tmpstr.pvalue;
           ++pvalue->refcount;
           return *this;
       }
     /*  const char& operator[](int idx) const 
       {
          return pvalue->point[idx];
       }*/
       char& operator[](int idx)
       {
          if(pvalue->refcount > 1)
          {
              //说明还有其他对象指向这个字符串
              --pvalue->refcount;
              pvalue =new stringvalue(pvalue->point)  //写时复制
          } 
          pvalue->shareable = false;
          return pvalue->point[idx];
       }

       ~mystring()
       {
           --pvalue->refcount;
           if(pvalue->refcount == 0)
           {
               delete pvalue;
           }
       }
       private:
            struct stringvalue
            {
                size_t refcount;    //引用计数
                char *point;      //指向实际字符串
                bool shareable;        //一个是否能否被共享的标技
                stringvalue(const char *tmp):refcount(1),shareable(true)//构造函数
                {
                     point = new char[strlen(tmpstr)+1];
                     strcpy(point,tmpstr);
                }
                ~stringvalue
                {
                     delete[] point;
                }
            }
        private:
            stringvalue *pvalue;   //mystring类中指向stringvalue 对象的指针
         
};


int main(int argc, char* argv[])
{

}