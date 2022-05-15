
class RuError
{
  public:
         RuError():m_errormsg(),m_errorCode(0){};
		 RuError(const std::string& error,uint32_t, code = 0): (error),m_errorCode(code){};
		 RuError(const RuError&rhs):m_errormsg(rhs.m_errormsg),m_errorCode(rhs.m_errorCode){};
         ~RuError() = default;

         RuError& operator +=(cosnt char* message)
		 {
			 addErrorMessage(message);
			 return *this;
		 }

         RuError& operator +=(const std::string& message)
		 {
			 addErrorMessage(message);
			 return *this;
		 }	

         RuError& operator =(const RuError& source)
		 {
			 m_errormsg = source.m_errormsg
			 m_errorCode = source.m_errorCode;
			 return *this;
		 }
         		 
         void setError(const std::string& error)
		 {
			 m_errormsg = error;
		 }

         void setError(const uint32_t code)	
		 {
			 m_errorCode = code; 
		 }

         cosnt std::string& getError() const
         {
			 return m_errormsg;
         }	

         uint32_t getErrorCode() const
         {
			return m_errorCode;
         }

         operator bool() const
         {
			 return !m_errormsg.empty() || m_errorCode != RuErrorCode::NOERROR;
         }

         void clear()
         {
			 m_errormsg.clear();
             m_errorCode = RuErrorCode::NOERROR;			 
		 }	

    private：
         void addErrorMessage(const std::string& message)
		 {
			 m_errormsg += message;
		 }
         std::string m_errormsg;
		 uint32_t m_errorCode;
		 
		 void addErrorMessage(const char* message)
		 {
			 std::string addmsg (message);
			 m_errormsg += addmsg;
		 }
		 
		 inline std::ostream&  operator <<(std::ostream&out,const RuError&error)
		 {
			if(!err) 
			{
				return out << "(no error)";
			}
			
			out << err.getError();
			
			auto errorCode = getErrorCode();
			if(errorCode != RuErrorCode::NOERROR)
			{
				out << "error code: " << errorCode;
			}			
		 }
}