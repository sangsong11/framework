#include <iostream>


using namespace std;
//通过一个对象克隆出多个一模一样的对象

//怪物类
class Monster
{
    public:
        Monster(int life,int magic,int attack):m_life(life),m_magic(magic),m_attack(attack){}
        virtual ~Monster()  {}
    public:
          virtual Monster* clone() = 0;
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
        M_Undead(const M_Undead& tmpobj):Monster(tmpobj)
        {
             cout<<"M_Undead"<<endl;
        }
        virtual Monster* clone()
        {
             return new M_Undead(*this);
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
        M_Element(const M_Element& tmpobj):Monster(tmpobj)
        {
             cout<<"M_Element"<<endl;
        }
        virtual Monster* clone()
        {
             return new M_Element(*this);
        }
        ~M_Element() {}
};

//机械类怪物
class M_Mechanic:public Monster
{
    public:
        M_Mechanic(int life,int magic,int attack):Monster(life,magic,attack)
        {
            cout<<"一个机械类怪物来到了这个世界"<<endl;
        }
        M_Mechanic(const M_Mechanic& tmpobj):Monster(tmpobj)
        {
             cout << "M_Mechain" <<endl;
        }
        virtual Monster* clone()
        {
                return new M_Mechanic(*this);
        }
        ~M_Mechanic(){}
};

int main(int argc, char* argv[])
{
   M_Mechanic mypro(400,0,100);
   Monster *pmy = new M_Element(200,80,100);
   //copy
   Monster* pclon = mypro.clone();
   Monster* pclon2 = pmy->clone();

   delete pmy;
   delete pclon;
   delete pclon2;

}
