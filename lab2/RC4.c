
#ifndef _SYS_CRYPTO_RC4_RC4_H_
#define _SYS_CRYPTO_RC4_RC4_H_

struct rc4_state {
    u_char  perm[256];
    u_char  index1;
    u_char  index2;
};

extern void rc4_init(struct rc4_state *state, const u_char *key, int keylen);
extern void rc4_crypt(struct rc4_state *state,
        const u_char *inbuf, u_char *outbuf, int buflen);

#endif


#include <sys/types.h>
#include <crypto/rc4/rc4.h>

static __inline void
swap_bytes(u_char *a, u_char *b)
{
    u_char temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * Initialize an RC4 state buffer using the supplied key,
 * which can have arbitrary length.
 */
void
rc4_init(struct rc4_state *const state, const u_char *key, int keylen)
{
    u_char j;
    int i;

    /* Initialize state with identity permutation */
    for (i = 0; i < 256; i++)
        state->perm[i] = (u_char)i; 
    state->index1 = 0;
    state->index2 = 0;

    /* Randomize the permutation using key data */
    for (j = i = 0; i < 256; i++) {
        j += state->perm[i] + key[i % keylen]; 
        swap_bytes(&state->perm[i], &state->perm[j]);
    }
}

/*
 * Encrypt some data using the supplied RC4 state buffer.
 * The input and output buffers may be the same buffer.
 * Since RC4 is a stream cypher, this function is used
 * for both encryption and decryption.
 */
void
rc4_crypt(struct rc4_state *const state,
    const u_char *inbuf, u_char *outbuf, int buflen)
{
    int i;
    u_char j;

    for (i = 0; i < buflen; i++) {

        /* Update modification indicies */
        state->index1++;
        state->index2 += state->perm[state->index1];

        /* Modify permutation */
        swap_bytes(&state->perm[state->index1],
            &state->perm[state->index2]);

        /* Encrypt/decrypt next byte */
        j = state->perm[state->index1] + state->perm[state->index2];
        outbuf[i] = inbuf[i] ^ state->perm[j];
    }
}

 int main(int argc, char* argv[]){

    int dataLength = 8;
    int keyLength = 8;
    const unsigned char dataStream[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    printf("before\n");
    for (int i = 0; i < dataLength ; i++) {
        printf("%x,",dataStream[i]);
    }
    printf("\n");
    unsigned char encryp[dataLength];
    unsigned char decryp[dataLength];
    unsigned char key[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    struct rc4_state state;

    rc4_init(&state, key, keyLength);// this code is very important

    rc4_crypt(&state, dataStream, encryp, dataLength);
    printf("\nencrypt\n");
    for (int i = 0; i < dataLength ; i++) {
        printf("%x,",encryp[i]);
    }
    printf("\nafter \n");
    rc4_init(&state, key, keyLength);// this code is very important
    rc4_crypt(&state, encryp, decryp, dataLength);
    for (int i = 0; i < dataLength ; i++) {
        printf("%x,",decryp[i]);
    }
    printf("\n");
    return 0;
}
