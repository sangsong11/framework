#include <iostream>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#ifdef _WIN32
#include <openssl/applink.c>
#endif
using namespace std;
#define PUBKEY_PEM "pubkey.pem"
#define PRIKEY_PEM "prikey.pem"

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
//////////////////////////////////////////////////////
/// 生成RSA 秘钥对
/// @return 返回的pkey由调用EVP_PKEY_free释放
EVP_PKEY* EvpRsaKey()
{
    //1 创建RSA公钥加密上下文
    auto ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
        return NULL;
    }

    //2 初始化密钥对生成上下文
    if (EVP_PKEY_keygen_init(ctx) <= 0)
    {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        return NULL;
    }

    //3 设置参数 RSA 秘钥位数
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 1024) <= 0)
    {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        return NULL;
    }

    //4 秘钥生成
    EVP_PKEY* pkey = NULL;
    //内部会生成EVP_PKEY 空间
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0)
    {
        ERR_print_errors_fp(stderr);
        EVP_PKEY_CTX_free(ctx);
        return NULL;
    }
    //释放上下文
    EVP_PKEY_CTX_free(ctx);

    //获取参数列表
    auto tp = EVP_PKEY_gettable_params(pkey);
    while (tp)
    {
        if (!tp->key)break;
        cout << tp->key << endl;
        tp++;
    }
    //获取参数的值
    BIGNUM* d = 0;
    EVP_PKEY_get_bn_param(pkey, "d", &d);
    PrintBn(d);
    BN_free(d);

    //输出公钥pem文件
    FILE* pubf = fopen(PUBKEY_PEM, "w");
    PEM_write_RSAPublicKey(pubf, EVP_PKEY_get0_RSA(pkey));


    //输出明文私钥pem文件
    FILE* prif = fopen(PRIKEY_PEM, "w");
    PEM_write_RSAPrivateKey(prif, EVP_PKEY_get0_RSA(pkey),
        NULL,   //加密的上下文
        NULL,   //秘钥
        0,      //秘钥长度
        NULL,   //加密回调函数
        NULL    //用户数据回调使用
    );
    fclose(pubf);
    fclose(prif);
    return pkey;
}

////////////////////////////////////////////////////////////////////////
///EVP Rsa加密
int EvpRsaEncrypt(const unsigned char *in,int in_size,unsigned char *out)
{
    //1 读取pem中的公钥
    FILE* fp = fopen(PUBKEY_PEM, "r");
    if (!fp)
        return 0;
    RSA* r = NULL;
    PEM_read_RSAPublicKey(fp,&r, NULL, NULL);
    fclose(fp);

    if (!r)
    {
        ERR_print_errors_fp(stderr);
        return 0;
    }
    //秘钥字节长度
    int key_size = RSA_size(r);
    
    //2 通过EVP_PKEY 生成EVP_PKEY_CTX上下文
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(pkey, r); //设置为rsa的秘钥
    auto ctx = EVP_PKEY_CTX_new(pkey, NULL);
    EVP_PKEY_free(pkey);
    RSA_free(r);

    //3 加密初始化
    EVP_PKEY_encrypt_init(ctx);
    // 数据块大小，考虑填充 （默认pkcs1） k-11
    int block_size = key_size - RSA_PKCS1_PADDING_SIZE;
    int out_size = 0; //输出数据大小 也用做输出空间偏移

    //4 加密数据块
    for (int i = 0; i < in_size; i += block_size)
    {
        //输出大小
        size_t out_len = key_size;
        //输入大小
        size_t bsize = block_size;      //k-11   128-11 = 117
        if (in_size - i < block_size)   //最后一块数据
            bsize = in_size - i;

        if (EVP_PKEY_encrypt(ctx,
            out + out_size,        //输出空间
            &out_len,   //输出空间大小，空间预留大小（输入）和实际加密后数据大小（输出）
            in + i,         //输入数据
            bsize) <= 0)   //输入数据大小，块大小
        {
            ERR_print_errors_fp(stderr);
            break;
        }
        out_size += out_len;
    }

    EVP_PKEY_CTX_free(ctx);
    return out_size;
}

