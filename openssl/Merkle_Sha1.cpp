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
#include <openssl/md5.h>
#include <fstream>
#include <thread>
#include <vector>
#include <openssl/sha.h>
using namespace std;

string GetFileListHash(string filepath)
{
    string hash;
    //�Զ����Ʒ�ʽ���ļ�
    ifstream ifs(filepath, ios::binary);
    if (!ifs)
        return hash;
    //һ�ζ�ȡ�����ֽڵ��ļ�
    int block_size = 128;
    
    //�ļ���ȡbuf
    unsigned char buf[1024] = { 0 };

    //hash���
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

//�ļ�������Hash
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
    //���hashҶ�ӽڵ㣬�������н������������
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
        //д��Ҷ�ӽڵ��hashֵ
        hashes.push_back(string(out, out + 20));
    }

    while (hashes.size() > 1) // ==1 ��ʾ�Ѿ����㵽root�ڵ�
    {
        //���Ƕ��ı������ڵ� ����������
        if (hashes.size() & 1)
        {
            //�������һ���ڵ�
            hashes.push_back(hashes.back());
        }
        //�������ڵ��hash�����д��hashes�У�
        for (int i = 0; i < hashes.size() / 2; i++)
        {
            //�����ڵ�ƴ���� i��ʾ���Ǹ��ڵ�
            string tmp_hash = hashes[i * 2];
            tmp_hash += hashes[i * 2 + 1];
            SHA1((unsigned char*)tmp_hash.data(), tmp_hash.size(), out);
            //д����
            hashes[i] = string(out, out + 20);
        }
        //hash�б�ɾ����һ�ζ����hashֵ
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
    unsigned char data[] = "����md5����";
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
    //��֤�ļ�������
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
            cout << "�ļ����޸�" ;
            PrintHex(hash);
        }   
        this_thread::sleep_for(1s);
    }



    getchar();
    return 0;
}