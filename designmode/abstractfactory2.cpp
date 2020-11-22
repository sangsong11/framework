#include <iostream>
using namespace std;

//身体抽象类
class Body
{
    public:
        virtual void getName() = 0;
        virtual ~Body(){}
};
//衣服抽象类
class Clothes
{
  public:
    virtual void getName() = 0;
    virtual ~Clothes(){}
};
//鞋子抽象类ho
class Shoes
{
 public:
    virtual void getName() = 0;
    virtual ~Shoes(){}
};

//抽象工厂类

class AbstractFactory
{
  public:
       virtual Body* createBody() = 0;
       virtual Clothes* createClothes() = 0;
       virtual Shoes* createShoes() = 0;
       virtual ~AbstractFactory(){}
};

//芭比娃娃类
class BarbieDoll
{
    public:
        BarbieDoll(Body* tmpbody,Clothes* tmpclothes,Shoes* tmpshoes)
        {
             body = tmpbody;
             clothes = tmpclothes;
             shoes = tmpshoes;
        }
        void Assemble()   //组装芭比娃娃
        {
            cout<<"成功组装了一个芭比娃娃"<<endl;
            body->getName();
            clothes->getName();
            shoes->getName();
        }
    private:
       Body* body;
       Clothes* clothes;
       Shoes* shoes;
};
//中国产商实现的三个部件
class China_Body: public Body
{
    public:
        virtual void getName()
        {
             cout<<"中国产商产的身体部件"<<endl;  
        }       
};

class China_Clothes: public Clothes
{
    public:
        virtual void getName()
        {
             cout<<"中国产商产的衣服部件"<<endl;  
        }       
};
class China_Shoes: public Shoes
{
    public:
        virtual void getName()
        {
             cout<<"中国产商产的鞋子部件"<<endl;  
        }       
};

//创建一个中国工厂
class ChinaFactory:public AbstractFactory
{
    public:
         virtual Body* createBody() 
         {
             return new China_Body;
         }
         virtual Clothes* createClothes()
         {
             return new China_Clothes;
         }
         virtual Shoes* createShoes()
         {
            return new China_Shoes;
         }
};


//日本产商实现的三个部件
class Japan_Body: public Body
{
    public:
        virtual void getName()
        {
             cout<<"日本产商产的身体部件"<<endl;  
        }       
};

class Japan_Clothes: public Clothes
{
    public:
        virtual void getName()
        {
             cout<<"日本产商产的衣服部件"<<endl;  
        }       
};
class Japan_Shoes: public Shoes
{
    public:
        virtual void getName()
        {
             cout<<"日本产商产的鞋子部件"<<endl;  
        }       
};

//创建一个日本工厂
class JapanFactory:public AbstractFactory
{
    public:
         virtual Body* createBody() 
         {
             return new Japan_Body;
         }
         virtual Clothes* createClothes()
         {
             return new Japan_Clothes;
         }
         virtual Shoes* createShoes()
         {
            return new Japan_Shoes;
         }
};


//美国产商实现的三个部件
class America_Body: public Body
{
    public:
        virtual void getName()
        {
             cout<<"美国产商产的身体部件"<<endl;  
        }       
};

class America_Clothes: public Clothes
{
    public:
        virtual void getName()
        {
             cout<<"美国产商产的衣服部件"<<endl;  
        }       
};
class America_Shoes: public Shoes
{
    public:
        virtual void getName()
        {
             cout<<"美国产商产的鞋子部件"<<endl;  
        }       
};

//创建一个美国工厂
class AmericaFactory:public AbstractFactory
{
    public:
         virtual Body* createBody() 
         {
             return new America_Body;
         }
         virtual Clothes* createClothes()
         {
             return new America_Clothes;
         }
         virtual Shoes* createShoes()
         {
            return new America_Shoes;
         }
};

int main(int argc,char **argv)
{
    AbstractFactory* pChinaFactory = new ChinaFactory();
    Body *pChinaBody =   pChinaFactory->createBody();
    Clothes *pChinaClothes =  pChinaFactory->createClothes();
    Shoes *pChinaShoes =  pChinaFactory->createShoes();

    BarbieDoll *wawa = new BarbieDoll(pChinaBody,pChinaClothes,pChinaShoes);
    wawa->Assemble();
    return 0;
}
