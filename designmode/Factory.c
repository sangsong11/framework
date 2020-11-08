#include <iostream>


//怪物类
class Monster
{
        Monster(int life,int magic,int attack):m_life(life),m_magic(magic),m_attack(attack)
        virtual ~Monster（）  {}
   protected:
        int m_life;
        int m_magic;
        int m_attack;
};

//亡灵类怪物
class M_Undead:public Monster
{
    public:
        M_Undead(int life,int magic,int attack):Monster(life,magic,attack)
        {
            cout<<"一个亡灵类怪物来到了这个世界"<<endl;
        }
        ~M_Undead() {}
};

//元素类怪物
class M_Element:public Monster
{
    public:
        M_Element(int life,int magic,int attack):Monster(life,magic,attack)
        {
            cout<<"一个元素类怪物来到了这个世界"<<endl;
        } 
        ~M_Elemen() {}
};

//机械类怪物
class M_Mechanic:public Monster
{
    public:
        M_Mechanic(int life,int magic,int attack):Monster(life,magic,attack)
        {
            cout<<"一个机械类怪物来到了这个世界"<<endl;
        }
        ~M_Mechanic(){}
};


class M_ParFactory
{
    public:
          virtual Monster* createMonster() = 0;
          virtual ~M_ParFactory() {};  //做父类，析构函数虚函数
};
class M_UndeadFactory:public M_ParFactory
{
    public:
         virtual Monster* createMonster()
         {
             prtnobj = new M_Undead(300,50,80);
         }
};

class M_ElementFactory:public M_ParFactory
{
    public:
         virtual Monster* createMonster()
         {
             prtnobj = new M_Element(200,80,100);
         }
};

class M_MechanicFactory:public M_ParFactory
{
    public:
         virtual Monster* createMonster()
         {
             prtnobj = new M_Mechanic(400,0,110);
         }
};

//全局的用于创建怪物对象的函数

Monster* Gbl_CreateMonster(M_ParFactory* factory)
{
    return factory->createMonster();  //该虚函数扮演了多态new的行为
};

int main(int argc,char **argv)
{
    M_ParFactory *p_elm_fy = new M_ElementFactory();
    Monster *PM2 = Gbl_CreateMonster(p_elm_fy);

    delete p_elm_fy;
    delete PM2;
}

