//===--- test_target_mapping_order.c---------------------------------------------------------===//
//
// OpenMP API Version 5.0 Nov 2018
//
// The description of the map clause was modified to clarify the mapping order when
// multiple map-types are specified for a variable or structure members of a variable
// on the same construct.
//
// For a given construct, the effect of a map clause with the to, from, or tofrom map-type
// is ordered before the effect of a map clause with the alloc, release, or delete map-type.
////===--------------------------------------------------------------------------------------===//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"

#define N 1024

int errors;

int to_before_delete () {

  int x, y, z, i;
  int scalar = 70;

  int a[N];
  int sum;

  struct {
    int var;
    int b[N];
  } member;

  for (i = 0; i < N; i++) { 
    a[i] = i;
    sum += i;
    member.b[i] = i;
  }

  member.var = 1;



#pragma omp target map (to: scalar, a, member) map (from: x, y, z) (delete: scalar, a, member)
  {
    x = scalar;
    z += member.var;

    for (i = 0; i < N; i++) {
      y += a[i];
      z += member.b[i];
    }
  }	 
  
  OMPVV_TEST_AND_SET_VERBOSE(errors, x != 70);
  OMPVV_TEST_AND_SET_VERBOSE(errors, y != sum);
  OMPVV_TEST_AND_SET_VERBOSE(errors, z != (sum + 1));

  return errors;

} 

int main () {
  
  int errors = 0;
  
  OMPVV_TEST_OFFLOADING;
  OMPVV_TEST_AND_SET_VERBOSE(errors, to_before_delete());
  OMPVV_REPORT_AND_RETURN(errors);

}
