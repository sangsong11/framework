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

#include "xsec.h"
#include <openssl/evp.h>
#include <openssl/err.h>
#include<iostream>
using namespace std;
void XSec::Close()
{   
    //��ʼ��iv_
    memset(iv_, 0, sizeof(iv_));
    if (ctx_)
    {
        EVP_CIPHER_CTX_free((EVP_CIPHER_CTX*)ctx_);
        ctx_ = nullptr;
    }
    
}

/////////////////////////////////////////////////////////////////
///��ʼ�����ܶ�������֮ǰ������
///@para type ��������
///@para pass ��Կ�������Ƕ�����
///@is_en true���� false����
///@return �Ƿ�ɹ�
bool XSec::Init(XSecType type, const std::string& pass, bool is_en)
{
    Close();
    this->type_ = type;
    this->is_en_ = is_en;
    //��Կ��ȫ����
    unsigned char key[32] = { 0 }; //�ٵĲ���0
    int key_size = pass.size();

    //�ӽ����㷨
    const EVP_CIPHER* cipher = 0;

    switch (type)
    {
    case XDES_ECB:
    case XDES_CBC:
        block_size_ = DES_KEY_SZ;
        ///������ԣ�����8�ֽڵĶ������ٵĲ���0
        //����8�ֽڵĶ�����
        if (key_size > block_size_) key_size = block_size_;
        //�ٵĲ���0
        memcpy(key, pass.data(), key_size);
        DES_set_key((const_DES_cblock*)key, &ks_);
        return true;
    
    case X3DES_ECB:
        cipher = EVP_des_ede3_ecb();
        break;
    case X3DES_CBC:
        cipher = EVP_des_ede3_cbc();
        break;
    case XAES128_ECB:
        cipher = EVP_aes_128_ecb();
        break;
    case XAES128_CBC:
        cipher = EVP_aes_128_cbc();
        break;
    case XAES192_ECB:
        cipher = EVP_aes_192_ecb();
        break;
    case XAES192_CBC:
        cipher = EVP_aes_192_cbc();
        break;
    case XAES256_ECB:
        cipher = EVP_aes_256_ecb();
        break;
    case XAES256_CBC:
        cipher = EVP_aes_192_cbc();
        break;
    case XSM4_ECB:
        cipher = EVP_sm4_ecb();
        break;
    case XSM4_CBC:
        cipher = EVP_sm4_cbc();
        break;
    default:
        break;
    }

    if (!cipher)return false;

    //�����С
    block_size_ = EVP_CIPHER_block_size(cipher);

    //��Կ������߶���
    if (key_size > EVP_CIPHER_key_length(cipher))
        key_size = EVP_CIPHER_key_length(cipher);
    memcpy(key, pass.data(), key_size);


    //�ӽ���������
    ctx_ = EVP_CIPHER_CTX_new();

    //��ʼ��������
    int re = EVP_CipherInit((EVP_CIPHER_CTX*)ctx_,
        cipher,key,iv_,is_en_
        );
    if (!re) 
    {
        ERR_print_errors_fp(stderr);
        return false;
    }
    //cout << "EVP_CipherInit success!" << endl;
    return true;
}


