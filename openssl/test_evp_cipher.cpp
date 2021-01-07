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
    //选择加解密算法，后面可以替换
    auto cipher = EVP_des_ede3_cbc();

    //输入文件大小
    int in_file_size = 0;

    //输出文件大小
    int out_file_size = 0;
    ifstream ifs(in_filename, ios::binary); //二进制打开输入文件
    if (!ifs)return false;
    ofstream ofs(out_filename, ios::binary);//二进制大小输出文件
    if (!ofs)
    {
        ifs.close();
        return false;
    }
    auto ctx = EVP_CIPHER_CTX_new(); //加解密上下文

    //密钥初始化 多出的丢弃
    unsigned char key[128] = { 0 };
    int key_size = EVP_CIPHER_key_length(cipher);// 获取密钥长度
    if (key_size > passwd.size())   //密码少了
    {
        key_size = passwd.size();
    }
    memcpy(key, passwd.data(), key_size);

    unsigned char iv[128] = { 0 }; //初始化向量
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
    //1 读文件=》2 加解密文件=》3写入文件
    while (!ifs.eof())
    {
        //1 读文件
        ifs.read((char*)buf, sizeof(buf));
        int count = ifs.gcount();
        if (count <= 0)break;
        in_file_size += count; //统计读取文件大小
        //2 加解密文件 机密到out
        EVP_CipherUpdate(ctx, out, &out_len, buf, count);
        if (out_len <= 0)break;
        //3 写入文件
        ofs.write((char*)out, out_len);
        out_file_size += out_len;
    }
    //取出最后一块数据
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
    ifstream ifs(in_filename, ios::binary); //二进制打开输入文件
    if (!ifs)return false;
    ofstream ofs(out_filename, ios::binary);//二进制大小输出文件
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
    //1 读文件=》2 加解密文件=》3写入文件
    while (!ifs.eof())
    {
        //1 读文件
        ifs.read((char*)buf, sizeof(buf));
        int count = ifs.gcount();
        if (count <= 0)break;
        bool is_end = false;
        if (ifs.eof()) //文件结尾
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

//测试算法性能
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
        //测试数据赋初值
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
        
        //加密
        sec.Init(type, passwd,true);
        auto start = clock();
        int en_size = sec.Encrypt(in_, data_size_, en_);
        auto end = clock();
        cout << "加密花费时间：" << (double)((end - start) / (double)CLOCKS_PER_SEC) << "秒" << endl;
        
        //解密
        sec.Init(type, passwd, false);
        start = clock();
        int de_size = sec.Encrypt(en_, en_size, de_);
        end = clock();
        cout  << "解密花费时间：" << (double)((end - start) / (double)CLOCKS_PER_SEC) << "秒" << endl;
    }

    ~TestCipher()
    {
        Close();
    }

private:
    //测试数字节数
    int data_size_ = 0;

    //测试数据
    unsigned char* in_ = nullptr;

    //加密后数据
    unsigned char* en_ = nullptr;

    //解密后数据
    unsigned char* de_ = nullptr;

    //密码 适应各种强度
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


    //加密文件
    EncryptFile("1234567812345678",
        "test_evp_cipher.cpp",
        "test_evp_cipher.encrypt2.txt",
        true);

    //解密文件
    EncryptFile("1234567812345678",
        "test_evp_cipher.encrypt2.txt",
        "test_evp_cipher.decrypt2.txt",
        false);

    //加密文件
    EncryptFile("12345678", 
        "test_evp_cipher.cpp", 
        "test_evp_cipher.encrypt.txt",
        true);
    
    //解密文件
    EncryptFile("12345678", 
        "test_evp_cipher.encrypt.txt", 
        "test_evp_cipher.decrypt.txt", 
        false);
    
    const unsigned char data[128] = "12345678123456781";//输入
    int data_size = strlen((char*)data);
    cout << "data_size = " << data_size << endl;
    unsigned char out[1024] = { 0 };                //输出
    unsigned char key[128] = "12345678901234567890";//秘钥
    unsigned char iv[128] = { 0 };                  //初始化向量
    //三重DES 3DES 算法
    auto cipher = EVP_des_ede3_cbc();
    //error:digital envelope routines:EVP_CipherInit_ex:initialization error
    //auto cipher = EVP_des_cbc();

    //获取算法的分组大小（）
    int block_size = EVP_CIPHER_block_size(cipher);
    int key_size = EVP_CIPHER_key_length(cipher);
    int iv_size = EVP_CIPHER_iv_length(cipher);
    cout << "block_size = " << block_size << endl;
    cout << "key_size = " << key_size << endl;
    cout << "iv_size = " << iv_size << endl;

    //加解密上下文
    auto ctx = EVP_CIPHER_CTX_new();
    //加密算法初始化
    int re = EVP_CipherInit(ctx, cipher, key, iv,
        1  //1 表示加密
    );
    if (!re)
    {
        ERR_print_errors_fp(stderr);
        getchar();
        return -1;
    }
    cout << "EVP_CipherInit success！" << endl;

    //默认 PKCS7 补充大小 EVP_PADDING_PKCS7
    //关闭自动填充
    //EVP_CIPHER_CTX_set_padding(ctx, 0);
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);
    int out_size = 0;

    //只处理分组大小得到数据,如果取消自动填充，多余数据丢弃
    // 如果自动填充，则在EVP_CipherFinal 中获取数据
    EVP_CipherUpdate(ctx,
        out,           //输出
        &out_size,     //输出数据大小
        data,          //输入数据
        data_size
    );

    cout << "EVP_CipherUpdate size:" << out_size << endl;
    //取出最后一块数据（需要填充的），或者是padding补充的数据
    int padding_size = 0;
    EVP_CipherFinal(ctx, out + out_size, &padding_size);
    cout << "padding_size = " << padding_size << endl;
    out_size += padding_size;
    cout << out_size << ":" << out << endl;

    //////////////////////////////////////////////////////////////////
    /// 解密数据 使用原来的ctx
    re = EVP_CipherInit(ctx, cipher, key, iv, 
        0 //0表示解密
    );
    if (!re)
    {
        ERR_print_errors_fp(stderr);
    }

    //解密密文后存放的明文
    unsigned char out2[1024] = { 0 };
    int out2_size = 0;
    //解密数据 填充数据取不到
    EVP_CipherUpdate(ctx,
        out2, &out2_size, //输入密文数据
        out, out_size);   //输出解密后明文
    cout << "EVP_CipherUpdate out2_size = " << out2_size << endl;

    //取出填充数据
    EVP_CipherFinal(ctx, out2 + out2_size, &padding_size);
    cout << "EVP_CipherFinal padding_size=" << padding_size << endl;
    out2_size += padding_size;
    cout << out2_size << ":" << out2 << "|" << endl;


    //释放上下文
    EVP_CIPHER_CTX_free(ctx);

    getchar();
    return 0;
}
