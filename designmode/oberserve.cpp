#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <iostream>
using namespace std;
class Fighter;
class Notifiter //通知其类
{
public:
     virtual void addToList(Fighter *player) = 0;   //把要被通知的玩家加入列表中
     virtual void removeFromList(Fighter *player)   = 0;  //把不想被通知的玩家从列表中取出
     virtual void notify(Fighter * talker,string tmpContent) = 0; //通知的一些细节
     virtual ~Notifiter(){}
};
class Fighter
{
public:
     Fighter(int tmpID,string tmpName):m_iPlyerID(tmpID),m_sPlayerName(tmpName)
     {
         m_iFamilyID = -1;
     }
     virtual ~Fighter() {}
public:
    void SetFamilyID(int tmpID)
    {
        m_iFamilyID = tmpID;
    }
    int GetFamilyID()
    {
        return m_iFamilyID;
    }
public:
    void SayWords(string tmpContent,Notifiter *notifyer)
    {
        notifyer->notify(this,tmpContent);
    }
    virtual void NotifyWords(Fighter* otherPlayer,string tmpContent)
    {
      cout << "玩家：" << otherPlayer->m_sPlayerName << "收到玩家：" << m_sPlayerName << "发送聊天信息："<< tmpContent <<endl;

    }
private:
     int m_iPlyerID; //玩家ID，全局唯一
     string m_sPlayerName; //玩家的名字
     int m_iFamilyID; //家族ID
};

class F_Warrior:public Fighter
{
public:
    F_Warrior(int tmpID,string tmpName):Fighter(tmpID,tmpName){}
};

class F_Mage:public Fighter
{
public:
   F_Mage(int tmpID,string tmpName):Fighter(tmpID,tmpName){}
};


//聊天信息通知类
class TalkNotifier:public Notifiter
{
public:
     //将玩家增加到家族列表中来
     virtual void addToList(Fighter *player)
     {
         int tmpfamilyid = player->GetFamilyID();
         if(tmpfamilyid != -1)
         {
            auto iter = m_familyList.find(tmpfamilyid);
            if(iter != m_familyList.end())
            {
                iter->second.push_back(player);
            }
            else
            {
                list<Fighter*> tmpplayerlist;
                m_familyList.insert(make_pair(tmpfamilyid,tmpplayerlist));
                m_familyList[tmpfamilyid].push_back(player);

            }
         }

     }
     virtual void removeFromList(Fighter *player)
     {
          int tmpfamilyid = player->GetFamilyID();
          if(tmpfamilyid != -1)
          {
              auto iter = m_familyList.find(tmpfamilyid);
              if(iter != m_familyList.end())
              {
                  m_familyList[tmpfamilyid].remove(player);
              }
          }
     }

     virtual void notify(Fighter * talker,string tmpContent)
     {
         int tmpfamilyid = talker->GetFamilyID();
         if(tmpfamilyid != -1)
         {
              auto itermap = m_familyList.find(tmpfamilyid);
              if(itermap != m_familyList.end())
              {
                for(auto iterlist = itermap->second.begin();iterlist != itermap->second.end();++iterlist)
                {
                    (*iterlist)->NotifyWords(talker,tmpContent);
                }
              }
         }
     }
private:
    map<int,list<Fighter *> >m_familyList;
};


int main(int argc,char **argv)
{
   Fighter* pplayerobj1 = new F_Warrior(10,"张三");
   pplayerobj1->SetFamilyID(100);
  
   Fighter* pplayerobj2 = new F_Warrior(20,"李四");
   pplayerobj2->SetFamilyID(100);

   Fighter* pplayerobj3 = new F_Mage(30,"王五");
   pplayerobj3->SetFamilyID(100);

   Fighter* pplayerobj4 = new F_Mage(50,"赵六");
   pplayerobj4->SetFamilyID(200);

   
   Notifiter *ptalknotify = new TalkNotifier();
   ptalknotify->addToList(pplayerobj1);
   ptalknotify->addToList(pplayerobj2);
   ptalknotify->addToList(pplayerobj3);
   ptalknotify->addToList(pplayerobj4);

   pplayerobj1->SayWords("--------jihe",ptalknotify);

   delete pplayerobj1;
   delete pplayerobj2;
   delete pplayerobj3;
   delete pplayerobj4;

   delete ptalknotify;
    return 0;
}
