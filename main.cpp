#include <iostream>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

//ctrl + p

#define ENCRYPT "encrypt"
#define DECRYPT "decrypt"

#define CBC "CBC"
#define CTR "CTR"
#define GCM "GCM"

#define encrypt 1
#define decrypt 0

#define cbc 0
#define ctr 1
#define gcm 2


using namespace std;


int en_de_crypt(int should_encrypt, int mode, unsigned char *key, unsigned char *iv, FILE *fIN, FILE *fOUT);

void handleErrors(void);


//TODO
/*cipher mode ( CBC/CTR/GCM/. . . ),
 path to keystore
 key id
 filename
 */

/*
 * Actually key hardcoded
 *
 * */

/* 1param - path to file
 * 2param - encrypt/decrypt
 * 3param - mode (CBC/CTR/GCM/)
 * ...
 * */




int main(int argc, char* argv[]) {

    FILE *fIN, *fOUT;
    char command [10];
    char mode [4];
    int _mode;

    if (argc < 5) {            //TODO 4-5 parameters
        printf("Usage: /path/to/file/ [encrypt/decrypt] [mode]");
        return -1;
    }

    /* A 256 bit key */
    // unsigned char *key = (unsigned char *)"01234567890123456789012345678901";   //TODO

    /* A 128 bit IV */
    //unsigned char *iv = (unsigned char *)"01234567890123456";   //TODO

    //unsigned char key[] = "thiskeyisverybad";
    unsigned char iv[] = "dontusethisinput";
    unsigned char key[256];


    sscanf(argv[2], "%s", command );

    sscanf(argv[3], "%s", mode );

    sscanf(argv[4], "%s", key );


    fIN = fopen(argv[1], "rb");


    if(fIN == NULL)
    {

        printf("Bad /path/to/file/ \n");
        fclose(fIN);
        return -1;
    }



    if(strcmp(mode, CBC) == 0)
        _mode = 0;
    else if(strcmp(mode, CTR) == 0)
        _mode = 1;
    else if(strcmp(mode, GCM) == 0)
        _mode = 2;
    else
    {
        printf("Bad mode '%s'", mode);
        return -1;
    }

    if(strcmp(command, ENCRYPT) == 0)
    {
        printf("Encrypting...\n");
        //fIN = fopen(argv[1], "rb");
        fOUT = fopen("/home/stas/ClionProjects/Kryptografia2.1/Encrypted", "wb");  //todo???


        en_de_crypt(encrypt, _mode, key, iv, fIN, fOUT);//TODO
        printf("Encrypted...\n");
    }

    else if (strcmp(command, DECRYPT) == 0)     //TODO return else if
    {
        printf("Decrypting...\n");
        //fIN = fopen("/home/stas/ClionProjects/Kryptografia2.1/Encrypted", "rb");
        fOUT = fopen("/home/stas/ClionProjects/Kryptografia2.1/Decrypted", "wb");
        en_de_crypt(decrypt, _mode, key, iv, fIN, fOUT);//TODO
        printf("Decrypted...\n");
    }


    else
    {
        printf("Bad command '%s'", command);
        return -1;
    }


    EVP_cleanup();
    ERR_free_strings();


    fclose(fIN);
    fclose(fOUT);


    return 0;
}



int en_de_crypt(int encryptOrDecrypt, int mode, unsigned char *key, unsigned char *iv, FILE *fIN, FILE *fOUT) {
    EVP_CIPHER_CTX *ctx;
    const unsigned BUFSIZE = 4096;
    int cipher_len;

    unsigned char *read_buf = (unsigned char *) malloc(BUFSIZE);
    unsigned char *cipher_buf;
    int blockSize;

    /*Create and initialise context*/

    if (!(ctx = EVP_CIPHER_CTX_new()))           //TODO is necessary???
        handleErrors();

    if(mode == cbc)
        EVP_CipherInit(ctx, EVP_aes_128_cbc(), key, iv, encryptOrDecrypt);
    else if(mode == ctr)
        EVP_CipherInit(ctx, EVP_aes_128_ctr(), key, iv, encryptOrDecrypt);
    else if(mode == gcm)
        EVP_CipherInit(ctx, EVP_aes_128_gcm(), key, iv, encryptOrDecrypt);


    blockSize = EVP_CIPHER_CTX_block_size(ctx);
    cipher_buf = (unsigned char *) malloc(BUFSIZE + blockSize);
    while (1) //TODO change it
    {

        // Read in data in blocks until EOF. Update the ciphering with each read.

        /*
         * size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
         *
         * The  function  fread()  reads  nmemb  elements of data, each size bytes
         * long, from the stream pointed to by stream, storing them at the location
         * given by ptr.
         */

        int numRead = fread(read_buf, sizeof(unsigned char), BUFSIZE, fIN);

        /*int EVP_CipherUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl,
                              const unsigned char *in, int inl)*/

        EVP_CipherUpdate(ctx, cipher_buf, &cipher_len, read_buf, numRead);
        /*
         * size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
         *
         * The  function  fwrite()  writes nmemb elements of data, each size bytes
         * long, to the stream pointed to by stream, obtaining them from the
         * location given by ptr.
         */

        fwrite(cipher_buf, sizeof(unsigned char), BUFSIZE, fOUT);

        if (numRead < BUFSIZE)
            break;
    }


    EVP_CipherFinal(ctx, cipher_buf, &cipher_len);
    fwrite(cipher_buf, sizeof(unsigned char), cipher_len, fOUT);


    EVP_CIPHER_CTX_cleanup(ctx);
    free(cipher_buf);
    free(read_buf);

    return cipher_len;

}



void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();

}
