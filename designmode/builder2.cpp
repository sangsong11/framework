#include<iostream>
#include<map>
#include<vector>
#include<sstream>
class DailyHeaderData
{
    private:
          string m_strDepName;
          string m_strGenData;
    public:
         DailyHeaderData(string strDepName,string strGenData):m_strDepName(strDepName),m_strGenData(strGenData)
         {

         }
         string getDepName()
         {
             return m_strDepName;
         }
         string getExportDate()
         {
            return m_strGenData;
         }
};

class DailyContentData
{
    public:
         DailyContentData(string strContent,double dspendTime):m_strContent(strContent),m_dspendTime(dspendTime)
         {

         }
         string getContent()
         {
             return m_strContent;
         }
         string getSpendTime()
         {
             return m_dspendTime;
         }
    private:
        string m_strContent;
        double m_dspendTime;
};

class DailyFooterData
{
    public:
           DailyFooterData(string strUserName):m_strUserName(strUserName)
           {

           }
           string getUserName()
           {
               return m_strUserName;
           }
    private:
         string m_strUserName;
};

//抽象构建起类（文件构建器父类）
class FileBuilder
{
    public:
        virtual ~FileBuilder() {}
    public:
        virtual void buildHeader(DailyHeaderData& dailyheaderobj) = 0;
        virtual void buildBody(vector<DailyContentData*>& vec_dailycontentobj) = 0;
        virtual void buildFooter(DailyFooterData& dailyFooterobj) = 0;
        string GetResult()
        {
            return m_strResult;
        }
    protected:
        string m_strResult;
};

class TxtFileBuilder:public FileBuilder
{
     public:
         virtual void buildHeader(DailyHeaderData& dailyheaderobj)
         {
            m_strResult +=  dailyheaderobj.getDepName() + "," + dailyheaderobj.getExportDate() + "\n";
         }
         virtual void buildBody(vector<DailyContentData*>& vec_dailycontentobj)
         {
             for(auto iter = vec_dailycontentobj.begin();iter != vec_dailycontentobj.end();++iter)
             {
                 ostringstream oss;
                 oss << (*iter)->getSpendTime();
                 m_strResult += (*iter)->geetContent() + ":(花费的时间："+oss.str()+"小时)" + "\n";
             }
         }
         virtual void buildFooter(DailyFooterData& dailyFooterobj)
         {
             m_strResult += "报告人：" + dailyFooterobj.getUserName() + "\n";
         }
};

class XmlFileBuilder:public FileBuilder
{
    public:
         virtual void buildHeader(DailyHeaderData& dailyheaderobj)
         {
            m_strResult += "<?xml version =\"1.0\" encoding=\"UTF-8\" ?>\n";
            m_strResult += "<DailyReport>\n";
            m_strResult += "    <Header>\n";
            m_strResult += "       <DepName>" + dailyheaderobj.getDepName() + "</DepName>\n";
            m_strResult += "       <GenDate>" + dailyheaderobj.GenDate()   +   "</GenDate>\n";
            m_strResult += "    </Header>\n";
         }
         virtual void buildBody(vector<DailyContentData*>& vec_dailycontentobj)
         {
             m_strResult += "    <Body>\n";
             for(auto iter = vec_dailycontentobj.begin();iter != vec_dailycontentobj.end();++iter)
             {
                 ostringstream oss;
                 oss << (*iter)->getSpendTime();
                 m_strResult += "    <Content>" + (*iter)->getContent() + "</Content>\n";
                 m_strResult += "    <SpendTimee>花费的时间：" + oss.str() + "小时" +"</SpendTime>\n";
             }
            m_strResult += "   </Body>\n";
         }
         virtual void buildFooter(DailyFooterData& dailyFooterobj)
         {
             m_strResult += "  <Footer>\n";
             m_strResult += "       <UserName>报告人：" + dailyFooterobj.getUserName() + "</UserName>\n";
             m_strResult += "   </Footer>\n";
             m_strResult += "</DailyReport>\n";;
         }
};

class FileDirector
{
   public:
        FileDirector(FileBuilder *ptmpBuilder)
        {
               m_pFileBuilder = ptmpBuilder;
        }
        string Construct(DailyHeaderData& dailyheaderobj,vector<DailyContentData*>& vec_dailycontentobj,DailyFooterData& dailyFooterobj)
        {
            m_pFileBuilder->buildHeader(dailyheaderobj);
            m_pFileBuilder->buildBody(vec_dailycontentobj);
            m_pFileBuilder->buildFooter(dailyFooterobj);
            return m_pFileBuilder->GetResult();
        }

    private:
        FileBuilder *m_pFileBuilder;
};

int main(int argc,char **argv)
{
   DailyHeaderData* pdhd = new DailyHeaderData("研发一部", "11月1日");
   DailyContentData *pdcd1 = new DailyContentData("完成A项目的需求分析工作",3.5);
   DailyContentData *pdcd2 = new DailyContentData("确定A项目开发的使用工具",4.5);

   vector<DailyContentData*> vec_dcd;
   vec_dcd.push_back(pdcd1);
   vec_dcd.push_back(pdcd2;

   DailyFooterData* pdfd = new DailyFooterData("小李");

   FileBuilder* pfd = new TxtFileBuilder();
   FileDirector *pDtr = new FileDirector(pfd);

   count << pDtr->Construct(*pdhd,vec_dcd,*pdfd);
   delete pdhd;

   for(auto iter = vec_dcd.begin(); iter != veec_dcd.end();++iter)
   {
       delete (*iter);
   }
   delete pdfd;
   delete pfd;
   delete pDtr;
}