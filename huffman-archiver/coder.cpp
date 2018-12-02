#include <vector>
#include <fstream>

#include "coder.hpp"

Coder::Coder(std::string file_in, std::string file_out):
  file_in_(file_in),
  file_out_(file_out)
{
  for (int &i: weight)
  {
    i = 0;
  }
}

void Coder::readFile()
{
  std::ifstream input(file_in_);
  if (!input)
  {
    throw std::exception();
  }
  /*
  * в цикле считываем символ из текста
  * и подсчитываем их частоту встречаемости
  */
  while (!input.eof())
  {
    unsigned char symbol;
    input.read((char *)&symbol, sizeof(symbol));
    ++weight[symbol];
  }
  input.close();
}

void Coder::createdTree()
{
  for (size_t i = 0; i < 256; ++i)
  {
    if (weight[i] > 0)
    {
      tree.push_back(Node{(char)i, -1, -1, -1, false}); //заполняем дерево символами
      charMap[i] = tree.size() - 1;                     //сохраняем индекс символа в дереве
      sortedWeight.insert(std::make_pair(weight[i], tree.size() - 1));// заполняем массив для сортировки
    }
  }
  /*
  *формируем узлы в дереве
  *в итоге в sortedWeight останется один эелемент, верхушка нашего дерева
  */
  while (sortedWeight.size() > 1)
  {
    int weight0 = begin(sortedWeight)->first;
    int index0 = begin(sortedWeight)->second;
    sortedWeight.erase(begin(sortedWeight));
    int weight1 = begin(sortedWeight)->first;
    int index1 = begin(sortedWeight)->second;
    sortedWeight.erase(begin(sortedWeight));
    tree.push_back(Node{'\0', -1, index0, index1, false}); // кладем созданный узел в дерево
    tree[index0].parent = tree.size() - 1;             // вносим информацию о родителе
    tree[index0].branch = false;                       // вносим информацию о ветке на которой находится данный узел
    tree[index1].parent = tree.size() - 1;
    tree[index1].branch = true;
    sortedWeight.insert(std::make_pair(weight0 + weight1, tree.size() - 1)); //кладем созданный узел в массив на место старых двух
  }
}

void Coder::coding()
{
  std::vector<bool> data;      //массив для закодированной последовательности
  std::ifstream input(file_in_);
  if (!input)
  {
    throw std::exception();
  }

  input.seekg(0, input.end);
  int length = input.tellg();  //длина исходного текста (нужно для точной декодировки)
  input.seekg(0, input.beg);

  while (!input.eof())//перебираем каждый символ
  {
    unsigned char symbol;
    input.read((char *)&symbol, sizeof(symbol));
    auto elementTree = tree[charMap[symbol]];
    std::vector<bool> invertedArray; //массив для закодированной последовательности (первоначально она будет перевернута, так как мы идем не с вершины дерева, а с конца)
    /*
    *  начинаем подниматься по дереву и записыватьв массив ветки по которым идем,
    *  формируя тем самым закодированную последовательность (правда еще пока перевернутую)
    *  (цикл работает пока не дойдем до вершины)
    */
    while (elementTree.parent != -1)
    {
      invertedArray.push_back(elementTree.branch);
      elementTree = tree[elementTree.parent];
    }
    data.insert(end(data), invertedArray.rbegin(), invertedArray.rend()); // переворачиваем закодированную последовательность
  }
  input.close();

  std::ofstream output(file_out_);
  output.write((char *)&length, sizeof(length));//записываем длину исходного текста
  int treeSize = tree.size();
  output.write((char *)&treeSize, sizeof(treeSize));//записываем длину дерева
  for (auto i: tree)
  {
    output.write((char *)&i, sizeof(i)); //записываем все дерево в файл
  }

  for (size_t i = 0; i <= data.size() / 8; ++i)
  {
    unsigned char symbol = 0;
    for (int j = 0; j < 8; ++j)
    {
      if (data[i * 8 + j])
      {
        symbol |= (1 << j); //формируем байт
      }
    }

    output.write((char *)&symbol, sizeof(symbol));// записываем полученный байт в файл
  }
  output.close();
}