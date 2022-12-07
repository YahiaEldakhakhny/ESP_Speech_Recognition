/*
 * File: _coder_time_to_frq_mex.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 01-Dec-2022 15:11:59
 */

#ifndef _CODER_TIME_TO_FRQ_MEX_H
#define _CODER_TIME_TO_FRQ_MEX_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[],
                                     int32_T nrhs, const mxArray *prhs[]);

emlrtCTX mexFunctionCreateRootTLS(void);

void unsafe_time_to_frq_mexFunction(int32_T nlhs, mxArray *plhs[1],
                                    int32_T nrhs, const mxArray *prhs[1]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_time_to_frq_mex.h
 *
 * [EOF]
 */
