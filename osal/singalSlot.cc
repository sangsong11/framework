#define SLOT1(resultType,parameterType,classType,callback)\
                new CommFW::Slot1<resultType,parameterType,classType>(this,&classType::callback)
				
#define SLOT0(resultType,classType,callback)\
                new CommFW::Slot0<resultType,classType>(this,&classType::callback)
				
namespace CommFW{
     
  template<class Tresult,class Tparam>
  class BaseSlot1
  {
	 public:
          virtual ~BaseSlot1() {}
		  virtual Tresult emit(Tparam p_param)= 0;
  };
  
  template<class Tresult>
  class BaseSlot0
  {
	 public:
          virtual ~BaseSlot0() {}
		  virtual Tresult emit(void)= 0;
  };
  
  
  template<class Tresult,class Tparam>
  class Slot0: puclic BaseSlot0<Tresult>
  {
	 public:
          typedef Tresult(Tistance::*Callback)(void);
		  
		  Slot0(Tistance *p_instance,Callback p_function)
		          :m_instance(p_instance),
				  :m_function(p_function)
		  {
			   
		  }
		  virtual ~Slot0(){}  
		  Tresult emit(void)
		  {
			return ((n_instance->*m_function)()); 
		  }
	 private:
	      Slot0(const Slot0 &);
		  Slot0 operator=(const Slot0 &);
		  Tistance *m_instance;
		  Callback m_function;
  };
  
  template<class Tresult,class Tparam,class Tistance>
  class Slot1: puclic BaseSlot1<Tresult,Tparam>
  {
	  public:
	     typedef Tresult(Tistance::*Callback)(Tparam);
		 
      	 Slot1(Tistance *p_instance,Callback p_function)
		          :m_instance(p_instance),
				  :m_function(p_function)
		 {
			   
		 }	

         ~Slot1() override
		 {
			 
		 }	
		 
		 Tresult emit(void)
		 {
			return ((m_instance->*m_function)()); 
		 }
		 
	  private:
	      Slot1(const Slot1 &);
		  Slot1 operator=(const Slot1 &);
		  Tistance *m_instance;
		  Callback m_function;
  };
  
   template<typename Functor>
   class Sigal
   {
	   public:
	       using ClientHandle = long long;
           Sigal() = default;	//生成默认的构造函数	

           Sigal(const Sigal & sig)
		   {
			   Mutex lock(sig.m_mutex);
			   m_functions = sig.m_functions;
		   }
		   Singal& operator=(const Sigal& rhs)
		   {
			    MutexLock lock(mutex);
				MutexLock lockRhs(rhs.mutex);
				m_functions = rhs.m_functions;
				m_previousClientHandle = rhs.m_previousClientHandle;
		   }
		   
		   ClientHandle connect(const Functor &func)
		   {
			    MutexLock lock(mutex);
				ClientHandle clientHandle = ++m_previousClientHandle;
				m_functions[ClientHandle] = func;
				return clientHandle;
		   }
		   
		   void disconnect(const ClientHandle& clientHandle)
		   {
			   MutexLock lock(mutex);
			   m_functions.erase(clientHandle);
		   }
		   
		   template<typename ...args>
		   void emit(Args ...args) const 
		   {
			   Functor functions;
			   {
				   MutexLock lock(mutex);
				   functions = m_functions;
			   }
			   
			   for(const auto& funcPair: functions)
			   {
				   funcPair.send(args...);
			   }
		   }
		   
		   template<typename ... Args>
		   void operator()(Args ... args)
           {
			   emit(args...);
           }	

           size_t size() const
           {
			   MutexLock(m_mutex);
			   return m_functions.size();
		   }

      private:
           using Functions = std::map<ClientHandle,Functor>;
           Functions m_functions;
           mutable Mutex m_mutex;
           ClientHandle	 m_previousClientHandle{0};  
   };
   
   
   template<class Tresult>
   class Signal0
   {
	   public:
	       Signal0(){}
		   virtual ~Signal0()
		   {
			   for(u32 i = 0;i < m_slot.size();i++)
			   {
				   delete m_slot.at(i);
			   }
			   m_slot.clear();
		   }
		   
		   void emit(void)
		   {
			    BaseSlot0<Tresult>* p_slot;
				m_slot_mutex.raw_lock_only_special_use();
				for(uint_32 i = 0 ;i < m_slot.size();i++)
				{
					 p_slot = m_slot[i];
					 m_slot_mutex.raw_unlock_only_special_use();
					 p_slot->emit();
					 m_slot_mutex.raw_lock_only_special_use();
				}
				m_slot_mutex.raw_unlock_only_special_use();
		   }
		   
		   void connect(BaseSlot0<Tresult>* p_slot)
		   {
			   MutexLock(m_slot_mutex);
			   m_slot.push_back(p_slot);
		   }
		   
		   size_t size()
		   {
			   MutexLock(m_slot_mutex);
			   return m_slot.size();
		   }
		   
		 private:
		    Signal0(const Signal0&);
			Signal0 operator=(const Signal0&);
			
			std::vector<BaseSlot0< Tresult>* > m_slot;
			Mutex m_slot_mutex;
   };
   
   template<class Tid,class Tparam>
   class Slot1Map
   {
	  public:
	      Slot1Map(){}
		  virtual ~Slot1Map()
		  {
			  typename std::map<Tid,BaseSlot1< voud,Tparam>*>:: const_iterator pos;
			  for(pos = m_map.begin();pos != m_map.end();++pos)
			  {
				  delete pos->second;
			  }
		  }
		  
		 void emit()
		 {
			 typename std::map<Tid,BaseSlot1< voud,Tparam>*>:: const_iterator pos = m_map.find(id);
			 if(pos != m_map.end())
			 {
				pos->second->emit(p_param); 
			 }
		 }	

        void connect(Tid id,BaseSlot1< voud,Tparam>*p_slot)
		{
			m_map.insert(std:make_pair(id,p_slot));
        }

      private:
         Slot1Map(const Slot1Map&);
         Slot1Map operator=(const Slot1Map& );

         std::map<Tid,BaseSlot1< voud,Tparam>*>	m_map;	 
   };
   
}	
	