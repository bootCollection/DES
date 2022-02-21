//Elliot Etches
//Oct 9, 2021

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <utility>
#include <bitset>
#include <vector>

//Data Handling
std::pair <std::string, std::string> input();
std::bitset<64> stringToBinary(std::string);
std::string binaryToString(std::bitset<64>);
std::bitset<64> hexToBinary(std::string);

//Encryption
std::bitset<64> initialPermE(std::bitset<64>);
std::bitset<64> initialPermD(std::bitset<64>);

std::bitset<32> splitBits(std::bitset<64>, bool);
std::bitset<64> catBits(std::bitset<32>, std::bitset<32>);

std::bitset<48> eTable(std::bitset<32>);

std::bitset<32> sBox(std::bitset<48>);
int selectBox(std::bitset<48>, int);
std::bitset<32> permu(std::bitset<32>);

std::bitset<64> initialPermENV(std::bitset<64>);

//Key Generation
std::bitset<56> permChoice1(std::bitset<64>);
std::bitset<28> kSplit(std::bitset<56>, bool);
std::bitset<56> kCat(std::bitset<28>, std::bitset<28>);
std::bitset<28> circSplit(std::bitset<28>, int);
std::bitset<48> permChoice2(std::bitset<56>);

//Total values
std::vector<std::bitset<48> > genKeys(std::bitset<64>);
std::bitset<64> enCryp(std::bitset<64>, std::bitset<64>);
std::bitset<64> deCryp(std::bitset<64>, std::bitset<64>);

int main(){
  std::bitset<64> plainT;
  std::bitset<64> key;
  std::vector<std::bitset<48> > keys;
  std::string textIn;
  bool sel;
  //data = input();
  unsigned long test;
  std::cout << "Input key: ";
  std::cin >> std::hex >> test;
  key = std::bitset<64>(test);

  std::cout << "Select mode of operation" << std::endl;
  std::cout << "[0] Encryption | [1] Decryption:";
  std::cin >> sel; 
  if(sel) {
    unsigned long test2;
    std::cout << "Enter cyphertext:";
    std::cin >> std::hex >> test2;
    plainT = std::bitset<64>(test2);
    std::cout << binaryToString(deCryp(plainT, key)) << std::endl;
                                } else {
    std::cout << "Enter plaintext:" << std::endl;
    std::cin.ignore();
    std::getline(std::cin, textIn);
    plainT = stringToBinary(textIn);
    std::cout << std::hex << (enCryp(plainT, key)).to_ullong() << std::endl;
    }
  return 0;
}

// Input
std::pair<std::string, std::string> input() {
  std::pair<std::string, std::string> in;
  std::cout << "Enter key: ";
  std::cin >> in.first;
  std::cout << "Enter Message: ";
  std::cin >> in.second;
  return in;
 }

// String To Binary
std::bitset<64> stringToBinary(std::string words) {
  std::bitset<64> tword; 
  for (int i =7; i >= 0; i--) {
    std::bitset<8> c = std::bitset<8>(words.c_str()[i]);
    for (int j = 7; j >= 0; j--) {
      if(c[j])
        tword.set((8*i)+j);
    }
  }
  return tword;
}

// Binary To String
std::string binaryToString(std::bitset<64> bitIn) {
  std::string strOut;
  for(int i = 0; i < 8; i++) {
    char c = 0;
    for(int j = 0; j < 8; j++) {
      if(bitIn[(8*i)+j])
        c |= 1 << j;
    }
    strOut.push_back(c);
  }
  return strOut;
}

std::bitset<64> hexToBinary(std::string strIn) {
  std::stringstream ss;
  unsigned n;
  ss >> n;
  std::bitset<64> bOut(n);
  return bOut;
}

std::bitset<64> initialPermE(std::bitset<64> in) {
  std::bitset<64> out;
  int table[64] =
    {58, 50, 42, 34, 26, 18, 10, 2,
     60, 52, 44, 36, 28, 20, 12, 4,
     62, 54, 46, 38, 30, 22, 14, 6,
     64, 56, 48, 40, 32, 24, 16, 8,
     57, 49, 41, 33, 25, 17, 9, 1,
     59, 51, 43, 35, 27, 19, 11, 3,
     61, 53, 45, 37, 29, 21, 13, 5,
     63, 55, 47, 39, 31, 23, 15, 7};
  for (int i = 0; i < 64; i++) {
    if(in[table[i] - 1])
      out.set(i, 1); 
  }
  return out;
}

