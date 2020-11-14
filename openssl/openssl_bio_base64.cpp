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
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
using namespace std;
int Base64Encode(const unsigned char* in, int len, char* out_base64)
{
    if (!in || len <= 0 || !out_base64)
        return 0;
    //�ڴ�Դ source
    auto mem_bio = BIO_new(BIO_s_mem());
    if (!mem_bio)return 0;

    //base64 filter
    auto b64_bio = BIO_new(BIO_f_base64());
    if (!b64_bio)
    {
        BIO_free(mem_bio);
        return 0;
    }

    //�γ�BIO��
    //b64-mem
    BIO_push(b64_bio, mem_bio);
    //����64�ֽڲ���ӻ��У�\n��,�����������һ����
    // Ĭ�Ͻ�β�л��з�\n ����64�ֽ������\n
    BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);
    
    // д�뵽base64 filter ���б��룬����ᴫ�ݵ��������һ���ڵ�
    // ��mem�ж�ȡ���(����ͷ����������������)
    // BIO_write ���� 3�ֽ�=��4�ֽ�  ����3�ֽڲ���0 �� =
    // ��������ÿ64�ֽڣ���ȷ�������\n ���з�
    int re = BIO_write(b64_bio, in, len);
    if (re <= 0)
    {
        //�����������ڵ�
        BIO_free_all(b64_bio);
        return 0;
    }

    //ˢ�»��棬д�������mem
    BIO_flush(b64_bio);

    int outsize = 0;
    //������Դ�ڴ��ȡ
    BUF_MEM* p_data = 0;
    BIO_get_mem_ptr(b64_bio, &p_data);
    if (p_data)
    {
        memcpy(out_base64, p_data->data, p_data->length);
        outsize = p_data->length;
    }
    BIO_free_all(b64_bio);
    return outsize;
}

int Base64Decode(const char* in, int len, unsigned char* out_data)
{
    if (!in || len <= 0 || !out_data)
        return 0;
    //�ڴ�Դ �����ģ�
    auto mem_bio = BIO_new_mem_buf(in, len);
    if (!mem_bio)return 0;
    //base64 ������
    auto b64_bio = BIO_new(BIO_f_base64());
    if (!b64_bio)
    {
        BIO_free(mem_bio);
        return 0;
    }
    //�γ�BIO��
    BIO_push(b64_bio, mem_bio);

    //Ĭ�϶�ȡ���з�������
    //���ú�����������\n��ʧ��
    BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);

    //��ȡ ���� 4�ֽ�ת3�ֽ�
    size_t size = 0;
    BIO_read_ex(b64_bio, out_data,len,&size);
    BIO_free_all(b64_bio);
    return size;

    

}
int main(int argc, char* argv[])
{
    cout << "Test  openssl BIO base64!" << endl;
    
    unsigned char data[] = "����base64����0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    int len = sizeof(data);
    char out[4096] = { 0 };
    unsigned char out2[4096] = { 0 };
    cout <<"source:"<< data << endl;
    int re = Base64Encode(data, len, out);
    if (re > 0)
    {
        //ncode:[suLK1GJhc2U2NMr9vt0A]
        out[re] = '\0';
        cout << "encode:["<<out<<"]" << endl;
    }
    re = Base64Decode(out, re, out2);
    cout <<"decode:"<< out2 << endl;
    getchar();
    return 0;
}