#include <stdint.h>
#include <stdio.h>

void encrypt(uint8_t *tx_buff, size_t size) {
  uint8_t key_1[] = {34, 20, 41, 97, 62, 66, 40, 80,
                     36, 82, 53, 72, 88, 79, 1,  11};
  uint8_t key_2[] = {27, 25, 18, 48, 49, 32, 36, 30,
                     44, 40, 5,  17, 88, 24, 9,  38};
  uint8_t key_3[] = {35, 28, 24, 43, 21, 97, 39, 41,
   98, 15, 99, 59, 66, 4,  97, 47};

  for (size_t i = 0; i < size; i++) {
    tx_buff[i] = tx_buff[i] ^ key_1[i];
  }
  for (size_t i = 0; i < size; i++) {
    tx_buff[i] = tx_buff[i] ^ key_2[i];
  }

  for (size_t i = 0; i < size; i++) {
    tx_buff[i] = tx_buff[i] ^ key_3[i];
  }

  for (size_t i = 0; i < size; i++) {
    tx_buff[i] = ~(tx_buff[i]); // NOR operation
  }
}

void decrypt(uint8_t *tx_buff, size_t size){
  uint8_t key_1[] = {34, 20, 41, 97, 62, 66, 40, 80,
                     36, 82, 53, 72, 88, 79, 1,  11};
  uint8_t key_2[] = {27, 25, 18, 48, 49, 32, 36, 30,
                     44, 40, 5,  17, 88, 24, 9,  38};
  uint8_t key_3[] = {35, 28, 24, 43, 21, 97, 39, 41,
   98, 15, 99, 59, 66, 4,  97, 47};

  for (size_t i = 0; i < size; i++) {
    tx_buff[i] = ~(tx_buff[i]); // NOR operation
  }

  for (size_t i = 0; i < size; i++) {
    tx_buff[i] = tx_buff[i] ^ key_3[i];
  }
  for (size_t i = 0; i < size; i++) {
    tx_buff[i] = tx_buff[i] ^ key_2[i];
  }

  for (size_t i = 0; i < size; i++) {
    tx_buff[i] = tx_buff[i] ^ key_1[i];
  }
}
