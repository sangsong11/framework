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

class MonsterFactory
{
   public:
        Monster* createMonster(string strmontype)
        {
            Monster * prtnobj = nullptr;
            if(strmontype == "udd")  //创建亡灵类怪物
            {
                prtnobj = new M_Undead(300,50,80);
            } 
            else if(strmontype == "ele") //创建元素类怪物
            {
                prtnobj = new M_Element(200,80,100);
            }
            else if(strmontype == "mec")
            {
                prtnobj = new M_Mechanic(400,0,110);
            }
            return prtnobj;
        }
}；

int main(int argc,char **argv)
{
    MonsterFactory facobj;
    Monster * pM1 = facobj.createMonster("udd");
    Monster * pM2 = facobj.createMonster("ele");
    Monster * pM2 = facobj.createMonster("mec");

    delete pM1;
    delete pM2;
    delete pM3;
    return 0;
}