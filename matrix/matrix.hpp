#pragma once

#include <vector>
#include <ostream>

#include "dll.hpp"

template <typename item_t>
class matrix final
{
public:
   explicit matrix(std::size_t rows, std::size_t columns)
      : rows_{ rows }
      , columns_{ columns }
   {
      if (rows == 0 || columns == 0)
      {
         throw std::invalid_argument{ "rows and columns should be positive value" };
      }
      matrix_.resize(rows_ * columns_);
   }

   explicit matrix(const std::vector<std::vector<item_t>>& data)
      : matrix{ data.size(), data.back().size() }
   {
      for (std::size_t i = 0; i < rows_; ++i)
      {
         for (std::size_t j = 0; j < columns_; ++j)
         {
            set_item(i, j, data[i][j]);
         }
      }
   }

   std::size_t get_rows_count() const noexcept { return rows_; }
   std::size_t get_columns_count() const noexcept { return columns_; }

   item_t get_item(std::size_t row, std::size_t column) const
   {
      const std::size_t index = calc_index(row, column);
      return matrix_[index];
   }

   matrix& set_item(std::size_t row, std::size_t column, const item_t& item)
   {
      const std::size_t index = calc_index(row, column);
      matrix_[index] = item;
      return *this;
   }

   const item_t* as_fortran() const
   {
      return matrix_.data();
   }

   item_t* as_fortran()
   {
      return matrix_.data();
   }

   friend matrix operator * (const matrix& a, const matrix& b)
   {
      if (a.get_columns_count() != b.get_rows_count())
      {
         throw std::invalid_argument{ "matrices are incompatible" };
      }

      matrix c{ a.get_rows_count(), b.get_columns_count() };

      if (!dll::create().mat_mul(
         a.as_fortran(),
         b.as_fortran(),
         c.as_fortran(),
         a.get_rows_count(),
         a.get_columns_count(),
         b.get_rows_count(),
         b.get_columns_count(),
         c.get_rows_count(),
         c.get_columns_count()))
      {
         throw std::runtime_error{ "smth went wrong" };
      }

      return c;
   }

   friend std::ostream& operator << (std::ostream& out, const matrix& self)
   {
      out << "[r=" << self.rows_ << "][c=" << self.columns_ << "]" << std::endl;
      for (std::size_t r = 0; r < self.rows_; ++r)
      {
         for (std::size_t c = 0; c < self.columns_; ++c)
         {
            out << self.get_item(r, c) << "\t";
         }
         out << std::endl;
      }
      return out;
   }

private:
   std::size_t calc_index(std::size_t row, std::size_t column) const
   {
      const std::size_t index = rows_ * column + row;
      if (index >= matrix_.size())
      {
         throw std::out_of_range{ "item is out of range" };
      }
      return index;
   }

   std::vector<item_t> matrix_;
   std::size_t rows_;
   std::size_t columns_;
};