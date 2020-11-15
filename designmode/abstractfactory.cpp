#include <iostream>
#include <stdlib.h>
using namespace std;

//怪物类
class Monster
{
  public:
        Monster(int life,int magic,int attack):m_life(life),m_magic(magic),m_attack(attack){}
        virtual ~Monster()  {}
   protected:
        int m_life;
        int m_magic;
        int m_attack;
};


//沼泽亡灵类怪物
class M_Undead_Swamp:public Monster
{
    public:
          M_Undead_Swamp(int life,int magic,int attack):Monster(life,magic,attack)
          {
              cout<< "一个沼泽亡灵类怪物来到了这个世界"<<endl;
          }
};

//沼泽元素类怪物
class M_Element_Swamp:public Monster
{
    public:
          M_Element_Swamp(int life,int magic,int attack):Monster(life,magic,attack)
          {
              cout<< "一个沼泽元素类怪物来到了这个世界"<<endl;
          }
};

//沼泽机械类怪物
class M_Mechanic_Swamp:public Monster
{
    public:
          M_Mechanic_Swamp(int life,int magic,int attack):Monster(life,magic,attack)
          {
              cout<< "一个沼泽机械类怪物来到了这个世界"<<endl;
          }
};


//山脉亡灵类怪物
class M_Undead_Mountain:public Monster
{
    public:
          M_Undead_Mountain(int life,int magic,int attack):Monster(life,magic,attack)
          {
              cout<< "一个山脉亡灵类怪物来到了这个世界"<<endl;
          }
};

//山脉元素类怪物
class M_Element_Mountain:public Monster
{
    public:
          M_Element_Mountain(int life,int magic,int attack):Monster(life,magic,attack)
          {
              cout << "一个山脉元素类怪物来到了这个世界"<<endl;
          }
};

//山脉机械类怪物
class M_Mechanic_Mountain:public Monster
{
    public:
          M_Mechanic_Mountain(int life,int magic,int attack):Monster(life,magic,attack)
          {
              cout<< "一个山脉机械类怪物来到了这个世界"<<endl;
          }
};


//城镇亡灵类怪物
class M_Undead_Town:public Monster
{
    public:
          M_Undead_Town(int life,int magic,int attack):Monster(life,magic,attack)
          {
              cout<< "一个城镇亡灵类怪物来到了这个世界"<<endl;
          }
};

//城镇元素类怪物
class M_Element_Town:public Monster
{
    public:
          M_Element_Town(int life,int magic,int attack):Monster(life,magic,attack)
          {
              cout<< "一个城镇元素类怪物来到了这个世界"<<endl;
          }
};

//城镇机械类怪物
class M_Mechanic_Town:public Monster
{
    public:
          M_Mechanic_Town(int life,int magic,int attack):Monster(life,magic,attack)
          {
              cout<< "一个城镇机械类怪物来到了这个世界"<<endl;
          }
};

//所有工厂类的父类
class M_PartFactory
{
    public:
        virtual Monster *createMonster_Undead() = 0;    //创建亡灵类怪物
        virtual Monster *createMonster_Element() = 0;   //创建元素类怪物
        virtual Monster *createMonster_Mechanic() = 0;    //创建机械类怪物
        virtual ~M_PartFactory() {}     //做父类时析构函数应该为虚函数
};

// 沼泽地区工厂
class M_Factory_Swamp:public M_PartFactory
{
    public:
        virtual Monster *createMonster_Undead() 
        {
            return new M_Undead_Swamp(300,50,120);    //创建沼泽亡灵类怪物
        }
        virtual Monster *createMonster_Element() 
        {
            return new M_Element_Swamp(200,80,110);    //创建沼泽元素类怪物
        }
        virtual Monster *createMonster_Mechanic() 
        {
            return new M_Mechanic_Swamp(400,0,90);    //创建沼泽机械类怪物
        }
        
};

// 山脉地区工厂
class M_Factory_Mountain:public M_PartFactory
{
    public:
        virtual Monster *createMonster_Undead() 
        {
            return new M_Undead_Mountain(300,50,80);    //创建山脉亡灵类怪物
        }
        virtual Monster *createMonster_Element() 
        {
            return new M_Element_Mountain(200,80,100);    //创建山脉元素类怪物
        }
        virtual Monster *createMonster_Mechanic() 
        {
            return new M_Mechanic_Mountain(600,0,110);    //创建山脉机械类怪物
        }
        
};

// 城镇地区工厂
class M_Factory_Town:public M_PartFactory
{
    public:
        virtual Monster *createMonster_Undead() 
        {
            return new M_Undead_Town(300,50,80);    //创建城镇亡灵类怪物
        }
        virtual Monster *createMonster_Element() 
        {
            return new M_Element_Town(200,80,100);    //创建城镇元素类怪物
        }
        virtual Monster *createMonster_Mechanic() 
        {
            return new M_Mechanic_Town(400,0,110);    //创建城镇机械类怪物
        }
        
};


int main(int argc,char **argv)
{
  
    M_PartFactory *p_mou_fy = new M_Factory_Mountain();
    Monster *Pm1 =  p_mou_fy->createMonster_Mechanic(); //创建山脉丢的元素类怪物
    
    M_PartFactory *p_twn_fy = new M_Factory_Town();
    Monster *Pm2 =  p_twn_fy->createMonster_Undead(); //创建山脉丢的元素类怪物
    Monster *Pm3 =  p_twn_fy->createMonster_Mechanic();
    
    delete  p_mou_fy;
    delete  p_twn_fy;
    delete  Pm1;
    delete  Pm2;
    delete  Pm3;
    return 0;
}
