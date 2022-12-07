/*
 * File: time_to_frq_initialize.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 01-Dec-2022 15:11:59
 */

/* Include Files */
#include "time_to_frq_initialize.h"
#include "rt_nonfinite.h"
#include "time_to_frq_data.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void time_to_frq_initialize(void)
{
  rt_InitInfAndNaN();
  isInitialized_time_to_frq = true;
}

/*
 * File trailer for time_to_frq_initialize.c
 *
 * [EOF]
 */
