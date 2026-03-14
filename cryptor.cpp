#include <iostream>
#include <fstream>
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
            data[i] ^= key[i];
        }
    }

    void savekey(const char* filename, long size)
    {
        std::ofstream out(filename, std::ios::binary);
        out.write(reinterpret_cast<char*>(key), size);
        out.close();
    }

    void savedata(const char* filename, unsigned char* data, long size)
    {
        std::ofstream out(filename, std::ios::binary);
        out.write(reinterpret_cast<char*>(data), size);
        out.close();
    }

    void decrypt(unsigned char* data, long size)
    {
        for (long i = 0; i < size; i++)
        {
            data[i] ^= key[i];
        }
    }

    void savedecrypteddata(const char* filename, unsigned char* data, long size)
    {
        std::ofstream out(filename, std::ios::binary);
        out.write(reinterpret_cast<char*>(data), size);
        out.close();
    }
};

int main()
{
    FILE* f = fopen("text.txt", "rb");
    FILE* k = fopen("key.bin", "wb");
    FILE* d = fopen("data.bin", "wb");
    FILE* dc = fopen("decrypted.txt", "wb");

    if (!f || !k || !d || !dc)
    {
        std::cout << "Ошибка открытия файла\n";
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char* data = new unsigned char[size];
    fread(data, 1, size, f);

    Cryptor cryptor;
    cryptor.encrypt(data, size);
    cryptor.savekey("key.bin", size);
    cryptor.savedata("data.bin", data, size);
    cryptor.decrypt(data, size);
    cryptor.savedecrypteddata("decrypted.txt", data, size);

    fclose(k);
    fclose(d);
    fclose(f);
    fclose(dc);
    delete[] data;
    return 0;
}