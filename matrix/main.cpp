#include <iostream>
#include <fstream>

#include "worker.hpp"

int main()
{
   std::fstream f{ "result.txt", std::fstream::out | std::fstream::trunc };
   worker w{ f };
   w.start();

   std::cout << "Started writing result.txt...\nPress any button to stop process" << std::endl;
   std::system("pause");

   w.stop();

   return 0;
}