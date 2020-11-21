#include <iostream>
#include <map>
using namespace std;


class absState{
    public:
       virtual void show() = 0;
};

class buildingState:public absState{
    public:
          virtual void show()
          {
              cout<<"建筑特效"<<endl;
          }
};

class fireState:public absState{
    public:
         virtual void show()
         {
             cout<<"受伤着火"<<endl;
         }
};

class productionState:public absState{
    public:
       virtual void show()
       {
            cout<<"生产白光"<<endl;
       }
};

class upGradeState:public absState{
    public:
        virtual void show()
        {
            cout<<"升级蓝光"<<endl;
        }
};

class repairState:public absState
{
    public:
    virtual void show()
    {
        cout<<"修复绿光"<<endl;
    }
};

enum StateType
{
    BUILDING,INJURED,REPAIR,UPGRADE,PRODUCTION
};

class absBuilding
{
    public:
        absBuilding(char *name):mName(name)
        {
        }
        virtual void show()
        {
            cout << mName<<"表现："<<endl;
            map<StateType,absState*>::iterator beg = mStates.begin();
            for ( ; beg != mStates.end(); beg++)
            {
              beg->second->show();
            }
           cout<<endl;
        };
        void eraseState(StateType type)  //实现删除一个特效
        {
          auto result = mStates.find(type);
          if(result != mStates.end())
             mStates.erase(result);
        }

     protected:
          char* mName;
          map<StateType, absState*> mStates;

};

class concreteBuilding:public absBuilding
 {
 public:
     concreteBuilding(char* name):absBuilding(name)
     {
        mStates[BUILDING] = new buildingState();
     }
     void upGrade()
     {
        auto result = mStates.find(UPGRADE);
         if(result == mStates.end())
         {
             mStates[UPGRADE] = new upGradeState();
         }
         eraseState(BUILDING);//升级的时候必须是建造过程已经结束，所以需要删掉“建筑”状态
         eraseState(PRODUCTION);//升级的时候不能进行生产
     }
     void production()
     {
         auto result = mStates.find(PRODUCTION);
         if(result == mStates.end())
         {
             mStates[PRODUCTION] = new productionState();
         }
         eraseState(BUILDING);//生产的时候必须已经建造完成
         eraseState(UPGRADE);//生产的时候不能升级
     }
     void injured()
     {
         auto result = mStates.find(INJURED);
         if(result == mStates.end())
         {
             mStates[INJURED] = new fireState();
         }
    }
     void repair()
     {
         auto injuredVal = mStates.find(INJURED);
         if(injuredVal == mStates.end())
             return;
         auto result = mStates.find(REPAIR);
         if(result == mStates.end())
         {
             mStates[REPAIR] = new repairState();
         }
    }
 };

class SiegeTank;
 
 int main()
 {
    concreteBuilding cb("兵营");
    cb.production();
    cb.injured();
    cb.repair();
    cb.show();
    return 0;
 }