/////////////////////////////////////////////////////////////////////
/// �ӽ�������
///@para in ��������
///@para in_size ���ݴ�С
///@para �������
///@para �ɹ����ؼӽ��ܺ������ֽڴ�С��ʧ�ܷ���0
int XSec::Encrypt(const unsigned char* in, int in_size, unsigned char* out, bool is_end)
{
    if (type_ == XDES_ECB)
    {
        if (is_en_)
        {
            return EnDesECB(in,in_size,out,is_end);
        }
        else
        {
            return DeDesECB(in, in_size, out,is_end);
        }
    }
    else if (type_ == XDES_CBC)
    {
        if (is_en_)
        {
            return EnDesCBC(in, in_size, out, is_end);
        }
        else
        {
            return DeDesCBC(in, in_size, out, is_end);
        }
    }
    //�������һ�����ݣ������PKCS7
    if (is_end)
        EVP_CIPHER_CTX_set_padding((EVP_CIPHER_CTX*)ctx_, EVP_PADDING_PKCS7);
    else
        EVP_CIPHER_CTX_set_padding((EVP_CIPHER_CTX*)ctx_, 0); //�ر��Զ����
    int out_len = 0;
    EVP_CipherUpdate((EVP_CIPHER_CTX*)ctx_, out, &out_len, in, in_size);
    if (out_len <= 0)
        return 0;
    //��ȥ���õ�����
    int out_padding_len = 0;
    EVP_CipherFinal((EVP_CIPHER_CTX*)ctx_, out + out_len, &out_padding_len);
    return out_len+ out_padding_len;
}
////////////////////////////////////////////////////////////////////////
/// DES ECBģʽ����
int XSec::DeDesECB(const unsigned char* in, int in_size, unsigned char* out, bool is_end)
{
    for (int i = 0; i < in_size; i += block_size_)
    {
        DES_ecb_encrypt(
            (const_DES_cblock*)(in + i),
            (DES_cblock*)(out + i),
            &ks_,
            DES_DECRYPT
        );
    }
    if (is_end)
        //PKCS7 ���һ���ֽڴ洢�Ĳ����ֽ���
        return in_size - out[in_size - 1];
    else
        return in_size;
}
////////////////////////////////////////////////////////////////////////
/// DES ECBģʽ����
int XSec::EnDesECB(const unsigned char* in, int in_size, unsigned char* out, bool is_end)
{
    ///������� PKCS7 Padding
    /*
    �������ݳ�����Ҫ���n(n>0)���ֽڲŶ��룬��ô���n���ֽڣ�ÿ���ֽڶ���n;
    ������ݱ�����Ѿ������ˣ������һ�鳤��Ϊ���С�����ݣ�ÿ���ֽڶ��ǿ��С��
    */
    unsigned char pad[8] = { 0 };
    int padding_size = block_size_ - (in_size % block_size_);
    //���벹����ֽڴ�С
    memset(pad, padding_size, sizeof(pad));
    int i = 0;
    for ( ;i < in_size; i += block_size_)
    {
        //���һ�����ݣ�С��block_size_ ��Ҫ���
        if (in_size - i < block_size_)
        {
            //��������
            memcpy(pad, in + i, in_size - i);
            break;
        }
        DES_ecb_encrypt((const_DES_cblock*)(in + i),
            (DES_cblock*)(out + i),
            &ks_,
            DES_ENCRYPT
        );
    }
    if (!is_end)return in_size;
    //���� PKCS7��β
    DES_ecb_encrypt((const_DES_cblock*)pad,
        (DES_cblock*)(out + i),
        &ks_,
        DES_ENCRYPT
    );
    return in_size + padding_size;
}

////////////////////////////////////////////////////////////////////////
/// DES CBCģʽ����
int XSec::EnDesCBC(const unsigned char* in, int in_size, unsigned char* out,bool is_end)
{
    //�������� PKCS7 Padding
    unsigned char pad[8] = { 0 };
    int padding_size = block_size_ - (in_size % block_size_);
    //���벹����ֽڴ�С
    memset(pad, padding_size, sizeof(pad));
    //block ��������С
    int size1 = in_size - (in_size % block_size_);

    //ncbc����iv�޸� ��ȥ��Ҫ���������
    DES_ncbc_encrypt(in, out,
        size1,
        &ks_,
        (DES_cblock*)iv_,
        DES_ENCRYPT
    );
    if (!is_end)return in_size;
    //PKCS7 Padding
    if (in_size % block_size_ != 0)
    {
        //����ʣ�������
        memcpy(pad, in + size1, (in_size % block_size_));
    }
    DES_ncbc_encrypt(pad, out+size1,
        sizeof(pad),
        &ks_,
        (DES_cblock*)iv_,
        DES_ENCRYPT
    );
    return in_size + padding_size;
}

////////////////////////////////////////////////////////////////////////
/// DES CBCģʽ����
int XSec::DeDesCBC(const unsigned char* in, int in_size, unsigned char* out,bool is_end)
{
    DES_ncbc_encrypt(in, out, in_size, &ks_, (DES_cblock*)iv_, DES_DECRYPT);
    if (!is_end)return in_size;
    return in_size - out[in_size - 1];
}