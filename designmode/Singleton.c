#include <iostream>

class Singleton
{
  private:
     Singleton() {}
  private:
       static Singleton* m_instance;
  public:
        static Singleton* GetInstance()
        {
          if(m_instance == NULL)
          {
            if(m_instance == NULL)
            {
                m_instancee = new Singleton();
                static CGarhuishou cl;
            }
            return m_instance;
          }
        }
       class CGarhuishou
       {
           public:
                 ~CGarhuishou()
                 {
                     if(Singleton::m_instance)
                     {
                         delete Singleton::m_instance;
                         Singleton::m_instance == NULL;
                     }
                 }
       };
    
    void func()
    {
        cout << "测试" <<endl;
    }
};

Singleton* Singleton::m_instance = NULL;

int main(int argc,char **argv)
{
    Singleton *p_a = Singleton::GetInstance();
    p_a->func();
    // Singleton::GetInstance()->func();
    delete Singleton::m_instance;
    return 0;
}