#include <vector>
#include <fstream>
#include <iostream>

#include "decoder.hpp"

Decoder::Decoder(std::string file_in, std::string file_out):
  file_in_(file_in),
  file_out_(file_out),
  length_(0)
{
}

void Decoder::readFile()
{
  std::ifstream input(file_in_);
  if (!input)
  {
    throw std::exception();
  }

  input.read((char *)&length_, sizeof(length_));//считываем длину исходного текста
  int treeSize;
  input.read((char *)&treeSize, sizeof(treeSize));//считываем размер дерева
  /*
  *считываем само дерево из файла и засовываем его в дерево в декодере
  */
  for (int i = 0; i < treeSize; ++i)
  {
    Node n;
    input.read((char *)&n, sizeof(n));
    tree.push_back(n);
  }
  unsigned char symbol = 0;
  while (!input.eof())
  {
    input.read((char *)&symbol, sizeof(symbol));//считываем байт
    for (int i = 0; i < 8; ++i)
    {
      data.push_back((symbol & (1 << i)) != 0);//раскручиваем байт по битам и записываем его в массив
    }
    symbol = 0;
  }
  input.close();
}

void Decoder::decoding()
{
  auto indexTree = tree.size() - 1;
  /*
  *  спускаем ся по дереву до исходного символа и записываем егов массив data
  */
  for (auto i: data)
  {
    if (i)
    {
      indexTree = tree[indexTree].one;
    }
    else
    {
      indexTree = tree[indexTree].zero;
    }
    if (tree[indexTree].one == -1)//напоминаю у исходного символа нет детей, поэтому такая проверка уместна (можно сравнить с zero)
    {
      /*
      * вы спросите, почему я не записываю расшифрованный текст сразу в файл.
      * я это делаю, так как когда мы формируем байты, последний байт может 
      * оказаться не полным (в нем будут лишние нули). Из-за этого расшифрованный текст
      * может быть с лишними элементами в конце. Для этого я и записываю текст в массив text.
      * и использую length_(длину исходного текста), чтобы получить корректный расшифрованный текст.
      */
      text.push_back(tree[indexTree].symbol);//кладем расшифрованный текст в массив text
      //fo.write((char *)&tree[indexTree].symbol, sizeof(tree[indexTree].symbol));
      //std::cout << tree[indexTree].symbol;
      indexTree = tree.size() - 1;
    }
  }
}

void Decoder::recordInFile()
{
  std:: ofstream output(file_out_);
  for (size_t i = 0; i < length_; i++)//о чем я и писал выше, благодаря этому мы получаем точно такой же файл, что и зашифровывали
  {
    output.write((char *)&text[i], sizeof(text[i]));
  }
  output.close();
}