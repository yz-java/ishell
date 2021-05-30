#include "aes.h"
#include <string.h>
#include <QFile>

AES::AES()
{

}

const string AES::aes_cbc(string data,string key,int type){
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv,0,AES_BLOCK_SIZE);
    unsigned char aes_keybuf[32];
    memset(aes_keybuf, 0, sizeof (aes_keybuf));
    strcpy((char *)aes_keybuf, key.c_str());
    AES_KEY aeskey;
    if(type==AES_ENCRYPT){
        AES_set_encrypt_key(aes_keybuf, 256, &aeskey);
    }

    if(type==AES_DECRYPT){
        AES_set_decrypt_key(aes_keybuf, 256, &aeskey);
    }

    unsigned int data_length = data.length();
    // ZeroPadding
    int padding = 0;
    if (data_length % (AES_BLOCK_SIZE) > 0)
    {
        padding = AES_BLOCK_SIZE - data_length % (AES_BLOCK_SIZE);
    }
    data_length += padding;
    while (padding > 0)
    {
        data += '\0';
        padding--;
    }

    string outData;
    const char* d =data.data();
    char* block_char=new char[AES_BLOCK_SIZE];
    for (unsigned int i = 0; i < data_length / (AES_BLOCK_SIZE); i++) {
        int offset=i*AES_BLOCK_SIZE;
        memcpy(block_char,d+offset,AES_BLOCK_SIZE);
        unsigned char out[AES_BLOCK_SIZE];
        if(type==AES_ENCRYPT){
            AES_cbc_encrypt((const unsigned char*)block_char,out,AES_BLOCK_SIZE,&aeskey,iv,type);
        }

        if(type==AES_DECRYPT){
             AES_cbc_encrypt((const unsigned char*)block_char,out,AES_BLOCK_SIZE,&aeskey,iv,type);

        }
        string res=string((char*)out,AES_BLOCK_SIZE);
        outData+=res;

    }
//    if(type==AES_ENCRYPT){
//        int length=outData.size();
//        outData=bytestohexstring((char*)outData.data(),length);
//    }


    return outData;
}

int AES::aesEncryptFile(QString in_file_path, QString out_file_path,const char Key[32]){

    int encrypt_chunk_size = 16;

    QFile fin(in_file_path);
    if(!fin.exists()){
        cout<< "文件不存在" << endl;
        return 0;
    }
    bool isOk=fin.open(QIODevice::ReadOnly);
    if(!isOk){
        cout<< "文件打开失败" << endl;
        return 0;
    }

    QFile fout(out_file_path);
    bool isopen=fout.open(QIODevice::WriteOnly);
    if(!isopen){
        cout<< "输出文件打开失败" << endl;
        return 0;
    }


    //用指定密钥对一段内存进行加密，结果放在outbuffer中
    unsigned char aes_keybuf[32];
    memset(aes_keybuf, 0, sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, Key);
    AES_KEY aeskey;
    AES_set_encrypt_key(aes_keybuf, 256, &aeskey);

    char *out_data=new char[encrypt_chunk_size];
    char *data=new char[encrypt_chunk_size];
    int fileSize=fin.size();
    while (fileSize>0) {
        int read_size=fin.read(data,encrypt_chunk_size);
        AES_decrypt((const unsigned char *)out_data, (unsigned char *)out_data, &aeskey);
        fout.write(out_data);
        fileSize-=read_size;
    }



    fout.close();
    fin.close();

    return 1;
}

int AES::aesDecryptFile(QString in_file_path, QString out_file_path,const char Key[32]){

    int encrypt_chunk_size = 16;

    QFile fin(in_file_path);
    if(!fin.exists()){
        cout<< "文件不存在" << endl;
        return 0;
    }
    bool isOk=fin.open(QIODevice::ReadOnly);
    if(!isOk){
        cout<< "文件打开失败" << endl;
        return 0;
    }

    QFile fout(out_file_path);
    bool isopen=fout.open(QIODevice::WriteOnly);
    if(!isopen){
        cout<< "输出文件打开失败" << endl;
        return 0;
    }


    //用指定密钥对一段内存进行加密，结果放在outbuffer中
    unsigned char aes_keybuf[32];
    memset(aes_keybuf, 0, sizeof(aes_keybuf));
    strcpy((char *)aes_keybuf, Key);
    AES_KEY aeskey;
    AES_set_encrypt_key(aes_keybuf, 256, &aeskey);

    char *out_data=new char[encrypt_chunk_size];
    char *data=new char[encrypt_chunk_size];
    int fileSize=fin.size();
    while (fileSize>0) {
        int read_size=fin.read(data,encrypt_chunk_size);
        AES_encrypt((const unsigned char *)out_data, (unsigned char *)out_data, &aeskey);
        fout.write(out_data);
        fileSize-=read_size;
    }



    fout.close();
    fin.close();

    return 1;
}
