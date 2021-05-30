#ifndef AES_H
#define AES_H
#include <openssl/aes.h>
#include <iostream>
#include <QString>
#include <sstream>

using namespace std;
class AES
{
public:
    AES();

    static const string aes_cbc(string data,string key,int type);

    static int aesEncryptFile(QString in_file_path, QString out_file_path,const char Key[32]);

    static int aesDecryptFile(QString in_file_path, QString out_file_path,const char Key[32]);

private:

};

#endif // AES_H
