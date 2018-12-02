#include <iostream>

#include "coder.hpp"
#include "decoder.hpp"

int main(int argc, char *argv[])
{
  if(argc != 4)
  {
    std::cerr << "Некорректное количество параметров" << std::endl;;
  }
  else
  {
    std::string key = (std::string) argv[1];
    if(key == "-c")
    {
      try
      {
        Coder obj(argv[2], argv[3]);
        obj.readFile();
        obj.createdTree();
        obj.coding();
      }
      catch(std::exception& e)
      {
        std::cerr << "File not open" << std::endl;
        return -1;
      }
    }
    else if(key == "-d")
    {
      try
      {
        Decoder obj(argv[2], argv[3]);
        obj.readFile();
        obj.decoding();
        obj.recordInFile();
      }
      catch(std::exception& e)
      {
        std::cerr << "File not open" << std::endl;
        return -1;
      }
    }
    else
    {
      std::cerr << "Некорректный ключ" << std::endl;;
    }
  }
  return 0;
}