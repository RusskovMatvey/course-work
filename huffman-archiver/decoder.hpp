#ifndef DECODER_HPP
#define DECODER_HPP

#include <vector>

class Decoder
{
public: 
  struct Node
  {
    char symbol;
    int parent;
    int zero;
    int one;
    bool branch;
  };

  Decoder(std::string file_in_, std::string file_out_);
  virtual ~Decoder() = default;
  void readFile();
  void decoding();
  void recordInFile();

  int length_;// длина исходного текста
  std::string file_in_, file_out_;
  std::vector<Node> tree; //дерево
  std::vector<char> text; //исходный текст
  std::vector<bool> data; //зашифрованный текст
};

#endif