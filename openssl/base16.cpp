#include <iostream>
using namespace std;
static const char BASE16_ENC_TAB[] = "0123456789ABCDEF";
//  '0' ~'9' =>  48~57  'A'~'F' = > 65~70
static const char BASE16_DEC_TAB[128] = {
    -1,                                 //0
    -1,-1,-1,-1,-1, -1,-1,-1,-1,-1,      //1-10
    -1,-1,-1,-1,-1, -1,-1,-1,-1,-1,      //11-20
    -1,-1,-1,-1,-1, -1,-1,-1,-1,-1,      //21-30
    -1,-1,-1,-1,-1, -1,-1,-1,-1,-1,      //31-40
    -1,-1,-1,-1,-1, -1,-1, 0, 1, 2,      //41-50
    3, 4, 5, 6 ,7,  8, 9, -1,-1,-1,      //51-60
    -1,-1,-1,-1,10, 11,12,13,14,15      //61-70 'A'~'F'
};

int Base16Encode(const unsigned char* in, int size, char* out)
{
    for (int i = 0; i < size; i++)
    {
        //一个字节取出高4位和低4位  1000 0001  0000 1000
        char h = in[i] >> 4;   //移位丢弃低位  （0~15）
        char l = in[i] & 0x0F; // 0000 1111 去掉高位 （0~15）
        out[i * 2] = BASE16_ENC_TAB[h]; //（0~15） 映射到对应字符
        out[i * 2 + 1] = BASE16_ENC_TAB[l];
    }
    //base16 转码后空间扩大一倍 4位转成一个字符 1个字节转成两个字符
    return size * 2;
}

int Base16Decode(const string &in, unsigned char* out)
{
    //讲两个字符拼成一个字节  B2E2CAD442617365313600
    for (int i = 0; i < in.size(); i += 2)
    {
        unsigned char ch = (int)in[i];           //高位转换的字符 'B'=>66 :10
        unsigned char cl = (int)in[i + 1];       //低位转换的字符 '2'=>50 :2
        unsigned char h = BASE16_DEC_TAB[ch];   //转换成原来的值
        unsigned char l = BASE16_DEC_TAB[cl];

        // 两个4位拼成一个字节（8位）
        ///  1000  >>4    1000 0000
        //   0001         0000 0001
        //               |1000 0001
        out[i / 2] = (int)(h << 4 | l);
    }
    return in.size() / 2;
}

int main(int argc,char *argv[])
{
    cout << "Test Base16" << endl;
    const unsigned char data[] = "测试Base16";
    int len = sizeof(data);
    char out1[1024] = { 0 };
    unsigned char out2[1024] = { 0 };
    cout << data << endl;
    int re = Base16Encode(data, len, out1);
    cout << re << ":" << out1 << endl;
    re = Base16Decode(out1, out2);
    cout << re << ":" << (char*)out2 << endl;

    return 0;
}