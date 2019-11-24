/**
 * Copyright (c) 2017, Armando Faz <armfazh@ic.unicamp.br>. All rights reserved.
 * Institute of Computing.
 * University of Campinas, Brazil.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of University of Campinas nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "random.h"
#include "gtest/gtest.h"
#include <cstring>
#include <iomanip>
#include <iostream>
#include <rfc7748_precomputed.h>

static std::ostream &operator<<(std::ostream &os, const X448_KEY &key) {
  int i = 0;
  for (i = 0; i < X448_KEYSIZE_BYTES; i++) {
    os << std::setbase(16) << std::setfill('0') << std::setw(2)
       << static_cast<int>(key[i]);
  }
  return os << std::endl;
}

static void random_X448_key(argKey key) {
  random_bytes(key, X448_KEYSIZE_BYTES);
}

static void times(int n, uint8_t *k) {
  X448_KEY r, u;
  int i;
  for (i = 0; i < X448_KEYSIZE_BYTES; i++) {
    u[i] = 0;
    k[i] = 0;
    r[i] = 0;
  }
  u[0] = 5;
  k[0] = 5;

  for (i = 0; i < n; i++) {
    X448_Shared(r, u, k);
    memcpy(u, k, X448_KEYSIZE_BYTES);
    memcpy(k, r, X448_KEYSIZE_BYTES);
  }
}

TEST(X448, IETF_CFRG0) {
  X448_KEY shared;

  X448_KEY key0 = {0x3d, 0x26, 0x2f, 0xdd, 0xf9, 0xec, 0x8e, 0x88, 0x49, 0x52,
                   0x66, 0xfe, 0xa1, 0x9a, 0x34, 0xd2, 0x88, 0x82, 0xac, 0xef,
                   0x04, 0x51, 0x04, 0xd0, 0xd1, 0xaa, 0xe1, 0x21, 0x70, 0x0a,
                   0x77, 0x9c, 0x98, 0x4c, 0x24, 0xf8, 0xcd, 0xd7, 0x8f, 0xbf,
                   0xf4, 0x49, 0x43, 0xeb, 0xa3, 0x68, 0xf5, 0x4b, 0x29, 0x25,
                   0x9a, 0x4f, 0x1c, 0x60, 0x0a, 0xd3};

  X448_KEY input_coord0 = {
      0x06, 0xfc, 0xe6, 0x40, 0xfa, 0x34, 0x87, 0xbf, 0xda, 0x5f, 0x6c, 0xf2,
      0xd5, 0x26, 0x3f, 0x8a, 0xad, 0x88, 0x33, 0x4c, 0xbd, 0x07, 0x43, 0x7f,
      0x02, 0x0f, 0x08, 0xf9, 0x81, 0x4d, 0xc0, 0x31, 0xdd, 0xbd, 0xc3, 0x8c,
      0x19, 0xc6, 0xda, 0x25, 0x83, 0xfa, 0x54, 0x29, 0xdb, 0x94, 0xad, 0xa1,
      0x8a, 0xa7, 0xa7, 0xfb, 0x4e, 0xf8, 0xa0, 0x86};

  X448_KEY output_coord0 = {
      0xce, 0x3e, 0x4f, 0xf9, 0x5a, 0x60, 0xdc, 0x66, 0x97, 0xda, 0x1d, 0xb1,
      0xd8, 0x5e, 0x6a, 0xfb, 0xdf, 0x79, 0xb5, 0x0a, 0x24, 0x12, 0xd7, 0x54,
      0x6d, 0x5f, 0x23, 0x9f, 0xe1, 0x4f, 0xba, 0xad, 0xeb, 0x44, 0x5f, 0xc6,
      0x6a, 0x01, 0xb0, 0x77, 0x9d, 0x98, 0x22, 0x39, 0x61, 0x11, 0x1e, 0x21,
      0x76, 0x62, 0x82, 0xf7, 0x3d, 0xd9, 0x6b, 0x6f};
  X448_Shared(shared, input_coord0, key0);
  EXPECT_EQ(memcmp(shared, output_coord0, X448_KEYSIZE_BYTES), 0)
      << "got:  " << shared << "want: " << output_coord0;

  X448_KEY key1 = {0x20, 0x3d, 0x49, 0x44, 0x28, 0xb8, 0x39, 0x93, 0x52, 0x66,
                   0x5d, 0xdc, 0xa4, 0x2f, 0x9d, 0xe8, 0xfe, 0xf6, 0x00, 0x90,
                   0x8e, 0x0d, 0x46, 0x1c, 0xb0, 0x21, 0xf8, 0xc5, 0x38, 0x34,
                   0x5d, 0xd7, 0x7c, 0x3e, 0x48, 0x06, 0xe2, 0x5f, 0x46, 0xd3,
                   0x31, 0x5c, 0x44, 0xe0, 0xa5, 0xb4, 0x37, 0x12, 0x82, 0xdd,
                   0x2c, 0x8d, 0x5b, 0xe3, 0x09, 0x5f};

  X448_KEY input_coord1 = {
      0x0f, 0xbc, 0xc2, 0xf9, 0x93, 0xcd, 0x56, 0xd3, 0x30, 0x5b, 0x0b, 0x7d,
      0x9e, 0x55, 0xd4, 0xc1, 0xa8, 0xfb, 0x5d, 0xbb, 0x52, 0xf8, 0xe9, 0xa1,
      0xe9, 0xb6, 0x20, 0x1b, 0x16, 0x5d, 0x01, 0x58, 0x94, 0xe5, 0x6c, 0x4d,
      0x35, 0x70, 0xbe, 0xe5, 0x2f, 0xe2, 0x05, 0xe2, 0x8a, 0x78, 0xb9, 0x1c,
      0xdf, 0xbd, 0xe7, 0x1c, 0xe8, 0xd1, 0x57, 0xdb};

  X448_KEY output_coord1 = {
      0x88, 0x4a, 0x02, 0x57, 0x62, 0x39, 0xff, 0x7a, 0x2f, 0x2f, 0x63, 0xb2,
      0xdb, 0x6a, 0x9f, 0xf3, 0x70, 0x47, 0xac, 0x13, 0x56, 0x8e, 0x1e, 0x30,
      0xfe, 0x63, 0xc4, 0xa7, 0xad, 0x1b, 0x3e, 0xe3, 0xa5, 0x70, 0x0d, 0xf3,
      0x43, 0x21, 0xd6, 0x20, 0x77, 0xe6, 0x36, 0x33, 0xc5, 0x75, 0xc1, 0xc9,
      0x54, 0x51, 0x4e, 0x99, 0xda, 0x7c, 0x17, 0x9d};

  X448_Shared(shared, input_coord1, key1);
  EXPECT_EQ(memcmp(shared, output_coord1, X448_KEYSIZE_BYTES), 0)
      << "got:  " << shared << "want: " << output_coord1;

  X448_KEY alice_sk = {
      0x9a, 0x8f, 0x49, 0x25, 0xd1, 0x51, 0x9f, 0x57, 0x75, 0xcf, 0x46, 0xb0,
      0x4b, 0x58, 0x00, 0xd4, 0xee, 0x9e, 0xe8, 0xba, 0xe8, 0xbc, 0x55, 0x65,
      0xd4, 0x98, 0xc2, 0x8d, 0xd9, 0xc9, 0xba, 0xf5, 0x74, 0xa9, 0x41, 0x97,
      0x44, 0x89, 0x73, 0x91, 0x00, 0x63, 0x82, 0xa6, 0xf1, 0x27, 0xab, 0x1d,
      0x9a, 0xc2, 0xd8, 0xc0, 0xa5, 0x98, 0x72, 0x6b};

  X448_KEY alice_pk = {
      0x9b, 0x08, 0xf7, 0xcc, 0x31, 0xb7, 0xe3, 0xe6, 0x7d, 0x22, 0xd5, 0xae,
      0xa1, 0x21, 0x07, 0x4a, 0x27, 0x3b, 0xd2, 0xb8, 0x3d, 0xe0, 0x9c, 0x63,
      0xfa, 0xa7, 0x3d, 0x2c, 0x22, 0xc5, 0xd9, 0xbb, 0xc8, 0x36, 0x64, 0x72,
      0x41, 0xd9, 0x53, 0xd4, 0x0c, 0x5b, 0x12, 0xda, 0x88, 0x12, 0x0d, 0x53,
      0x17, 0x7f, 0x80, 0xe5, 0x32, 0xc4, 0x1f, 0xa0};
  X448_KEY alice_public;

  X448_KeyGen(alice_public, alice_sk);
  EXPECT_EQ(memcmp(alice_public, alice_pk, X448_KEYSIZE_BYTES), 0)
      << "got:  " << alice_public << "want: " << alice_pk;

  X448_KEY bob_sk = {0x1c, 0x30, 0x6a, 0x7a, 0xc2, 0xa0, 0xe2, 0xe0, 0x99, 0x0b,
                     0x29, 0x44, 0x70, 0xcb, 0xa3, 0x39, 0xe6, 0x45, 0x37, 0x72,
                     0xb0, 0x75, 0x81, 0x1d, 0x8f, 0xad, 0x0d, 0x1d, 0x69, 0x27,
                     0xc1, 0x20, 0xbb, 0x5e, 0xe8, 0x97, 0x2b, 0x0d, 0x3e, 0x21,
                     0x37, 0x4c, 0x9c, 0x92, 0x1b, 0x09, 0xd1, 0xb0, 0x36, 0x6f,
                     0x10, 0xb6, 0x51, 0x73, 0x99, 0x2d};

  X448_KEY bob_pk = {0x3e, 0xb7, 0xa8, 0x29, 0xb0, 0xcd, 0x20, 0xf5, 0xbc, 0xfc,
                     0x0b, 0x59, 0x9b, 0x6f, 0xec, 0xcf, 0x6d, 0xa4, 0x62, 0x71,
                     0x07, 0xbd, 0xb0, 0xd4, 0xf3, 0x45, 0xb4, 0x30, 0x27, 0xd8,
                     0xb9, 0x72, 0xfc, 0x3e, 0x34, 0xfb, 0x42, 0x32, 0xa1, 0x3c,
                     0xa7, 0x06, 0xdc, 0xb5, 0x7a, 0xec, 0x3d, 0xae, 0x07, 0xbd,
                     0xc1, 0xc6, 0x7b, 0xf3, 0x36, 0x09};

  X448_KEY bob_public;

  X448_KeyGen(bob_public, bob_sk);
  EXPECT_EQ(memcmp(bob_public, bob_pk, X448_KEYSIZE_BYTES), 0)
      << "got:  " << bob_public << "want: " << bob_pk;

  X448_KEY shared_secret = {
      0x07, 0xff, 0xf4, 0x18, 0x1a, 0xc6, 0xcc, 0x95, 0xec, 0x1c, 0x16, 0xa9,
      0x4a, 0x0f, 0x74, 0xd1, 0x2d, 0xa2, 0x32, 0xce, 0x40, 0xa7, 0x75, 0x52,
      0x28, 0x1d, 0x28, 0x2b, 0xb6, 0x0c, 0x0b, 0x56, 0xfd, 0x24, 0x64, 0xc3,
      0x35, 0x54, 0x39, 0x36, 0x52, 0x1c, 0x24, 0x40, 0x30, 0x85, 0xd5, 0x9a,
      0x44, 0x9a, 0x50, 0x37, 0x51, 0x4a, 0x87, 0x9d};

  X448_KEY alice_shared_secret;
  X448_KEY bob_shared_secret;

  X448_Shared(bob_shared_secret, alice_public, bob_sk);
  X448_Shared(alice_shared_secret, bob_public, alice_sk);
  EXPECT_EQ(memcmp(alice_shared_secret, shared_secret, X448_KEYSIZE_BYTES), 0)
      << "got:  " << alice_shared_secret << "want: " << shared_secret;
  EXPECT_EQ(memcmp(bob_shared_secret, shared_secret, X448_KEYSIZE_BYTES), 0)
      << "got:  " << bob_shared_secret << "want: " << shared_secret;
}

TEST(X448, IETF_CFRG1_0) {
  X448_KEY k;
  X448_KEY k_1_times = {
      0x3f, 0x48, 0x2c, 0x8a, 0x9f, 0x19, 0xb0, 0x1e, 0x6c, 0x46, 0xee, 0x97,
      0x11, 0xd9, 0xdc, 0x14, 0xfd, 0x4b, 0xf6, 0x7a, 0xf3, 0x07, 0x65, 0xc2,
      0xae, 0x2b, 0x84, 0x6a, 0x4d, 0x23, 0xa8, 0xcd, 0x0d, 0xb8, 0x97, 0x08,
      0x62, 0x39, 0x49, 0x2c, 0xaf, 0x35, 0x0b, 0x51, 0xf8, 0x33, 0x86, 0x8b,
      0x9b, 0xc2, 0xb3, 0xbc, 0xa9, 0xcf, 0x41, 0x13};
  times(1, k);
  EXPECT_EQ(memcmp(k, k_1_times, X448_KEYSIZE_BYTES), 0)
      << "got:  " << k << "want: " << k_1_times;
}

TEST(X448, IETF_CFRG1_1) {
  X448_KEY k;
  X448_KEY k_1000_times = {
      0xaa, 0x3b, 0x47, 0x49, 0xd5, 0x5b, 0x9d, 0xaf, 0x1e, 0x5b, 0x00, 0x28,
      0x88, 0x26, 0xc4, 0x67, 0x27, 0x4c, 0xe3, 0xeb, 0xbd, 0xd5, 0xc1, 0x7b,
      0x97, 0x5e, 0x09, 0xd4, 0xaf, 0x6c, 0x67, 0xcf, 0x10, 0xd0, 0x87, 0x20,
      0x2d, 0xb8, 0x82, 0x86, 0xe2, 0xb7, 0x9f, 0xce, 0xea, 0x3e, 0xc3, 0x53,
      0xef, 0x54, 0xfa, 0xa2, 0x6e, 0x21, 0x9f, 0x38};
  times(1000, k);
  EXPECT_EQ(memcmp(k, k_1000_times, X448_KEYSIZE_BYTES), 0)
      << "got:  " << k << "want: " << k_1000_times;
}

TEST(X448, DISABLED_IETF_CFRG1_2) {
  X448_KEY k;
  X448_KEY k_1000000_times = {
      0x07, 0x7f, 0x45, 0x36, 0x81, 0xca, 0xca, 0x36, 0x93, 0x19, 0x84, 0x20,
      0xbb, 0xe5, 0x15, 0xca, 0xe0, 0x00, 0x24, 0x72, 0x51, 0x9b, 0x3e, 0x67,
      0x66, 0x1a, 0x7e, 0x89, 0xca, 0xb9, 0x46, 0x95, 0xc8, 0xf4, 0xbc, 0xd6,
      0x6e, 0x61, 0xb9, 0xb9, 0xc9, 0x46, 0xda, 0x8d, 0x52, 0x4d, 0xe3, 0xd6,
      0x9b, 0xd9, 0xd9, 0xd6, 0x6b, 0x99, 0x7e, 0x37};
  times(1000000, k);
  EXPECT_EQ(memcmp(k, k_1000000_times, X448_KEYSIZE_BYTES), 0)
      << "got:  " << k << "want: " << k_1000000_times;
}

TEST(X448, DIFFIE_HELLMAN) {
  int64_t i = 0, TIMES = 1000;
  int64_t cnt = 0;

  for (i = 0; i < TIMES; i++) {
    X448_KEY alicesk, alicepk;
    random_X448_key(alicesk);
    X448_KeyGen(alicepk, alicesk);

    X448_KEY bobsk, bobpk;
    random_X448_key(bobsk);
    X448_KeyGen(bobpk, bobsk);

    X448_KEY alice_shared_secret;
    X448_KEY bob_shared_secret;
    X448_Shared(alice_shared_secret, bobpk, alicesk);
    X448_Shared(bob_shared_secret, alicepk, bobsk);
    EXPECT_EQ(
        memcmp(alice_shared_secret, bob_shared_secret, X448_KEYSIZE_BYTES), 0)
        << "got:  " << alice_shared_secret << "want: " << bob_shared_secret;
    cnt++;
  }
  EXPECT_EQ(cnt, TIMES) << "passed: " << cnt << "/" << TIMES << std::endl;
}
