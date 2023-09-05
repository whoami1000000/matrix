#pragma once

#include <Windows.h>

#include <memory>

class dll final
{
public:
   static dll& create()
   {
      static dll instance{};
      return instance;
   }

   bool is_valid() const noexcept { return dll_ && mat_mul_func_; }

   bool mat_mul(const int* a, const int* b, int* c, std::size_t a_m, std::size_t a_n, std::size_t b_m, std::size_t b_n, std::size_t c_m, std::size_t c_n)
   {
      bool result{ false };
      if (mat_mul_func_)
      {
         mat_mul_func_(a, b, c, a_m, a_n, b_m, b_n, c_m, c_n, &result);
      }
      return result;
   }

private:
   dll()
      : dll_{ LoadLibraryA(DLL_NAME), FreeLibrary }
      , mat_mul_func_{ (mat_mul_func_t)GetProcAddress(dll_.get(), "mat_mul") }
   {}

   static inline const char* DLL_NAME = "matrix_lib.dll";

   using dll_t = std::remove_pointer_t<HMODULE>;
   std::unique_ptr<dll_t, BOOL(*)(dll_t*)> dll_;

   using mat_mul_func_t = std::add_pointer<void(const int*, const int*, int*, std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, std::size_t, bool*)>::type;
   mat_mul_func_t mat_mul_func_;
};