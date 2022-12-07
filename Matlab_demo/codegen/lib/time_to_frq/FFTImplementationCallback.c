/*
 * File: FFTImplementationCallback.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 01-Dec-2022 15:11:59
 */

/* Include Files */
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const double x[4]
 *                int xoffInit
 *                creal_T y[4]
 * Return Type  : void
 */
void c_FFTImplementationCallback_doH(const double x[4], int xoffInit,
                                     creal_T y[4])
{
  double d;
  double temp_im;
  double temp_re;
  double temp_re_tmp_tmp;
  double y_im;
  double y_re;
  y[0].re = x[xoffInit];
  y[0].im = x[xoffInit + 1];
  y[1].re = x[xoffInit + 2];
  y[1].im = x[xoffInit + 3];
  temp_re_tmp_tmp = y[1].re;
  temp_im = y[1].im;
  y[1].re = y[0].re - y[1].re;
  y[1].im = y[0].im - y[1].im;
  y[0].re += temp_re_tmp_tmp;
  y[0].im += temp_im;
  temp_re = y[0].re;
  temp_im = y[0].im;
  y_im = -y[0].re + y[0].im;
  y_re = y[0].re;
  temp_re_tmp_tmp = -y[0].im;
  y[0].re = 0.5 * ((y[0].re - (-y[0].im)) + (y_re - temp_re_tmp_tmp));
  y[0].im = 0.5 * (y_im + (y_re + temp_re_tmp_tmp));
  d = temp_re - temp_im;
  y[2].re = 0.5 * (d + d);
  y[2].im = 0.5 * ((temp_re + temp_im) + (-temp_re + -temp_im));
  temp_re = y[1].re;
  d = y[1].im;
  temp_re_tmp_tmp = y[1].re * 0.0;
  temp_im = y[1].im * 0.0;
  y_re = y[1].re;
  y_im = -y[1].im;
  y[1].re = 0.5 * ((temp_re_tmp_tmp - temp_im) + (y_re * 2.0 - y_im * 0.0));
  y[1].im = 0.5 * ((temp_re_tmp_tmp + temp_im) + (y_re * 0.0 + y_im * 2.0));
  temp_re_tmp_tmp = -d * 0.0;
  y[3].re =
      0.5 * ((temp_re * 2.0 - d * 0.0) + (temp_re * 0.0 - temp_re_tmp_tmp));
  y[3].im =
      0.5 * ((temp_re * 0.0 + d * 2.0) + (temp_re * 0.0 + temp_re_tmp_tmp));
}

/*
 * File trailer for FFTImplementationCallback.c
 *
 * [EOF]
 */
