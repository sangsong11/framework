#include <iostream>
using namespace std;
class Monster
{
public:
    virtual ~Monster() {}
    /*void Assemble(string strmodelno)  //参数：模型编号
    {
            LoadTrunkModel(strmodelno.substr(4,3));
            LoadHeadModel(strmodelno.substr(7,3));
            LoadLimbsModel(strmodelno.substr(10,3));
    }
    virtual void LoadTrunkModel(string strno) = 0;
    virtual void LoadHeadModel(string strno) = 0;
    virtual void LoadLimbsModel(string strno) = 0;*/
};

class M_Undead:public Monster
{
/*public:
    virtual void LoadTrunkModel(string strno)
     {
        cout << " LoadTrunkMode ---- M_Undead"<<endl;
     }
     virtual void LoadHeadModel(string strno)
     {
        cout << " LoadHeadMode ---- M_Undead"<<endl;
     }
     virtual void LoadLimbsModel(string strno)
     {
        cout << " LoadLimbsMode ---- M_Undead"<<endl;
     } */
};

class M_Element:public Monster
{
/*public:
    virtual void LoadTrunkModel(string strno)
     {
        cout << " LoadTrunkMode ---- M_Element"<<endl;
     }
     virtual void LoadHeadModel(string strno)
     {
        cout << " LoadHeadMode ---- M_Element"<<endl;
     }
     virtual void LoadLimbsModel(string strno)
     {
        cout << " LoadLimbsMode ---- M_Element"<<endl;
     }*/
};

class M_Mechanic:public Monster
{
/*public:
     virtual void LoadTrunkModel(string strno)
     {
        cout << " LoadTrunkMode ---- M_Mechanic"<<endl;
     }
     virtual void LoadHeadModel(string strno)
     {
        cout << " LoadHeadMode ---- M_Mechanic"<<endl;
     }
     virtual void LoadLimbsModel(string strno)
     {
        cout << " LoadLimbsMode ---- M_Mechanic"<<endl;
     }*/
};

//怪物构建器父类
class MonsterBuilder
{
public:
    virtual ~MonsterBuilder() {}
    /*void Assemble(string strmodelno)  //参数：模型编号
    {
            LoadTrunkModel(strmodelno.substr(4,3));
            LoadHeadModel(strmodelno.substr(7,3));
            LoadLimbsModel(strmodelno.substr(10,3));
    }*/
    Monster * GetResult()
    {
        return m_pMonster;
    }
    virtual void LoadTrunkModel(string strno) = 0;
    virtual void LoadHeadModel(string strno) = 0;
    virtual void LoadLimbsModel(string strno) = 0;
protected:
  Monster* m_pMonster;
};

class M_UndeadBuilder: public MonsterBuilder
{
public:
      M_UndeadBuilder()
      {
          m_pMonster = new M_Undead();
      }
     virtual void LoadTrunkModel(string strno)
     {
        cout << " LoadTrunkMode ---- M_UndeadBuilder"<<endl;
     }
     virtual void LoadHeadModel(string strno)
     {
        cout << " LoadHeadMode ---- M_UndeadBuilder"<<endl;
     }
     virtual void LoadLimbsModel(string strno)
     {
        cout << " LoadLimbsMode ---- M_UndeadBuilder"<<endl;
     }
};

class M_ElementBuilder: public MonsterBuilder
{
public:
      M_ElementBuilder()
      {
          m_pMonster = new M_Element();
      }
     virtual void LoadTrunkModel(string strno)
     {
        cout << " LoadTrunkMode ---- M_ElementBuilder"<<endl;
     }
     virtual void LoadHeadModel(string strno)
     {
        cout << " LoadHeadMode ---- M_ElementBuilder"<<endl;
     }
     virtual void LoadLimbsModel(string strno)
     {
        cout << " LoadLimbsMode ---- M_ElementBuilder"<<endl;
     }
};

class M_MechanicBuilder: public MonsterBuilder
{
public:
      M_MechanicBuilder()
      {
          m_pMonster = new M_Mechanic();
      }
     virtual void LoadTrunkModel(string strno)
     {
        cout << " LoadTrunkMode ---- M_MechanicBuilder"<<endl;
     }
     virtual void LoadHeadModel(string strno)
     {
        cout << " LoadHeadMode ---- M_MechanicBuilder"<<endl;
     }
     virtual void LoadLimbsModel(string strno)
     {
        cout << " LoadLimbsMode ---- M_MechanicBuilder"<<endl;
     }
};

//指挥者类
class MonsterDirector
{
public:
    MonsterDirector(MonsterBuilder* ptmpBuilder)
    {
        m_pMonsterBuilder = ptmpBuilder;
    }
    void SetBuider(MonsterBuilder* ptmpBuilder)
    {
        m_pMonsterBuilder = ptmpBuilder;
    }
     Monster* Construct(string strmodelno)  //参数：模型编号
    {
           m_pMonsterBuilder->LoadTrunkModel(strmodelno.substr(4,3));
           m_pMonsterBuilder->LoadHeadModel(strmodelno.substr(7,3));
           m_pMonsterBuilder->LoadLimbsModel(strmodelno.substr(10,3));
            return m_pMonsterBuilder->GetResult();
    }
private:
   MonsterBuilder* m_pMonsterBuilder;    
};


int main(int argc,char **argv)
{
     MonsterBuilder *pMonsterBuilder = new M_UndeadBuilder();  
     MonsterDirector *pDirector = new MonsterDirector(pMonsterBuilder);
     Monster *pMonster = pDirector->Construct("1253679201254");

    delete  pMonster;
    delete  pDirector;
    delete  pMonsterBuilder;
    return 0;
}