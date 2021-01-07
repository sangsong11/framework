/*/*******************************************************************************
**                                                                            **
**                     Jiedi(China nanjing)Ltd.                               **
**	               ������������ �Ĳܿ����˴��������Ϊѧϰ�ο�                **
*******************************************************************************/

/*****************************FILE INFOMATION***********************************
**
** Project       :c++ʵս��������������ѧ-����openssl
** Contact       : xiacaojun@qq.com
**  ����   : http://blog.csdn.net/jiedichina
**	��Ƶ�γ� : �����ƿ���	http://study.163.com/u/xiacaojun		
			   ��Ѷ����		https://jiedi.ke.qq.com/				
			   csdnѧԺ               http://edu.csdn.net/lecturer/lecturer_detail?lecturer_id=961	
**             51ctoѧԺ              http://edu.51cto.com/lecturer/index/user_id-12016059.html	
** 			   ���Ŀ���		http://www.laoxiaketang.com 
**                 
**  c++ʵս��������������ѧ-����openssl   �γ�Ⱥ ��1064420127����Ⱥ���ش����ѧԱ����
**                           ΢�Ź��ں�  : jiedi2007
**		ͷ����	 : �Ĳܿ�
**
*****************************************************************************
//������������������c++ʵս��������������ѧ-����openssl �γ�  QQȺ��1064420127���ش����ѧԱ����*/

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
    ///��ʼ�����ܶ�������֮ǰ������
    ///@para type ��������
    ///@para pass ��Կ�������Ƕ�����
    ///@is_en true���� false����
    ///@return �Ƿ�ɹ�
    virtual bool Init(XSecType type, const std::string& pass, bool is_en);

    /////////////////////////////////////////////////////////////////////
    /// �ӽ�������
    ///@para in ��������
    ///@para in_size ���ݴ�С
    ///@para �������
    ///@para �ɹ����ؼӽ��ܺ������ֽڴ�С��ʧ�ܷ���0
    virtual int Encrypt(const unsigned char* in, int in_size, unsigned char* out,bool is_end = true);

    /////////////////////////////////////////////////////////////////////
    /// ���������ĺ���ʱ�ռ䣬ֵ��ʼ��
    virtual void Close();

private:

    ////////////////////////////////////////////////////////////////////////
    /// DES ECBģʽ����
    int EnDesECB(const unsigned char* in, int in_size, unsigned char* out,bool is_end);
    
    ////////////////////////////////////////////////////////////////////////
    /// DES ECBģʽ����
    int DeDesECB(const unsigned char* in, int in_size, unsigned char* out, bool is_end);

    ////////////////////////////////////////////////////////////////////////
    /// DES CBCģʽ����
    int EnDesCBC(const unsigned char* in, int in_size, unsigned char* out, bool is_end);

    ////////////////////////////////////////////////////////////////////////
    /// DES CBCģʽ����
    int DeDesCBC(const unsigned char* in, int in_size, unsigned char* out, bool is_end);
    
    ////////////////////////////////////////////////////////////////////////
    //DES�㷨��Կ
    DES_key_schedule ks_;

    //�����㷨����
    XSecType type_;

    bool is_en_;

    //���ݿ��С �����С
    int block_size_ = 0;

    //��ʼ������
    unsigned char iv_[128] = { 0 };

    //�ӽ���������
    void* ctx_ = 0;
};

