/*
 * File: _coder_time_to_frq_api.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 01-Dec-2022 15:11:59
 */

#ifndef _CODER_TIME_TO_FRQ_API_H
#define _CODER_TIME_TO_FRQ_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void time_to_frq(real_T signal_time[4], real_T freq_mag[4]);

void time_to_frq_api(const mxArray *prhs, const mxArray **plhs);

void time_to_frq_atexit(void);

void time_to_frq_initialize(void);

void time_to_frq_terminate(void);

void time_to_frq_xil_shutdown(void);

void time_to_frq_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_time_to_frq_api.h
 *
 * [EOF]
 */
