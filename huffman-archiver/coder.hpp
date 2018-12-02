#ifndef CODER_HPP
#define CODER_HPP

#include <map>
#include <vector>

class Coder
{
public:
  struct Node
  {
    char symbol;   //символ
    int parent;    //индекс родителя
    int zero;      //ребенок по ветке ноль (равно -1 если нет ребенка)
    int one;       //ребенок по ветке единица (равно -1 если нет ребенка)
    bool branch;   //ветка, к которой относится сам узел(символ)
  };

  Coder(std::string file_in, std::string file_out);
  virtual ~Coder() = default;

  void readFile();
  void createdTree();
  void coding();

  std::string file_in_, file_out_;
  int weight[256];                                                  //массив частоты втречаемости символов
  std::multimap<int /*вес*/, int /* индекс в tree */> sortedWeight; //массив частоты встречаемости символов отсортированный по возврастанию
  std::vector<Node> tree;                                           //дерево(псевдо, так как ориентируемся по индексам, а не строим реально дерево)
  std::map<char, int> charMap;                                     //массив для хранения индексов символов в дереве
};


#endif