////////////////////////////////////////////////////////////////////////
//EVP Rsa解密
int EvpRsaDecrypt(const unsigned char* in, int in_size, unsigned char* out)
{
    int out_size = 0;
    
    //1 打开pEM文件获取私钥
    FILE* fp = fopen(PRIKEY_PEM, "r");
    if (!fp)return 0;
    RSA* r = NULL;
    PEM_read_RSAPrivateKey(fp, &r, NULL, NULL);
    if (!r)
    {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    //秘钥字节长度
    int key_size = RSA_size(r);

    //生成PKEY 并创建上下文
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(pkey,r);
    auto ctx = EVP_PKEY_CTX_new(pkey, NULL);
    EVP_PKEY_free(pkey);
    RSA_free(r);

    //解密初始化
    EVP_PKEY_decrypt_init(ctx);
    
    //解密数据
    for (int i = 0; i < in_size; i += key_size)
    {
        size_t out_len = key_size;//需要设置输出空间大小
        if (EVP_PKEY_decrypt(ctx,
            out + out_size,
            &out_len,
            in + i,
            key_size) <= 0)
        {
            ERR_print_errors_fp(stderr);
            break;
        }
        out_size += out_len;
    }
    EVP_PKEY_CTX_free(ctx);

    return out_size;
}

////////////////////////////////////////////////////////////////////////
//EVP RSA 签名 hash=》私钥签名
int EvpSign(const unsigned char* in, int in_size, unsigned char* sign)
{
    //1 打开pEM文件获取私钥
    FILE* fp = fopen(PRIKEY_PEM, "r");
    if (!fp)return 0;
    RSA* r = NULL;
    PEM_read_RSAPrivateKey(fp, &r, NULL, NULL);
    if (!r)
    {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    
    //秘钥字节长度
    int key_size = RSA_size(r);

    //2 生成PKEY 并创建上下文
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(pkey, r);
    RSA_free(r);
   
    auto ctx = EVP_PKEY_CTX_new(pkey, NULL);

    //生成hash算法上下文
    auto mctx = EVP_MD_CTX_new();
    EVP_SignInit(mctx, EVP_sha512());
    
    //消息生成hash值
    EVP_SignUpdate(mctx, in, in_size);
    unsigned int size = in_size;
    
    //取出hash值并用私钥加密
    EVP_SignFinal(mctx, sign, &size, pkey);

    EVP_MD_CTX_free(mctx);
    EVP_PKEY_free(pkey);
    return size;
}

bool EvpRsaVerify(const unsigned char* in, int in_size, const unsigned char* sign,int sign_size)
{
    //1 读取pem中的公钥
    FILE* fp = fopen(PUBKEY_PEM, "r");
    if (!fp)
        return 0;
    RSA* r = NULL;
    PEM_read_RSAPublicKey(fp, &r, NULL, NULL);
    fclose(fp);

    if (!r)
    {
        ERR_print_errors_fp(stderr);
        return 0;
    }
    //秘钥字节长度
    int key_size = RSA_size(r);

    //2 生成EVP_PKEY 生成
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(pkey, r); //设置为rsa的秘钥
    RSA_free(r);

    //验签 hash算法
    auto mctx = EVP_MD_CTX_new();
    EVP_VerifyInit(mctx, EVP_sha512());
    //生成单向散列
    EVP_VerifyUpdate(mctx, in, in_size);
    
    //公钥解密签名 ，对比生成的单向散列
    int re = EVP_VerifyFinal(mctx,   //上下文中存放单向散列
        sign,               //签名
        sign_size, 
        pkey);              //公钥解密
    EVP_MD_CTX_free(mctx);
    EVP_PKEY_free(pkey);
    if (re == 1)
        return true;

    return false;
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

    //生成签名
    int sign_size = EvpSign(data, data_size, out);
    cout << sign_size << ":" << out << endl;
    //验证签名
    if (EvpRsaVerify(data, data_size, out, sign_size))
    {
        cout << "验签成功" << endl;
    }
    else
        cout << "验签失败" << endl;

    //数据篡改
    data[0] = 'C';
    //out[0] = 'C';
    if (EvpRsaVerify(data, data_size, out, sign_size))
    {
        cout << "验签成功" << endl;
    }
    else
        cout << "验签失败" << endl;


    //生成密钥对
    auto pkey = EvpRsaKey();
    EVP_PKEY_free(pkey);

    //rsa加密
    int len = EvpRsaEncrypt(data, data_size, out);
    cout<<"EvpRsaEncrypt len = "<<len<<endl;
    
    //rsa解密
    len = EvpRsaDecrypt(out, len, out2);
    cout << "EvpRsaDecrypt len = " << len << endl;
    cout << out2 << endl;

    getchar();
    return 0;
}










































