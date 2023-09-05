#pragma once

#include <random>
#include <thread>
#include <atomic>
#include <ostream>

#include "matrix.hpp"

class worker final
{
public:
   worker(std::ostream& out, std::size_t max_matrix_size = 100, std::size_t max_item = 100)
      : stopped_{ true }
      , out_{ out }
      , max_matrix_size_{ max_matrix_size }
      , max_item_{ max_item }
   {}

   ~worker()
   {
      if (thread_.joinable())
      {
         thread_.join();
      }
   }

   void start()
   {
      thread_ = std::thread([this]()
         {
            stopped_ = false;

            while (!stopped_)
            {
               const std::size_t size = gen_size(max_matrix_size_);

               const matrix<int> a = gen_matrix(size, max_matrix_size_);
               const matrix<int> b = gen_matrix(size, max_matrix_size_);

               const matrix<int> c = a * b;

               out_ << a << "\nX\n" << b << "\n=\n" << c << "\n--------------------------------" << std::endl;
            }
         });
   }

   void stop()
   {
      stopped_ = true;
   }

   bool stopped() const { return stopped_; }

private:
   static std::size_t gen_size(std::size_t max_size)
   {
      std::random_device dev{};
      std::mt19937 rng{ dev() };
      std::uniform_int_distribution<std::mt19937::result_type> size_dist{ 1, static_cast<std::uint32_t>(max_size) };
      std::size_t size = size_dist(dev);
      return size;
   }

   static matrix<int> gen_matrix(std::size_t matrix_size, std::size_t max_item)
   {
      matrix<int> m{ matrix_size, matrix_size };

      std::random_device dev{};
      std::mt19937 rng{ dev() };
      std::uniform_int_distribution<std::mt19937::result_type> item_dist{ 1, static_cast<std::uint32_t>(max_item) };
      for (std::size_t i = 0; i < m.get_rows_count(); ++i)
      {
         for (std::size_t j = 0; j < m.get_columns_count(); ++j)
         {
            m.set_item(i, j, item_dist(dev));
         }
      }

      return m;
   }

   std::thread thread_;
   std::atomic<bool> stopped_;
   std::ostream& out_;
   std::size_t max_matrix_size_;
   std::size_t max_item_;
};