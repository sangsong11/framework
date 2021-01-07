/*/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               创建：丁宋涛 夏曹俊，此代码可用作为学习参考                **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       :c++实战区块链核心密码学-基于openssl
** Contact       : xiacaojun@qq.com
**  博客   : http://blog.csdn.net/jiedichina
**	视频课程 : 网易云课堂	http://study.163.com/u/xiacaojun		
			   腾讯课堂		https://jiedi.ke.qq.com/				
			   csdn学院               http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961	
**             51cto学院              http://edu.51cto.com/lecturer/index/user_id-12016059.html	
** 			   老夏课堂		http://www.laoxiaketang.com 
**                 
**  c++实战区块链核心密码学-基于openssl   课程群 ：1064420127加入群下载代码和学员交流
**                           微信公众号  : jiedi2007
**		头条号	 : 夏曹俊
**
*****************************************************************************
//！！！！！！！！！c++实战区块链核心密码学-基于openssl 课程  QQ群：1064420127下载代码和学员交流*/

#pragma once
#include <string>
#include <openssl/des.h>
#include <string.h>
enum XSecType
{
    XDES_ECB,
    XDES_CBC,
    X3DES_ECB,
    X3DES_CBC,
    XAES128_ECB,
    XAES128_CBC,
    XAES192_ECB,
    XAES192_CBC, 
    XAES256_ECB,
    XAES256_CBC,
    XSM4_ECB,
    XSM4_CBC
};
/*
XSec sec;
sec.Init(XDES_ECB,"12345678",true);

*/
class XSec
{
public:

    /////////////////////////////////////////////////////////////////
    ///初始化加密对象，清理之前的数据
    ///@para type 加密类型
    ///@para pass 秘钥，可以是二进制
    ///@is_en true加密 false解密
    ///@return 是否成功
    virtual bool Init(XSecType type, const std::string& pass, bool is_en);

    /////////////////////////////////////////////////////////////////////
    /// 加解密数据
    ///@para in 输入数据
    ///@para in_size 数据大小
    ///@para 输出数据
    ///@para 成功返回加解密后数据字节大小，失败返回0
    virtual int Encrypt(const unsigned char* in, int in_size, unsigned char* out,bool is_end = true);

    /////////////////////////////////////////////////////////////////////
    /// 清理上下文和临时空间，值初始化
    virtual void Close();

private:

    ////////////////////////////////////////////////////////////////////////
    /// DES ECB模式加密
    int EnDesECB(const unsigned char* in, int in_size, unsigned char* out,bool is_end);
    
    ////////////////////////////////////////////////////////////////////////
    /// DES ECB模式解密
    int DeDesECB(const unsigned char* in, int in_size, unsigned char* out, bool is_end);

    ////////////////////////////////////////////////////////////////////////
    /// DES CBC模式加密
    int EnDesCBC(const unsigned char* in, int in_size, unsigned char* out, bool is_end);

    ////////////////////////////////////////////////////////////////////////
    /// DES CBC模式解密
    int DeDesCBC(const unsigned char* in, int in_size, unsigned char* out, bool is_end);
    
    ////////////////////////////////////////////////////////////////////////
    //DES算法秘钥
    DES_key_schedule ks_;

    //加密算法类型
    XSecType type_;

    bool is_en_;

    //数据块大小 分组大小
    int block_size_ = 0;

    //初始化向量
    unsigned char iv_[128] = { 0 };

    //加解密上下文
    void* ctx_ = 0;
};

