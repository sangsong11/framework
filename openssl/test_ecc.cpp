#include <iostream>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/ec.h>
#ifdef _WIN32
#include <openssl/applink.c>
#endif
using namespace std;

void PrintBn(const BIGNUM* n)
{
    //大数对象转为二进制
    unsigned char to[256] = { 0 };
    BN_bn2bin(n, to);
    int byte_size = BN_num_bytes(n);
    for (int i = 0; i < byte_size; i++)
        printf("%02x", to[i]);
    printf("\n");
}
EVP_PKEY* EccKey()
{
    //打印所有支持的椭圆曲线
    //获取椭圆曲线数量
    int cur_len = EC_get_builtin_curves(NULL, 0);
    EC_builtin_curve* curves = new EC_builtin_curve[cur_len];
    EC_get_builtin_curves(curves, cur_len);
    for (int i = 0; i < cur_len; i++)
    {
        cout << i+1<<"|"<<curves[i].nid << ":" << curves[i].comment << endl;
    }
    delete []curves;

    //选择椭圆曲线 设置生成密钥参数 国密sm2 支持 加解密
    // secp256k1 不支持加解密 （比特币，以太网用），支持签名和密钥交换
    auto group = EC_GROUP_new_by_curve_name(NID_sm2);
    if (!group)
    {
        ERR_print_errors_fp(stderr);
        return NULL;
    }

    //ec 密钥存放上下文
    auto key = EC_KEY_new();
    //设置密钥参数
    EC_KEY_set_group(key, group);
    //生成密钥
    int re = EC_KEY_generate_key(key);
    if (re != 1)
    {
        EC_KEY_free(key);
        ERR_print_errors_fp(stderr);
        return NULL;
    }
    //检查密钥
    re = EC_KEY_check_key(key);
    if(re != 1)
    {
        EC_KEY_free(key);
        ERR_print_errors_fp(stderr);
        return NULL;
    }
    cout << "EC_KEY_check_key success!" << endl;
    
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_set1_EC_KEY(pkey, key);
    EC_KEY_free(key);
    return pkey;
}

int main(int argc, char* argv[])
{
    unsigned char data[1024] = { 0 };
    unsigned char out[2046] = { 0 };
    unsigned char out2[2046] = { 0 };

    //初始化测试数据
    for (int i = 0; i < sizeof(data) - 1; i++)
    {
        data[i] = 'A' + i % 26;
    }
    int data_size = sizeof(data);

    //ecc 密钥对生成
    auto pkey = EccKey();

    //ecc加密上下文初始化
    auto ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
    }

    //除了sm2 其他的一些算法会失败
    int re = EVP_PKEY_encrypt_init(ctx);
    if (re != 1)
    {
        ERR_print_errors_fp(stderr);
    }

    //ecc 加密 
    size_t out_len = sizeof(out);
    EVP_PKEY_encrypt(ctx, out, &out_len, data, data_size);
    cout << out_len << ":" << out << endl;


    //ecc 解密
    EVP_PKEY_decrypt_init(ctx);
    size_t in_size = out_len;
    out_len = sizeof(out2);
    EVP_PKEY_decrypt(ctx, out2, &out_len, out, in_size);
    cout << out_len << ":" << out2 << endl;

    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);

    getchar();
    return 0;
}










































