#include <iostream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
using namespace std;
int Base64Encode(const unsigned char* in, int len, char* out_base64)
{
    if (!in || len <= 0 || !out_base64)
        return 0;
    //内存源 source
    auto mem_bio = BIO_new(BIO_s_mem());
    if (!mem_bio)return 0;

    //base64 filter
    auto b64_bio = BIO_new(BIO_f_base64());
    if (!b64_bio)
    {
        BIO_free(mem_bio);
        return 0;
    }

    //形成BIO链
    //b64-mem
    BIO_push(b64_bio, mem_bio);
    //超过64字节不添加换行（\n）,编码的数据在一行中
    // 默认结尾有换行符\n 超过64字节再添加\n
    BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);
    
    // 写入到base64 filter 进行编码，结果会传递到链表的下一个节点
    // 到mem中读取结果(链表头部代表了整个链表)
    // BIO_write 编码 3字节=》4字节  不足3字节补充0 和 =
    // 编码数据每64字节（不确定）会加\n 换行符
    int re = BIO_write(b64_bio, in, len);
    if (re <= 0)
    {
        //情况整个链表节点
        BIO_free_all(b64_bio);
        return 0;
    }

    //刷新缓存，写入链表的mem
    BIO_flush(b64_bio);

    int outsize = 0;
    //从链表源内存读取
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
    //内存源 （密文）
    auto mem_bio = BIO_new_mem_buf(in, len);
    if (!mem_bio)return 0;
    //base64 过滤器
    auto b64_bio = BIO_new(BIO_f_base64());
    if (!b64_bio)
    {
        BIO_free(mem_bio);
        return 0;
    }
    //形成BIO链
    BIO_push(b64_bio, mem_bio);

    //默认读取换行符做结束
    //设置后编码中如果有\n会失败
    BIO_set_flags(b64_bio, BIO_FLAGS_BASE64_NO_NL);

    //读取 解码 4字节转3字节
    size_t size = 0;
    BIO_read_ex(b64_bio, out_data,len,&size);
    BIO_free_all(b64_bio);
    return size;

    

}
int main(int argc, char* argv[])
{
    cout << "Test  openssl BIO base64!" << endl;
    
    unsigned char data[] = "测试base64数据0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
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
