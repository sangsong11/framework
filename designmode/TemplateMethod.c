#include <iostream>
 

using namespace std;

// 战斗者父类
class Fighter
{
   public:
          Fighter(int life,int magic,int attack):m_life(life),m_magic(magic),m_attack(attack) {} 
          virtual ~Fighter(){}  //做父类，析构函数为虚函数 
          void JN_Burn() //模版方法
          {
            if(canUseJN == false)
               return ;
             effect_enemy();  //对敌人产生的影响
             effect_self();   //对猪脚自身产生的影响
             play_effect();   //播放燃烧的技能特效
          }
   private:
          virtual void effect_enemy(){} //函数体为空，表示啥也不做，如果实现要求必须在子类中重新实现
          virtual void effect_self(){}  //
          void play_effect()
          {
              cout<<"播放技能燃烧特效给玩家看"<<endl;
          }
          virtual bool canUseJN() = 0;  //纯虚函数
   protected:
          int  m_life;
          int  m_magic;
          int  m_attack;
}

//战士类
class F_Warrior:public Fighter
{
   public:
          F_Warrior(int life,int magic,int attack):Fighter(life,magic,attack)
          {
               
          }

   private:
          virtual void effect_enemy() //函数体为空，表示啥也不做，如果实现要求必须在子类中重新实现
          {
              cout<<"战士猪脚让所用敌人每人失去500点生命......"<<endl;
          }
          virtual void effect_self()
          {
              cout<<"战士猪脚失去300点生命值"<<endl;
              m_life -=300;
          }  
          virtual bool canUseJN()
          {
              if(m_life < 300)
                 return flase;
              return true;
          }
  
};

// 法师
class F_Mage:public Fighter
{
   public:
          F_Mage(int life,int magic,int attack):Fighter(life,magic,attack)
          {
               
          }

   private:
          virtual void effect_enemy() //函数体为空，表示啥也不做，如果实现要求必须在子类中重新实现
          {
              cout<<"法师猪脚让所用敌人每人失去650点生命......"<<endl;
          }
          virtual void effect_self()
          {
             cout<<"法师猪脚失去100点魔法值"<<endl;
              m_magic -=100;
          }  
          virtual bool canUseJN()
          {
              if(m_magic < 100)
                 return flase;
              return true;
          }
  
};
int main(int argc,char **argv)
 {
    
     Fighter *prole_war = new F_Warrior(1000,0,200);
     prole_war->JN_Burn();
     cout<<"-------------------------"<<endl;
     Fighter *prole_mag = new F_Mage(800,200,300);
     prole_mag->JN_Burn();

     delete prole_war;
     delete prole_mag;
     return 0;
 }