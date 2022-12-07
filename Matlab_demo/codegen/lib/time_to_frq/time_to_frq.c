/*
 * File: time_to_frq.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 01-Dec-2022 15:11:59
 */

/* Include Files */
#include "time_to_frq.h"
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"
#include "time_to_frq_data.h"
#include "time_to_frq_initialize.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_hypotd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double y;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) {
    a /= y;
    y *= sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * sqrt(y * y + 1.0);
  } else if (!rtIsNaN(y)) {
    y = a * 1.4142135623730951;
  }
  return y;
}

/*
 * Arguments    : const double signal_time[4]
 *                double freq_mag[4]
 * Return Type  : void
 */
void time_to_frq(const double signal_time[4], double freq_mag[4])
{
  creal_T yCol[4];
  if (!isInitialized_time_to_frq) {
    time_to_frq_initialize();
  }
  c_FFTImplementationCallback_doH(signal_time, 0, yCol);
  freq_mag[0] = rt_hypotd_snf(yCol[0].re, yCol[0].im);
  freq_mag[1] = rt_hypotd_snf(yCol[1].re, yCol[1].im);
  freq_mag[2] = rt_hypotd_snf(yCol[2].re, yCol[2].im);
  freq_mag[3] = rt_hypotd_snf(yCol[3].re, yCol[3].im);
}

/*
 * File trailer for time_to_frq.c
 *
 * [EOF]
 */
