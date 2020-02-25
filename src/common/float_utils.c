/*****************************************************************************
 *                                                                           *
 *  This file is part of Verificarlo.                                        *
 *                                                                           *
 *  Copyright (c) 2015                                                       *
 *     Universite de Versailles St-Quentin-en-Yvelines                       *
 *     CMLA, Ecole Normale Superieure de Cachan                              *
 *  Copyright (c) 2018-2020                                                  *
 *     Universite de Versailles St-Quentin-en-Yvelines                       *
 *                                                                           *
 *  Verificarlo is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  Verificarlo is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with Verificarlo.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 *****************************************************************************/

#include <stdbool.h>

#include "float_const.h"
#include "float_struct.h"
#include "generic_builtin.h"

/* Return true if the binary32 x is representable on the precision
 * virtual_precision  */
bool _is_representable_binary32(const float x, const int virtual_precision) {
  binary32 b32 = {.f32 = x};
  /* We must check if the mantissa is 0 since the behavior of ctz is undefied */
  /* in this case */
  if (b32.ieee.mantissa == 0) {
    return true;
  } else {
    uint32_t trailing_0 = __builtin_ctz(b32.ieee.mantissa);
    return FLOAT_PMAN_SIZE < (virtual_precision + trailing_0);
  }
}

/* Return true if the binary64 x is representable on the precision
 * virtual_precision  */
bool _is_representable_binary64(const double x, const int virtual_precision) {
  binary64 b64 = {.f64 = x};
  /* We must check if the mantissa is 0 since the behavior of ctzl is undefied
   */
  /* in this case */
  if (b64.ieee.mantissa == 0) {
    return true;
  } else {
    uint64_t trailing_0 = __builtin_ctzl(b64.ieee.mantissa);
    return DOUBLE_PMAN_SIZE < (virtual_precision + trailing_0);
  }
}

/* Return true if the binary128 x is representable on the precision
 * virtual_precision  */
bool _is_representable_binary128(const __float128 x,
                                 const int virtual_precision) {
  binary128 b128 = {.f128 = x};
  /* We must check if the mantissa is 0 since the behavior of ctzl is undefied
   */
  /* in this case */
  if (b128.ieee128.mantissa == 0) {
    return true;
  } else {
    /* Count the number of trailing zeros in the lower part of the mantissa */
    const uint64_t trailing_0_lx =
        (b128.ieee.mant_low == 0) ? QUAD_LX_PMAN_SIZE : CTZ(b128.ieee.mant_low);
    /* Count the number of trailing zeros in the higher part of the mantissa
       if the lower part is zero */
    const uint64_t trailing_0_hx =
        (b128.ieee.mant_low == 0) ? __builtin_ctzl(b128.ieee.mant_high) : 0;
    /* Sum the number of trailing zeros in the higher and in the lower part */
    const uint64_t trailing_0 = trailing_0_lx + trailing_0_hx;
    return QUAD_PMAN_SIZE < (virtual_precision + trailing_0);
  }
}
/* Returns the unbiased exponent of the binary32 f */
int32_t _get_exponent_binary32(const float f) {
  binary32 x = {.f32 = f};
  /* Substracts the bias */
  return x.ieee.exponent - FLOAT_EXP_COMP;
}

/* Returns the unbiased exponent of the binary64 d */
int32_t _get_exponent_binary64(const double d) {
  binary64 x = {.f64 = d};
  /* Substracts the bias */
  return x.ieee.exponent - DOUBLE_EXP_COMP;
}

/* Returns the unbiased exponent of the binary128 q */
int32_t _get_exponent_binary128(const __float128 q) {
  binary128 x = {.f128 = q};
  /* Substracts the bias */
  return x.ieee.exponent - QUAD_EXP_COMP;
}

/* Returns 2^exp for binary32 */
/* Fast function that implies no overflow neither underflow */
float _fast_pow2_binary32(const int exp) {
  binary32 b32 = {.f32 = 0.0f};
  b32.ieee.exponent = exp + FLOAT_EXP_COMP;
  return b32.f32;
}

/* Returns 2^exp for binary64 */
/* Fast function that implies no overflow neither underflow */
double _fast_pow2_binary64(const int exp) {
  binary64 b64 = {.f64 = 0.0};
  b64.ieee.exponent = exp + DOUBLE_EXP_COMP;
  return b64.f64;
}

/* Returns 2^exp for binary128 */
/* Fast function that implies no overflow neither underflow */
__float128 _fast_pow2_binary128(const int exp) {
  binary128 b128 = {.f128 = 0.0Q};
  b128.ieee128.exponent = exp + QUAD_EXP_COMP;
  return b128.f128;
}