std::bitset<64> initialPermD(std::bitset<64> in) {
  std::bitset<64> out;
  int table[64] =
    {58, 50, 42, 34, 26, 18, 10, 2,
     60, 52, 44, 36, 28, 20, 12, 4,
     62, 54, 46, 38, 30, 22, 14, 6,
     64, 56, 48, 40, 32, 24, 16, 8,
     57, 49, 41, 33, 25, 17, 9, 1,
     59, 51, 43, 35, 27, 19, 11, 3,
     61, 53, 45, 37, 29, 21, 13, 5,
     63, 55, 47, 39, 31, 23, 15, 7};
  for (int i =0; i < 64; i++) {
    if(in[i])
      out.set(table[i] - 1, 1);
  }
  return out;
}

std::bitset<32> splitBits(std::bitset<64> in, bool sel) {
  std::bitset<32> out;
  if(!sel)
    for (int i = 0; i < 32; i++)
      if(in[63 - i])
        out.set(31 - i, 1);
  if(sel)
    for (int i = 32; i < 64; i++)
      if(in[63 - i])
        out.set(31 - (i - 32), 1);
  return out;
}

std::bitset<64> catBits(std::bitset<32> l, std::bitset<32> r) {
  std::bitset<64> out;
  for(int i = 0; i < 32; i++)
    if(l[31 - i])
      out.set(63 - (i), 1);
  for(int i = 32; i < 64; i++)
    if(r[31 - (i - 32)])
      out.set((63 - i), 1);
  return out;
}

std::bitset<48> eTable(std::bitset<32> in) {
  std::bitset<48> out;
  int table[48] =
    { 32, 1, 2, 3, 4, 5,
      4, 5, 6, 7, 8, 9,
      8, 9, 10, 11, 12, 13,
      12, 13, 14, 15, 16, 17,
      16, 17, 18, 19, 20, 21,
      20, 21, 22, 23, 24, 25,
      24, 25, 26, 27, 28, 29,
      28, 29, 30, 31, 32, 1 };
    for (int i = 0; i < 48; i++) {
      if(in[table[i] - 1])
        out.set(i, 1);
    }
  return out;
}

//Key

std::bitset<56> permChoice1(std::bitset<64> in) {
  std::bitset<56> out;
  int table[56] =
    {57, 49, 41, 33, 25, 17, 9,
     1, 58, 50, 42, 34, 26, 18,
     10, 2, 59, 51, 43, 35, 27,
     19, 11, 3, 60, 52, 44, 36,
     63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
     14, 6, 61, 53, 45, 37, 29,
     21, 13, 5, 28, 20, 12, 4};
  for (int i = 0; i < 56; i++) { 
    if(in[63 - (table[i] - 1)])
      out.set(55 - i, 1); 
  }
  return out;

}

std::bitset<28> kSplit(std::bitset<56> in, bool sel) {
  std::bitset<28> out;
  if(!sel)
    for (int i = 0; i < 28; i++)
      if(in[55 - i])
        out.set(27 - i, 1);
  if(sel)
    for (int i = 28; i < 56; i++)
      if(in[55 - i])
        out.set(27 - (i - 28), 1);
  return out;
}

std::bitset<56> kCat(std::bitset<28> l, std::bitset<28> r) {
  std::bitset<56> out;
  for(int i = 0; i < 28; i++)
    if(l[27 - i])
      out.set(55 - (i), 1);
  for(int i = 28; i < 56; i++)
    if(r[27 - (i - 28)])
      out.set((55 - i), 1);
  return out;
}

std::bitset<28> circSplit(std::bitset<28> in, int round) {
  std::bitset<28> out;
  if (round == 1 || round == 2 || round == 9 || round == 16) {
    for(int i = 1; i < 28; i++) {
      if(in[i-1])
        out.set(i, 1);
    }
    if(in[27])
      out.set(0, 1);
  }
  else {
    for(int i = 2; i < 28; i++) {
      if(in[i-2])
        out.set(i, 1);
    }
    if(in[27])
      out.set(1, 1);
    if(in[26])
      out.set(0, 1);
  }

  return out;
}

std::bitset<48> permChoice2(std::bitset<56> in) {
  std::bitset<48> out;
  int table[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};
  for (int i = 0; i < 48; i++) { 
    if(in[55 - (table[i] - 1)])
      out.set(47 - i, 1); 
  }
  return out;
}

std::vector<std::bitset<48> > genKeys(std::bitset<64> in) {
  std::vector<std::bitset<48> > out;
  std::vector<std::bitset<28> > left, right;
  left.push_back(circSplit(kSplit(permChoice1(in), 0), 1));
  right.push_back(circSplit(kSplit(permChoice1(in), 1), 1));
  for(int i = 0; i < 16; i++){
    left.push_back(circSplit(left[i],i+2));
    right.push_back(circSplit(right[i], i+2));
  }
  for(int i = 0; i < 16; i++) {
    out.push_back(permChoice2(kCat(left[i], right[i])));
  }
  return out;
}

