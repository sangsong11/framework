#include <iostream>
#include <openssl/md5.h>
#include <fstream>
#include <thread>
using namespace std;

string GetFileListHash(string filepath)
{
    string hash;
    //以二进制方式打开文件
    ifstream ifs(filepath, ios::binary);
    if (!ifs)
        return hash;
    //一次读取多少字节的文件
    int block_size = 128;
    
    //文件读取buf
    unsigned char buf[1024] = { 0 };

    //hash输出
    unsigned char out[1024] = { 0 };

    while (!ifs.eof())
    {
        ifs.read((char*)buf, block_size);
        int read_size = ifs.gcount();
        if (read_size <= 0)break;
        MD5(buf, read_size, out);
        hash.insert(hash.end(), out, out + 16);
    }
    ifs.close();
    MD5((unsigned char*)hash.data(), hash.size(), out);
    return string(out,out+16);
}
void PrintHex(string data)
{
    for (auto c : data)
        cout << hex << (int)(unsigned char)c;
    cout << endl;
}
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
    
    string filepath = "../../src/test_hash/test_hash.cpp";
    auto hash1 = GetFileListHash(filepath);
    PrintHex(hash1);
    //验证文件完整性
    for (;;)
    {
        auto hash = GetFileListHash(filepath);
        if (hash != hash1)
        { 
            cout << "文件被修改" ;
            PrintHex(hash);
        }   
        this_thread::sleep_for(1s);
    }
    getchar();
    
    
    return 0;
}
