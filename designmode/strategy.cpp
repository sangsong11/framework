#include <iostream>
using namespace std;
class ItemStrategy;

class Fighetr
{
public:
     Fighetr(int life,int magic,int attack):m_life(life),m_magic(magic),m_attack(attack){}
     virtural ~Fighetr() {}
public:
     void SetItemStrategy(ItemStrategy* strategy)
     {
        itemstrategy =  strategy;
     }
     void UseItem()
     {
         itemstrategy->UseItem(this);
     }
     int GetLife()
     {
         return m_life;
     }
     void SetLife(int life)
     {
         m_life = life;
     }
protected:
     int m_life;
     int m_magic;
     int m_attack;
     ItemStrategy* itemstrategy = nullptr;
};

class F_Warrior:public Fighetr
{
   F_Warrior(int life,int magic,int attack):Fighetr(int life,int magic,int attack){}
};

class F_Mage:public Fighetr
{
   F_Mage(int life,int magic,int attack):Fighetr(int life,int magic,int attack){}
};

class ItemStrategy
{
public:
    virtual void UseItem(Fighetr *mainobj) = 0;
    virtual ~ItemStrategy() {}
};

class ItemStrategy_BXD:public ItemStrategy
{
public:
     virtual void UseItem(Fighetr *mainobj)
     {
         mainobj->SetLife(mainobj->GetLife() + 200);
     }
};

class ItemStrategy_DHD:public ItemStrategy
{
public:
     virtual void UseItem(Fighetr *mainobj)
     {
         mainobj->SetLife(mainobj->GetLife() + 300);
     }
};

class ItemStrategy_SHD:public ItemStrategy
{
public:
     virtual void UseItem(Fighetr *mainobj)
     {
         mainobj->SetLife(mainobj->GetLife() + 500);
     }
};

int main(int argc,char **argv)
{

   Fighetr* prole_war = new F_Warrior(1000,0,200);
   ItemStrategy *strategy = new ItemStrategy_BXD();
   prole_war->SetItemStrategy(strategy);
   prole_war->UseItem();


   delete strategy;
   delete prole_war;
    return 0;
}

