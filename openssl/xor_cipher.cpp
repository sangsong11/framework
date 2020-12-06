#include<iostream>
using namespace std;
////////////////////////////////////////////////////
/// �ԳƼӽ�������
/// @para data ��������
/// @para data_size �������ݴ�С
/// @para out �������
/// @para pass ��Կ
/// @para pass_size ��Կ����
/// @return �ӽ��ܺ����ݴ�С
#define XOR_BLOCK 8
int XorCipher(const unsigned char*data,int data_size,
    unsigned char *out,
    const unsigned char *pass,
    int pass_size
    )
{
    static const char iv[] = "abcdefgh";
    //��ʼ����Կ
    auto p = *(unsigned long long*)iv;
    //��Կ��ȫ����������ʼ������
    //��ԿС�� XOR_BLOCK ���ߴ��� XOR_BLOCK
    for (int i = 0; i < pass_size; i += XOR_BLOCK)
    {
        unsigned long long tmp = 0;
        int size = XOR_BLOCK;
        ///��ԿС�� XOR_BLOCK 
        if (pass_size - i < XOR_BLOCK)
        {
            size = pass_size - i;
        }
        memcpy(&tmp, (pass + i), size);
        p = (p ^ tmp);
    }


    //����Դת����8�ֽ���������
    auto d = (unsigned long long*)data;
    //�������
    auto o = (unsigned long long*) out;
    //���ݷ��鴦��
    int i = 0;
    for ( ;i < data_size / XOR_BLOCK; i++)
    {
        //XOR �������
        o[i] = (d[i] ^ p);
    }
    //�������ݵĲ��� ZERO
    /*
        block = 8
        12345678 9
        12345678 90000000  ZERO
        12345678 97777777   PKCS7 
        ѧԱʵ�� PKCS7
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
    unsigned char data[] = "���Լӽ�������TEST123������Կ";
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