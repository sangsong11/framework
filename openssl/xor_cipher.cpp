#include<iostream>
using namespace std;
////////////////////////////////////////////////////
/// 对称加解密数据
/// @para data 输入数据
/// @para data_size 输入数据大小
/// @para out 输出数据
/// @para pass 秘钥
/// @para pass_size 秘钥长度
/// @return 加解密后数据大小
#define XOR_BLOCK 8
int XorCipher(const unsigned char*data,int data_size,
    unsigned char *out,
    const unsigned char *pass,
    int pass_size
    )
{
    static const char iv[] = "abcdefgh";
    //初始化秘钥
    auto p = *(unsigned long long*)iv;
    //秘钥补全，并且异或初始化向量
    //秘钥小于 XOR_BLOCK 或者大于 XOR_BLOCK
    for (int i = 0; i < pass_size; i += XOR_BLOCK)
    {
        unsigned long long tmp = 0;
        int size = XOR_BLOCK;
        ///秘钥小于 XOR_BLOCK 
        if (pass_size - i < XOR_BLOCK)
        {
            size = pass_size - i;
        }
        memcpy(&tmp, (pass + i), size);
        p = (p ^ tmp);
    }


    //数据源转换成8字节数据类型
    auto d = (unsigned long long*)data;
    //输出数据
    auto o = (unsigned long long*) out;
    //数据分组处理
    int i = 0;
    for ( ;i < data_size / XOR_BLOCK; i++)
    {
        //XOR 异或运算
        o[i] = (d[i] ^ p);
    }
    //输入数据的补充 ZERO
    /*
        block = 8
        12345678 9
        12345678 90000000  ZERO
        12345678 97777777   PKCS7 
        学员实现 PKCS7
    */
    int mod = data_size % XOR_BLOCK;
    if (mod != 0)
    {
        unsigned long long tmp = 0;
        memcpy(&tmp, (d + i), mod);
        o[i] = (tmp ^ p);
    }

    int re_size = data_size;
    return re_size;
}
int main(int argc, char* argv[])
{
    unsigned char data[] = "测试加解密数据TEST123测试秘钥";
    unsigned char out[1024] = { 0 };
    unsigned char out2[1024] = { 0 };
    unsigned char pass[] = "12345678";
    int pass_size = strlen((char*)pass);
    int len = XorCipher(data, sizeof(data), out, pass, pass_size);
    cout << len << "|" << out << endl;
    len = XorCipher(out, len, out2, pass, pass_size);
    cout << len << ":" << out2 << endl;

    return 0;
}