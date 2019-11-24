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

static std::ostream &operator<<(std::ostream &os, const X25519_KEY &key) {
  int i = 0;
  for (i = 0; i < X25519_KEYSIZE_BYTES; i++) {
    os << std::setbase(16) << std::setfill('0') << std::setw(2)
       << static_cast<int>(key[i]);
  }
  return os << std::endl;
}

static void random_X25519_key(argKey key) {
  random_bytes(key, X25519_KEYSIZE_BYTES);
}

static void times(int n, uint8_t *k) {
  X25519_KEY r, u;
  int i;
  for (i = 0; i < X25519_KEYSIZE_BYTES; i++) {
    u[i] = 0;
    k[i] = 0;
    r[i] = 0;
  }
  u[0] = 9;
  k[0] = 9;

  for (i = 0; i < n; i++) {
    X25519_Shared(r, u, k);
    memcpy(u, k, X25519_KEYSIZE_BYTES);
    memcpy(k, r, X25519_KEYSIZE_BYTES);
  }
}

TEST(X25519, NACL_CRYPTO) {
  X25519_KEY alicesk = {0x77, 0x07, 0x6d, 0x0a, 0x73, 0x18, 0xa5, 0x7d,
                        0x3c, 0x16, 0xc1, 0x72, 0x51, 0xb2, 0x66, 0x45,
                        0xdf, 0x4c, 0x2f, 0x87, 0xeb, 0xc0, 0x99, 0x2a,
                        0xb1, 0x77, 0xfb, 0xa5, 0x1d, 0xb9, 0x2c, 0x2a};
  X25519_KEY alicepk;

  X25519_KeyGen(alicepk, alicesk);

  X25519_KEY bobsk = {0x5d, 0xab, 0x08, 0x7e, 0x62, 0x4a, 0x8a, 0x4b,
                      0x79, 0xe1, 0x7f, 0x8b, 0x83, 0x80, 0x0e, 0xe6,
                      0x6f, 0x3b, 0xb1, 0x29, 0x26, 0x18, 0xb6, 0xfd,
                      0x1c, 0x2f, 0x8b, 0x27, 0xff, 0x88, 0xe0, 0xeb};
  X25519_KEY bobpk;

  X25519_KeyGen(bobpk, bobsk);
  X25519_KEY alice_shared_secret;
  X25519_KEY bob_shared_secret;
  X25519_Shared(alice_shared_secret, bobpk, alicesk);
  X25519_Shared(bob_shared_secret, alicepk, bobsk);

  X25519_KEY shared = {0x4a, 0x5d, 0x9d, 0x5b, 0xa4, 0xce, 0x2d, 0xe1,
                       0x72, 0x8e, 0x3b, 0xf4, 0x80, 0x35, 0x0f, 0x25,
                       0xe0, 0x7e, 0x21, 0xc9, 0x47, 0xd1, 0x9e, 0x33,
                       0x76, 0xf0, 0x9b, 0x3c, 0x1e, 0x16, 0x17, 0x42};
  EXPECT_EQ(memcmp(alice_shared_secret, shared, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << alice_shared_secret << "want: " << shared;
  EXPECT_EQ(memcmp(bob_shared_secret, shared, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << alice_shared_secret << "want: " << shared;
}

TEST(X25519, IETF_CFRG0) {
  X25519_KEY shared;

  X25519_KEY key0 = {0xa5, 0x46, 0xe3, 0x6b, 0xf0, 0x52, 0x7c, 0x9d,
                     0x3b, 0x16, 0x15, 0x4b, 0x82, 0x46, 0x5e, 0xdd,
                     0x62, 0x14, 0x4c, 0x0a, 0xc1, 0xfc, 0x5a, 0x18,
                     0x50, 0x6a, 0x22, 0x44, 0xba, 0x44, 0x9a, 0xc4};

  X25519_KEY input_coord0 = {0xe6, 0xdb, 0x68, 0x67, 0x58, 0x30, 0x30, 0xdb,
                             0x35, 0x94, 0xc1, 0xa4, 0x24, 0xb1, 0x5f, 0x7c,
                             0x72, 0x66, 0x24, 0xec, 0x26, 0xb3, 0x35, 0x3b,
                             0x10, 0xa9, 0x03, 0xa6, 0xd0, 0xab, 0x1c, 0x4c};

  X25519_KEY output_coord0 = {0xc3, 0xda, 0x55, 0x37, 0x9d, 0xe9, 0xc6, 0x90,
                              0x8e, 0x94, 0xea, 0x4d, 0xf2, 0x8d, 0x08, 0x4f,
                              0x32, 0xec, 0xcf, 0x03, 0x49, 0x1c, 0x71, 0xf7,
                              0x54, 0xb4, 0x07, 0x55, 0x77, 0xa2, 0x85, 0x52};

  X25519_Shared(shared, input_coord0, key0);
  EXPECT_EQ(memcmp(shared, output_coord0, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << shared << "want: " << output_coord0;

  X25519_KEY key1 = {0x4b, 0x66, 0xe9, 0xd4, 0xd1, 0xb4, 0x67, 0x3c,
                     0x5a, 0xd2, 0x26, 0x91, 0x95, 0x7d, 0x6a, 0xf5,
                     0xc1, 0x1b, 0x64, 0x21, 0xe0, 0xea, 0x01, 0xd4,
                     0x2c, 0xa4, 0x16, 0x9e, 0x79, 0x18, 0xba, 0x0d};

  X25519_KEY input_coord1 = {0xe5, 0x21, 0x0f, 0x12, 0x78, 0x68, 0x11, 0xd3,
                             0xf4, 0xb7, 0x95, 0x9d, 0x05, 0x38, 0xae, 0x2c,
                             0x31, 0xdb, 0xe7, 0x10, 0x6f, 0xc0, 0x3c, 0x3e,
                             0xfc, 0x4c, 0xd5, 0x49, 0xc7, 0x15, 0xa4, 0x93};

  X25519_KEY output_coord1 = {0x95, 0xcb, 0xde, 0x94, 0x76, 0xe8, 0x90, 0x7d,
                              0x7a, 0xad, 0xe4, 0x5c, 0xb4, 0xb8, 0x73, 0xf8,
                              0x8b, 0x59, 0x5a, 0x68, 0x79, 0x9f, 0xa1, 0x52,
                              0xe6, 0xf8, 0xf7, 0x64, 0x7a, 0xac, 0x79, 0x57};

  X25519_Shared(shared, input_coord1, key1);
  EXPECT_EQ(memcmp(shared, output_coord1, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << shared << "want: " << output_coord1;

  X25519_KEY alice_sk = {0x77, 0x07, 0x6d, 0x0a, 0x73, 0x18, 0xa5, 0x7d,
                         0x3c, 0x16, 0xc1, 0x72, 0x51, 0xb2, 0x66, 0x45,
                         0xdf, 0x4c, 0x2f, 0x87, 0xeb, 0xc0, 0x99, 0x2a,
                         0xb1, 0x77, 0xfb, 0xa5, 0x1d, 0xb9, 0x2c, 0x2a};

  X25519_KEY alice_pk = {0x85, 0x20, 0xf0, 0x09, 0x89, 0x30, 0xa7, 0x54,
                         0x74, 0x8b, 0x7d, 0xdc, 0xb4, 0x3e, 0xf7, 0x5a,
                         0x0d, 0xbf, 0x3a, 0x0d, 0x26, 0x38, 0x1a, 0xf4,
                         0xeb, 0xa4, 0xa9, 0x8e, 0xaa, 0x9b, 0x4e, 0x6a};
  X25519_KEY alice_public;

  X25519_KeyGen(alice_public, alice_sk);
  EXPECT_EQ(memcmp(alice_public, alice_pk, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << alice_public << "want: " << alice_pk;

  X25519_KEY bob_sk = {0x5d, 0xab, 0x08, 0x7e, 0x62, 0x4a, 0x8a, 0x4b,
                       0x79, 0xe1, 0x7f, 0x8b, 0x83, 0x80, 0x0e, 0xe6,
                       0x6f, 0x3b, 0xb1, 0x29, 0x26, 0x18, 0xb6, 0xfd,
                       0x1c, 0x2f, 0x8b, 0x27, 0xff, 0x88, 0xe0, 0xeb};

  X25519_KEY bob_pk = {0xde, 0x9e, 0xdb, 0x7d, 0x7b, 0x7d, 0xc1, 0xb4,
                       0xd3, 0x5b, 0x61, 0xc2, 0xec, 0xe4, 0x35, 0x37,
                       0x3f, 0x83, 0x43, 0xc8, 0x5b, 0x78, 0x67, 0x4d,
                       0xad, 0xfc, 0x7e, 0x14, 0x6f, 0x88, 0x2b, 0x4f};

  X25519_KEY bob_public;

  X25519_KeyGen(bob_public, bob_sk);
  EXPECT_EQ(memcmp(bob_public, bob_pk, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << bob_public << "want: " << bob_pk;

  X25519_KEY shared_secret = {0x4a, 0x5d, 0x9d, 0x5b, 0xa4, 0xce, 0x2d, 0xe1,
                              0x72, 0x8e, 0x3b, 0xf4, 0x80, 0x35, 0x0f, 0x25,
                              0xe0, 0x7e, 0x21, 0xc9, 0x47, 0xd1, 0x9e, 0x33,
                              0x76, 0xf0, 0x9b, 0x3c, 0x1e, 0x16, 0x17, 0x42};

  X25519_KEY alice_shared_secret;
  X25519_KEY bob_shared_secret;

  X25519_Shared(bob_shared_secret, alice_public, bob_sk);
  X25519_Shared(alice_shared_secret, bob_public, alice_sk);
  EXPECT_EQ(memcmp(alice_shared_secret, shared_secret, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << alice_shared_secret << "want: " << shared_secret;
  EXPECT_EQ(memcmp(bob_shared_secret, shared_secret, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << bob_shared_secret << "want: " << shared_secret;
}

TEST(X25519, IETF_CFRG1_0) {
  X25519_KEY k;
  X25519_KEY k_1_times = {0x42, 0x2c, 0x8e, 0x7a, 0x62, 0x27, 0xd7, 0xbc,
                          0xa1, 0x35, 0x0b, 0x3e, 0x2b, 0xb7, 0x27, 0x9f,
                          0x78, 0x97, 0xb8, 0x7b, 0xb6, 0x85, 0x4b, 0x78,
                          0x3c, 0x60, 0xe8, 0x03, 0x11, 0xae, 0x30, 0x79};
  times(1, k);
  EXPECT_EQ(memcmp(k, k_1_times, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << k << "want: " << k_1_times;
}

TEST(X25519, IETF_CFRG1_1) {
  X25519_KEY k;
  X25519_KEY k_1000_times = {0x68, 0x4c, 0xf5, 0x9b, 0xa8, 0x33, 0x09, 0x55,
                             0x28, 0x00, 0xef, 0x56, 0x6f, 0x2f, 0x4d, 0x3c,
                             0x1c, 0x38, 0x87, 0xc4, 0x93, 0x60, 0xe3, 0x87,
                             0x5f, 0x2e, 0xb9, 0x4d, 0x99, 0x53, 0x2c, 0x51};
  times(1000, k);
  EXPECT_EQ(memcmp(k, k_1000_times, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << k << "want: " << k_1000_times;
}

TEST(X25519, DISABLED_IETF_CFRG1_2) {
  X25519_KEY k;
  X25519_KEY k_1000000_times = {0x7c, 0x39, 0x11, 0xe0, 0xab, 0x25, 0x86, 0xfd,
                                0x86, 0x44, 0x97, 0x29, 0x7e, 0x57, 0x5e, 0x6f,
                                0x3b, 0xc6, 0x01, 0xc0, 0x88, 0x3c, 0x30, 0xdf,
                                0x5f, 0x4d, 0xd2, 0xd2, 0x4f, 0x66, 0x54, 0x24};
  times(1000000, k);
  EXPECT_EQ(memcmp(k, k_1000000_times, X25519_KEYSIZE_BYTES), 0)
      << "got:  " << k << "want: " << k_1000000_times;
}

TEST(X25519, DIFFIE_HELLMAN) {
  int64_t i = 0, TIMES = 1000;
  int64_t cnt = 0;

  for (i = 0; i < TIMES; i++) {
    X25519_KEY alicesk, alicepk;
    random_X25519_key(alicesk);
    X25519_KeyGen(alicepk, alicesk);

    X25519_KEY bobsk, bobpk;
    random_X25519_key(bobsk);
    X25519_KeyGen(bobpk, bobsk);

    X25519_KEY alice_shared_secret;
    X25519_KEY bob_shared_secret;
    X25519_Shared(alice_shared_secret, bobpk, alicesk);
    X25519_Shared(bob_shared_secret, alicepk, bobsk);
    EXPECT_EQ(
        memcmp(alice_shared_secret, bob_shared_secret, X25519_KEYSIZE_BYTES), 0)
        << "got:  " << alice_shared_secret << "want: " << bob_shared_secret;
    cnt++;
  }
  EXPECT_EQ(cnt, TIMES) << "passed: " << cnt << "/" << TIMES << std::endl;
}
