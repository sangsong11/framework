#include <iostream>
#include <openssl/md5.h>
using namespace std;
int main(int argc, char* argv[])
{
    cout << "Test  Hash!" << endl;
    unsigned char data[] = "²âÊÔmd5Êý¾Ý";
    unsigned char out[1024] = { 0 };
    int len = sizeof(data);
    MD5_CTX c;
    MD5_Init(&c);
    MD5_Update(&c, data, len);
    MD5_Final(out, &c);
    for (int i = 0; i < 16; i++)
        cout << hex << (int)out[i];
    cout << endl;
    data[1] = 9;
    MD5(data, len, out);
    for (int i = 0; i < 16; i++)
        cout << hex << (int)out[i];
    cout << endl;
    getchar();
    return 0;
}