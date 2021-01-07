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

#include <iostream>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <fstream>
#include <ctime>

// _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#include <openssl/applink.c>
#endif
#include "xsec.h"
using namespace std;

bool EncryptFile(string passwd, string in_filename, string out_filename,bool is_enc)
{
    //ѡ��ӽ����㷨����������滻
    auto cipher = EVP_des_ede3_cbc();

    //�����ļ���С
    int in_file_size = 0;

    //����ļ���С
    int out_file_size = 0;
    ifstream ifs(in_filename, ios::binary); //�����ƴ������ļ�
    if (!ifs)return false;
    ofstream ofs(out_filename, ios::binary);//�����ƴ�С����ļ�
    if (!ofs)
    {
        ifs.close();
        return false;
    }
    auto ctx = EVP_CIPHER_CTX_new(); //�ӽ���������

    //��Կ��ʼ�� ����Ķ���
    unsigned char key[128] = { 0 };
    int key_size = EVP_CIPHER_key_length(cipher);// ��ȡ��Կ����
    if (key_size > passwd.size())   //��������
    {
        key_size = passwd.size();
    }
    memcpy(key, passwd.data(), key_size);

    unsigned char iv[128] = { 0 }; //��ʼ������
    int re = EVP_CipherInit(ctx, cipher, key, iv, is_enc);
    if (!re)
    {
        ERR_print_errors_fp(stderr);
        ifs.close();
        ofs.close();
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    unsigned char buf[1024] = { 0 };
    unsigned char out[1024] = { 0 };
    int out_len = 0;
    //1 ���ļ�=��2 �ӽ����ļ�=��3д���ļ�
    while (!ifs.eof())
    {
        //1 ���ļ�
        ifs.read((char*)buf, sizeof(buf));
        int count = ifs.gcount();
        if (count <= 0)break;
        in_file_size += count; //ͳ�ƶ�ȡ�ļ���С
        //2 �ӽ����ļ� ���ܵ�out
        EVP_CipherUpdate(ctx, out, &out_len, buf, count);
        if (out_len <= 0)break;
        //3 д���ļ�
        ofs.write((char*)out, out_len);
        out_file_size += out_len;
    }
    //ȡ�����һ������
    EVP_CipherFinal(ctx, out, &out_len);
    if (out_len > 0)
    {
        ofs.write((char*)out, out_len);
        out_file_size += out_len;
    }

    ifs.close();
    ofs.close();
    EVP_CIPHER_CTX_free(ctx);
    cout << "in_file_size:" << in_file_size << endl;
    cout << "out_file_size:" << out_file_size << endl;
    return true;
}
bool XSecEncryptFile(string passwd, string in_filename, string out_filename, bool is_enc)
{
    ifstream ifs(in_filename, ios::binary); //�����ƴ������ļ�
    if (!ifs)return false;
    ofstream ofs(out_filename, ios::binary);//�����ƴ�С����ļ�
    if (!ofs)
    {
        ifs.close();
        return false;
    }
    XSec sec;
    sec.Init(XAES128_CBC, "1234567812345678", is_enc);

    unsigned char buf[1024] = { 0 };
    unsigned char out[1024] = { 0 };
    int out_len = 0;
    //1 ���ļ�=��2 �ӽ����ļ�=��3д���ļ�
    while (!ifs.eof())
    {
        //1 ���ļ�
        ifs.read((char*)buf, sizeof(buf));
        int count = ifs.gcount();
        if (count <= 0)break;
        bool is_end = false;
        if (ifs.eof()) //�ļ���β
            is_end = true;
        out_len = sec.Encrypt(buf, count, out, is_end);
        if (out_len <= 0)
            break;
        ofs.write((char*)out, out_len);
    }
    sec.Close();
    ifs.close();
    ofs.close();
    return true;
}

//�����㷨����
class TestCipher
{
public:
    void Close()
    {
        delete in_;
        in_ = nullptr;
        delete de_;
        de_ = nullptr;
        delete en_;
        en_ = nullptr;
    }
    void Init(int data_size)
    {
        Close();
        data_size_ = data_size;
        in_ = new unsigned char[data_size];
        en_ = new unsigned char[data_size + 128];
        de_ = new unsigned char[data_size + 128];
        //�������ݸ���ֵ
        unsigned int data = 1;
        for (int i = 0; i < data_size; i += sizeof(data))
        {
            memcpy(in_ + i, &data, sizeof(data));
            data++;
        }
        memset(en_, 0, data_size + 128);
        memset(de_, 0, data_size + 128);
    }
    void Test(XSecType type, string type_name)
    {
        memset(en_, 0, data_size_ + 128);
        memset(de_, 0, data_size_ + 128);
        cout << " ============ "<<type_name << " ============ " << endl;
        XSec sec;
        
        //����
        sec.Init(type, passwd,true);
        auto start = clock();
        int en_size = sec.Encrypt(in_, data_size_, en_);
        auto end = clock();
        cout << "���ܻ���ʱ�䣺" << (double)((end - start) / (double)CLOCKS_PER_SEC) << "��" << endl;
        
        //����
        sec.Init(type, passwd, false);
        start = clock();
        int de_size = sec.Encrypt(en_, en_size, de_);
        end = clock();
        cout  << "���ܻ���ʱ�䣺" << (double)((end - start) / (double)CLOCKS_PER_SEC) << "��" << endl;
    }

    ~TestCipher()
    {
        Close();
    }

private:
    //�������ֽ���
    int data_size_ = 0;

    //��������
    unsigned char* in_ = nullptr;

    //���ܺ�����
    unsigned char* en_ = nullptr;

    //���ܺ�����
    unsigned char* de_ = nullptr;

    //���� ��Ӧ����ǿ��
    string passwd = "12345678ABCDEFGHabcdefgh!@#$%^&*";
};

//ci.Test(XDES_ECB, "XDES_ECB");
#define TEST_CIPHER(s) ci.Test(s, #s)
int main(int argc, char* argv[])
{
    TestCipher ci;
    ci.Init(1024 * 1024 * 100); // 100MB
    TEST_CIPHER(XDES_ECB);
    TEST_CIPHER(XDES_CBC);
    TEST_CIPHER(X3DES_ECB);
    TEST_CIPHER(X3DES_CBC);
    TEST_CIPHER(XAES128_ECB);
    TEST_CIPHER(XAES128_CBC);
    TEST_CIPHER(XAES192_ECB);
    TEST_CIPHER(XAES192_CBC);
    TEST_CIPHER(XAES256_ECB);
    TEST_CIPHER(XAES256_CBC);
    TEST_CIPHER(XSM4_ECB);
    TEST_CIPHER(XSM4_CBC);
    getchar();
    /*
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
    XSM4_CBC*/
    //ci.Test(XDES_ECB, "XDES_ECB");


    //�����ļ�
    EncryptFile("1234567812345678",
        "test_evp_cipher.cpp",
        "test_evp_cipher.encrypt2.txt",
        true);

    //�����ļ�
    EncryptFile("1234567812345678",
        "test_evp_cipher.encrypt2.txt",
        "test_evp_cipher.decrypt2.txt",
        false);

    //�����ļ�
    EncryptFile("12345678", 
        "test_evp_cipher.cpp", 
        "test_evp_cipher.encrypt.txt",
        true);
    
    //�����ļ�
    EncryptFile("12345678", 
        "test_evp_cipher.encrypt.txt", 
        "test_evp_cipher.decrypt.txt", 
        false);
    
    const unsigned char data[128] = "12345678123456781";//����
    int data_size = strlen((char*)data);
    cout << "data_size = " << data_size << endl;
    unsigned char out[1024] = { 0 };                //���
    unsigned char key[128] = "12345678901234567890";//��Կ
    unsigned char iv[128] = { 0 };                  //��ʼ������
    //����DES 3DES �㷨
    auto cipher = EVP_des_ede3_cbc();
    //error:digital envelope routines:EVP_CipherInit_ex:initialization error
    //auto cipher = EVP_des_cbc();

    //��ȡ�㷨�ķ����С����
    int block_size = EVP_CIPHER_block_size(cipher);
    int key_size = EVP_CIPHER_key_length(cipher);
    int iv_size = EVP_CIPHER_iv_length(cipher);
    cout << "block_size = " << block_size << endl;
    cout << "key_size = " << key_size << endl;
    cout << "iv_size = " << iv_size << endl;

    //�ӽ���������
    auto ctx = EVP_CIPHER_CTX_new();
    //�����㷨��ʼ��
    int re = EVP_CipherInit(ctx, cipher, key, iv,
        1  //1 ��ʾ����
    );
    if (!re)
    {
        ERR_print_errors_fp(stderr);
        getchar();
        return -1;
    }
    cout << "EVP_CipherInit success��" << endl;

    //Ĭ�� PKCS7 �����С EVP_PADDING_PKCS7
    //�ر��Զ����
    //EVP_CIPHER_CTX_set_padding(ctx, 0);
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);
    int out_size = 0;

    //ֻ��������С�õ�����,���ȡ���Զ���䣬�������ݶ���
    // ����Զ���䣬����EVP_CipherFinal �л�ȡ����
    EVP_CipherUpdate(ctx,
        out,           //���
        &out_size,     //������ݴ�С
        data,          //��������
        data_size
    );

    cout << "EVP_CipherUpdate size:" << out_size << endl;
    //ȡ�����һ�����ݣ���Ҫ���ģ���������padding���������
    int padding_size = 0;
    EVP_CipherFinal(ctx, out + out_size, &padding_size);
    cout << "padding_size = " << padding_size << endl;
    out_size += padding_size;
    cout << out_size << ":" << out << endl;

    //////////////////////////////////////////////////////////////////
    /// �������� ʹ��ԭ����ctx
    re = EVP_CipherInit(ctx, cipher, key, iv, 
        0 //0��ʾ����
    );
    if (!re)
    {
        ERR_print_errors_fp(stderr);
    }

    //�������ĺ��ŵ�����
    unsigned char out2[1024] = { 0 };
    int out2_size = 0;
    //�������� �������ȡ����
    EVP_CipherUpdate(ctx,
        out2, &out2_size, //������������
        out, out_size);   //������ܺ�����
    cout << "EVP_CipherUpdate out2_size = " << out2_size << endl;

    //ȡ���������
    EVP_CipherFinal(ctx, out2 + out2_size, &padding_size);
    cout << "EVP_CipherFinal padding_size=" << padding_size << endl;
    out2_size += padding_size;
    cout << out2_size << ":" << out2 << "|" << endl;


    //�ͷ�������
    EVP_CIPHER_CTX_free(ctx);

    getchar();
    return 0;
}