std::bitset<32> sBox(std::bitset<48> in) {
  std::bitset<32> out;
  std::bitset<4> temp;

  int s1[64] = {
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6 , 13
  };
  temp = std::bitset<4>(s1[selectBox(in, 0)]);
  for(int i = 0; i < 4; i++)
    if(temp[3 - i])
      out.set(31- i, 1);

  temp.reset(1);
  int s2[64] = {
    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
    3, 13, 4, 7, 15, 2, 7, 14, 12, 0, 1, 10, 6, 9, 11, 5,
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
  };
  temp = std::bitset<4>(s2[selectBox(in, 1)]);
  for(int i = 0; i < 4; i++)
    if(temp[3-i])
      out.set(27- i, 1);

  temp.reset(1);
  int s3[64] = {
    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
  };
  temp = std::bitset<4>(s3[selectBox(in, 2)]);
  for(int i = 0; i < 4; i++)
    if(temp[3 - i])
      out.set(23- i, 1);

  temp.reset(1);
  int s4[64] = {
    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14};
  temp = std::bitset<4>(s4[selectBox(in, 3)]);
  for(int i = 0; i < 4; i++)
    if(temp[3 - i])
      out.set(19- i, 1);

  temp.reset(1);
  int s5[64] = {
    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
    14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
};
  temp = std::bitset<4>(s5[selectBox(in, 4)]);
  for(int i = 0; i < 4; i++)
    if(temp[3 - i])
      out.set(15- i, 1);

  temp.reset(1);
  int s6[64] = {
    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
  };
  temp = std::bitset<4>(s6[selectBox(in, 5)]);
  for(int i = 0; i < 4; i++)
    if(temp[3 - i])
      out.set(11- i, 1);

  temp.reset(1);
  int s7[64] = {
    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
  };
  temp = std::bitset<4>(s7[selectBox(in, 6)]);
  for(int i = 0; i < 4; i++)
    if(temp[3 - i])
      out.set(7- i, 1);

  temp.reset(1);
  int s8[64] = {
    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
  };
  temp = std::bitset<4>(s8[selectBox(in, 7)]);
  for(int i = 0; i < 4; i++)
    if(temp[3 - i])
      out.set(3- i, 1);

  return out;
}

int selectBox(std::bitset<48> in, int index) {
  std::bitset<4> col;
  std::bitset<2> row;
  std::bitset<4> temp;
  int n;
  for(int i = 1; i < 5; i++)
   if(in[(47-index*6) - i])
     col.set(3-(i-1), 1);
  if(in[47-index*6])
    row.set(1, 1);
  if(in[(47-index*6)-5])
    row.set(0, 1);
  return 16*static_cast<int>(row.to_ulong()) + static_cast<int>(col.to_ulong());
}

std::bitset<32> permu(std::bitset<32> in) {
  std::bitset<32> out;
  int table[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
  };
  for (int i = 0; i < 32; i++) { 
    if(in[31 - (table[i] - 1)])
      out.set(31 - i, 1);
  }
  return out;
}

std::bitset<64> enCryp(std::bitset<64> plainT, std::bitset<64> key) {
  std::vector<std::bitset<48> > keys;
  std::bitset<32> left;
  std::bitset<32> right;
  keys = genKeys(key);
  left = splitBits(initialPermE(plainT), 0);
  right = splitBits(initialPermE(plainT), 1);
  for(int i = 0; i < 16; i++) {
    std::bitset<32> temp;
    temp = right;
    right = (left ^ permu(sBox((keys[i] ^ eTable(right)))));
    left = temp;
  }
  return initialPermENV(catBits(right, left));
}

std::bitset<64> initialPermENV(std::bitset<64> in) {
  std::bitset<64> out;
  int table[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
  };
  for (int i = 0; i < 64; i++) {
    if(in[table[i] - 1])
      out.set(i, 1);
  }
  return out;
}

std::bitset<64> deCryp(std::bitset<64>crypT, std::bitset<64> key){
 std::vector<std::bitset<48> > keys;
 std::bitset<32> left;
 std::bitset<32> right;
 keys = genKeys(key);
 left = splitBits(initialPermE(crypT), 0);
 right = splitBits(initialPermE(crypT), 1);
 for(int i = 0; i < 16; i++) {
   std::bitset<32> temp;
   temp = right;
   right = (left ^ permu(sBox((keys[15-i] ^ eTable(right)))));
   left = temp;
 }
 return initialPermENV(catBits(right, left));
}
