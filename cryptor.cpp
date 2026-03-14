#include <iostream>
#include <fstream>
#include <string>
#include <openssl/rand.h>

class Cryptor
{
private:
    unsigned char key[4096];

public:
    Cryptor()
    {
        RAND_bytes(key, 4096);
    }

    void encrypt(unsigned char* data, long size)
    {
        for (long i = 0; i < size; i++)
        {
            data[i] ^= key[i % 4096];
        }
    }

    void savekey(const char* filename, long size)
    {
        std::ofstream out(filename, std::ios::binary);
        out.write(reinterpret_cast<char*>(key), size);
        out.close();
    }

    void savedata(const char* filename, const char* data, long size)
    {
        std::ofstream out(filename, std::ios::binary);
        out.write(data, size);
        out.close();
    }

    void decrypt(unsigned char* data, long size)
    {
        for (long i = 0; i < size; i++)
        {
            data[i] ^= key[i % 4096];
        }
    }

    void savedecrypteddata(const char* filename, const char* data, long size)
    {
        std::ofstream out(filename, std::ios::binary);
        out.write(data, size);
        out.close();
    }
};

int main(int argc, char *argv[])
{
    std::string filename;
    if(argc < 2)
    {
        std::cout << "Вы не указали путь к файлу\n";
        return 1;
    }

    filename = argv[1];

    std::ifstream f(filename, std::ios::binary);
    if (!f)
    {
        std::cout << "Ошибка открытия файла\n";
        return 1;
    }

    std::ofstream k("key.bin", std::ios::binary);
    if (!k)
    {
        std::cout << "Ошибка открытия файла\n";
        return 1;
    }

    std::ofstream d("data.bin", std::ios::binary);
    if (!d)
    {
        std::cout << "Ошибка открытия файла\n";
        return 1;
    }

    std::ofstream dc("decrypted.txt");
    if (!dc)
    {
        std::cout << "Ошибка открытия файла\n";
        return 1;
    }

    std::string data;
    std::string line;

    while(getline(f, line))
    {
        data += line;
        data += "\n";
    }

    long size = data.length();
    char* row_data = &data[0];

    Cryptor cryptor;

    cryptor.encrypt(reinterpret_cast<unsigned char*>(row_data), size);
    cryptor.savekey("key.bin", size);
    cryptor.savedata("data.bin", data.c_str(), size);

    cryptor.decrypt(reinterpret_cast<unsigned char*>(row_data), size);
    cryptor.savedecrypteddata("decrypted.txt", data.c_str(), size);

    return 0;
}