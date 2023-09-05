subroutine mat_mul(a, b, c, a_m, a_n, b_m, b_n, c_m, c_n, res) bind(C,NAME="mat_mul")
  !DEC$ ATTRIBUTES DLLEXPORT::mat_mul
  !DEC$ ATTRIBUTES VALUE :: a_m, a_n, b_m, b_n, c_m, c_n

  use  iso_c_binding
  implicit none
  
  integer :: i, j
  
  integer (C_SIZE_T), intent(IN) :: a_m, a_n, b_m, b_n, c_m, c_n
  integer (C_INT),    dimension(a_m, a_n) :: a
  integer (C_INT),    dimension(b_m, b_n) :: b
  integer (C_INT),    dimension(c_m, c_n) :: c
  logical (C_BOOL),   intent(OUT) :: res
  
  if (a_n /= b_m) then
      res = .false.
      return
  endif
  
  if (c_m /= a_m .or. c_n /= b_n) then
      res = .false.
      return
  endif
  
  c = matmul(a, b)
  
  res = .true.
end subroutine mat_mul
