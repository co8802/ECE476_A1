#include "prog2_intrin.h"

// computes the absolute value of all elements in the input array
// values, stores result in output
void absSerial(float* values, float* output, int N) {
  for (int i = 0; i < N; i++) {
    float x = values[i];
    if (x < 0) {
      output[i] = -x;
    } else {
      output[i] = x;
    }
  }
}

// implementation of absSerial() above, but it is vectorized using the fake
// intrinsics
void absVector(float* values, float* output, int N) {
  __prog2_vec_float x;
  __prog2_vec_float result;
  __prog2_vec_float zero = _prog2_vset_float(0.f);
  __prog2_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH) {
    // All ones
    maskAll = _prog2_init_ones();

    // All zeros
    maskIsNegative = _prog2_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _prog2_vload_float(x, values + i, maskAll);  // x = values[i];

    // Set mask according to predicate
    _prog2_vlt_float(maskIsNegative, x, zero, maskAll);  // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _prog2_vsub_float(result, zero, x, maskIsNegative);  //   output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _prog2_mask_not(maskIsNegative);  // } else {

    // Execute instruction ("else" clause)
    _prog2_vload_float(result, values + i,
                       maskIsNotNegative);  //   output[i] = x; }

    // Write results back to memory
    _prog2_vstore_float(output + i, result, maskAll);
  }
}

// accepts an array of values and an array of exponents
//
// For each element, compute values[i]^exponents[i] and clamp value to
// 9.999.  Store result in output.
void clampedExpSerial(float* values, int* exponents, float* output, int N) {
  for (int i = 0; i < N; i++) {
    float x = values[i];
    int y = exponents[i];
    if (y == 0) {
      output[i] = 1.f;
    } else {
      float result = x;
      int count = y - 1;
      while (count > 0) {
        result *= x;
        count--;
      }
      if (result > 9.999999f) {
        result = 9.999999f;
      }
      output[i] = result;
    }
  }
}

void clampedExpVector(float* values, int* exponents, float* output, int N) {
  //
  // ECE476 STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //
  //

  __prog2_vec_float x, result, maxVal;
  __prog2_vec_int y, count, zeroInt, oneInt;

  __prog2_mask maskAll, maskYisZero, maskNotZero, maskActive, maskTooBig;

  maxVal = _prog2_vset_float(9.999999f);
  zeroInt = _prog2_vset_int(0);
  oneInt = _prog2_vset_int(1);

  // jump by vector_width for SIMD
  for (int i = 0; i < N; i += VECTOR_WIDTH) {
    // out of bounds
    int width = VECTOR_WIDTH;
    if (i + VECTOR_WIDTH > N) {
      width = N - i;
    }
    // ex: if i = 8 and N = 10 -> maskAll = [1 1 0 0]
    maskAll = _prog2_init_ones(width);

  // load data
  _prog2_vload_float(x, values + i, maskAll);
  _prog2_vload_int(y, exponents + i, maskAll);

  // if (y == 0) output[i] = 1
  // anything to the exponent of 0 is 1
  _prog2_vset_float(result, 1.f, maskAll);

  // find lanes where exponent == 0
  _prog2_veq_int(maskYisZero, y, zeroInt, maskAll);

  // mask for exponent != 0;
  maskNotZero = _prog2_mask_not(maskYisZero);
  maskNotZero = _prog2_mask_and(maskNotZero, maskAll);

  // for y != 0 lanes, result = x
  _prog2_vmove_float(result, x, maskNotZero);

  // then subtract count by 1
  // count = y - 1
  _prog2_vsub_int(count, y, oneInt, maskNotZero);

  // while count > 0
  // for exponents that are 2 or greater
  // if exponent is 2, then the while loop runs once, and so on
  _prog2_vgt_int(maskActive, count, zeroInt, maskNotZero);
  while (_prog2_cntbits(maskActive) > 0) {
    // result *= x
    _prog2_vmult_float(result, result, x, maskActive);
    // count--
    _prog2_vsub_int(count, count, oneInt, maskActive);
    // recompute maskActive 
    _prog2_vgt_int(maskActive, count, zeroInt, maskNotZero);
  }

  // if (result > 9.999999f) {
  //    result = 9.999999f;
  //  }
  _prog2_vgt_float(maskTooBig, result, maxVal, maskAll);
  _prog2_vmove_float(result, maxVal, maskTooBig);

  _prog2_vstore_float(output + i, result, maskAll);


















  }



}

// returns the sum of all elements in values
float arraySumSerial(float* values, int N) {
  float sum = 0;
  for (int i = 0; i < N; i++) {
    sum += values[i];
  }

  return sum;
}

// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float* values, int N) {
  //
  // ECE476 STUDENTS TODO: Implement your vectorized version of arraySumSerial
  // here
  //
  // This is extra credit.

  return 0.0;
}