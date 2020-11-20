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

#include <iostream>
#include <openssl/md5.h>
#include <fstream>
#include <thread>
#include <vector>
#include <openssl/sha.h>
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

//文件可信树Hash
/*
                    A               A
                  /  \            /   \
                B     C         B       C
               / \    |        / \     / \
              D   E   F       D   E   F   F
             / \ / \ / \     / \ / \ / \ / \
             1 2 3 4 5 6     1 2 3 4 5 6 5 6
*/
string GetFileMerkleHash(string filepath)
{
    string hash;
    //存放hash叶子节点，后面所有结果都存在其中
    vector<string> hashes;
    ifstream ifs(filepath, ios::binary);
    if (!ifs)return hash;
    unsigned char buf[1024] = { 0 };
    unsigned char out[1024] = { 0 };
    int block_size = 128;
    while (!ifs.eof())
    {
        ifs.read((char*)buf, block_size);
        int read_size = ifs.gcount();
        if (read_size <= 0)break;
        SHA1(buf, read_size, out);
        //写入叶子节点的hash值
        hashes.push_back(string(out, out + 20));
    }

    while (hashes.size() > 1) // ==1 表示已经计算到root节点
    {
        //不是二的倍数补节点 （二叉树）
        if (hashes.size() & 1)
        {
            //补充最后一个节点
            hashes.push_back(hashes.back());
        }
        //把两两节点的hash结果还写入hashes中，
        for (int i = 0; i < hashes.size() / 2; i++)
        {
            //两个节点拼起来 i表示的是父节点
            string tmp_hash = hashes[i * 2];
            tmp_hash += hashes[i * 2 + 1];
            SHA1((unsigned char*)tmp_hash.data(), tmp_hash.size(), out);
            //写入结果
            hashes[i] = string(out, out + 20);
        }
        //hash列表删除上一次多余的hash值
        hashes.resize(hashes.size() / 2);
    }
    if (hashes.size() == 0) return hash;
    return hashes[0];
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
    unsigned char data[] = "测试md5数据";
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
        auto thash = GetFileMerkleHash(filepath);
        cout << "HashList:\t";
        PrintHex(hash);
        cout << "MerkleTree:\t";
        PrintHex(thash);
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