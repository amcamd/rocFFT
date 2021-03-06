/*******************************************************************************
 * Copyright (C) 2016 Advanced Micro Devices, Inc. All rights reserved.
 ******************************************************************************/


#ifndef KERNEL_LAUNCH_SINGLE
#define KERNEL_LAUNCH_SINGLE

#define FN_PRFX(X) rocfft_internal_ ## X
#include <iostream>
#include "rocfft.h"
#include "rocfft_hip.h"
#include "plan.h"
#include "repo.h"
#include "transform.h"
#include "error.h"
#include "kernel_launch_generator.h"
#include "kargs.h"

struct DeviceCallIn
{
    TreeNode *node;
    void *bufIn[2];
    void *bufOut[2];

    GridParam gridParam;
};

struct DeviceCallOut
{
    int err;
};

extern "C"
{

    /* Naming convention 

    dfn – device function caller (just a prefix, though actually GPU kernel function)

    sp (dp) – single (double) precision

    ip – in-place

    op - out-of-place

    ci – complex-interleaved (format of input buffer)

    ci – complex-interleaved (format of output buffer)

    stoc – stockham fft kernel
    bcc - block column column 

    1(2) – one (two) dimension data from kernel viewpoint, but 2D may transform into 1D. e.g  64*128(2D) = 8192(1D)

    1024, 64_128 – length of fft on each dimension 

    */

    //single precsion (sp)



    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_2_64_128)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_2_64_256)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_2_128_256)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_2_256_256)(void *data_p, void *back_p);

    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_2_64_2048)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_2_64_4096)(void *data_p, void *back_p);

    void FN_PRFX(dfn_sp_op_ci_ci_sbrc_2_128_64)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbrc_2_256_64)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbrc_2_256_128)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbrc_2_256_256)(void *data_p, void *back_p);

    void FN_PRFX(dfn_sp_ip_ci_ci_stoc_2_4096)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_ip_ci_ci_stoc_2_2048)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_ip_ci_ci_stoc_2_1024)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_ip_ci_ci_stoc_2_512)(void *data_p, void *back_p);

    void FN_PRFX(dfn_sp_op_ci_ci_stoc_2_4096)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_stoc_2_2048)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_stoc_2_1024)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_stoc_2_512)(void *data_p, void *back_p);

    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_3_64_128)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_3_64_256)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_3_128_256)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbcc_3_256_256)(void *data_p, void *back_p);

    void FN_PRFX(dfn_sp_op_ci_ci_sbrc_3_128_64)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbrc_3_256_64)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbrc_3_256_128)(void *data_p, void *back_p);
    void FN_PRFX(dfn_sp_op_ci_ci_sbrc_3_256_256)(void *data_p, void *back_p);

    void FN_PRFX(transpose_var1_sp)(void *data_p, void *back_p);

    void FN_PRFX(transpose_var2)(void *data_p, void *back_p);
    
    //double precsion (dp)


    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_2_64_128)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_2_64_256)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_2_128_256)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_2_256_256)(void *data_p, void *back_p);

    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_2_64_2048)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_2_64_4096)(void *data_p, void *back_p);

    void FN_PRFX(dfn_dp_op_ci_ci_sbrc_2_128_64)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbrc_2_256_64)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbrc_2_256_128)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbrc_2_256_256)(void *data_p, void *back_p);

    void FN_PRFX(dfn_dp_ip_ci_ci_stoc_2_4096)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_ip_ci_ci_stoc_2_2048)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_ip_ci_ci_stoc_2_1024)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_ip_ci_ci_stoc_2_512)(void *data_p, void *back_p);

    void FN_PRFX(dfn_dp_op_ci_ci_stoc_2_4096)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_stoc_2_2048)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_stoc_2_1024)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_stoc_2_512)(void *data_p, void *back_p);

    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_3_64_128)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_3_64_256)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_3_128_256)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbcc_3_256_256)(void *data_p, void *back_p);

    void FN_PRFX(dfn_dp_op_ci_ci_sbrc_3_128_64)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbrc_3_256_64)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbrc_3_256_128)(void *data_p, void *back_p);
    void FN_PRFX(dfn_dp_op_ci_ci_sbrc_3_256_256)(void *data_p, void *back_p);

    void FN_PRFX(transpose_var1_dp)(void *data_p, void *back_p);


}

/*
   data->node->devKernArg : points to the internal length device pointer
   data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH : points to the intenal in stride device pointer 
   data->node->devKernArg + 2*KERN_ARGS_ARRAY_WIDTH : points to the internal out stride device pointer, only used in outof place kernels
*/

#define POWX_SINGLE_SMALL_GENERATOR(FUNCTION_NAME, IP_FWD_KERN_NAME, IP_BACK_KERN_NAME, OP_FWD_KERN_NAME, OP_BACK_KERN_NAME) \
void FUNCTION_NAME(void *data_p, void *back_p)\
{\
    DeviceCallIn *data = (DeviceCallIn *)data_p;\
    printf("number of blocks = %d, number of threads = %d\n", data->gridParam.b_x, data->gridParam.tpb_x);  \
    if (data->node->placement == rocfft_placement_inplace) { \
        if(data->node->inStride[0] == 1 && data->node->outStride[0] == 1){ \
            if(data->node->direction == -1 ) {\
                hipLaunchKernel(HIP_KERNEL_NAME( IP_FWD_KERN_NAME<float2, SB_UNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (float2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (float2 *)data->bufIn[0]); \
            }\
            else{ \
                hipLaunchKernel(HIP_KERNEL_NAME( IP_BACK_KERN_NAME<float2, SB_UNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (float2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (float2 *)data->bufIn[0]); \
            }\
        } \
        else{ \
            if(data->node->direction == -1 ) {\
                hipLaunchKernel(HIP_KERNEL_NAME( IP_FWD_KERN_NAME<float2, SB_NONUNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (float2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (float2 *)data->bufIn[0]); \
            }\
            else{ \
                hipLaunchKernel(HIP_KERNEL_NAME( IP_BACK_KERN_NAME<float2, SB_NONUNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (float2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (float2 *)data->bufIn[0]); \
            }\
        } \
    }\
    else{ \
        if(data->node->inStride[0] == 1 && data->node->outStride[0] == 1){ \
            if(data->node->direction == -1) {\
                hipLaunchKernel(HIP_KERNEL_NAME( OP_FWD_KERN_NAME<float2, SB_UNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (float2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, data->node->devKernArg + 2*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (float2 *)data->bufIn[0], (float2 *)data->bufOut[0]); \
            }\
            else{ \
                hipLaunchKernel(HIP_KERNEL_NAME( OP_BACK_KERN_NAME<float2, SB_UNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (float2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, data->node->devKernArg + 2*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (float2 *)data->bufIn[0], (float2 *)data->bufOut[0]); \
            }\
        }\
        else{ \
            if(data->node->direction == -1) {\
                hipLaunchKernel(HIP_KERNEL_NAME( OP_FWD_KERN_NAME<float2, SB_NONUNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (float2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, data->node->devKernArg + 2*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (float2 *)data->bufIn[0], (float2 *)data->bufOut[0]); \
            }\
            else{ \
                hipLaunchKernel(HIP_KERNEL_NAME( OP_BACK_KERN_NAME<float2, SB_NONUNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (float2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, data->node->devKernArg + 2*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (float2 *)data->bufIn[0], (float2 *)data->bufOut[0]); \
            }\
        } \
    }\
}


#define POWX_DOUBLE_SMALL_GENERATOR(FUNCTION_NAME, IP_FWD_KERN_NAME, IP_BACK_KERN_NAME, OP_FWD_KERN_NAME, OP_BACK_KERN_NAME) \
void FUNCTION_NAME(void *data_p, void *back_p)\
{\
    DeviceCallIn *data = (DeviceCallIn *)data_p;\
    if (data->node->placement == rocfft_placement_inplace) { \
        if(data->node->inStride[0] == 1 && data->node->outStride[0] == 1){ \
            if(data->node->direction == -1 ) {\
                hipLaunchKernel(HIP_KERNEL_NAME( IP_FWD_KERN_NAME<double2, SB_UNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (double2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (double2 *)data->bufIn[0]); \
            }\
            else{ \
                hipLaunchKernel(HIP_KERNEL_NAME( IP_BACK_KERN_NAME<double2, SB_UNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (double2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (double2 *)data->bufIn[0]); \
            }\
        } \
        else{ \
            if(data->node->direction == -1 ) {\
                hipLaunchKernel(HIP_KERNEL_NAME( IP_FWD_KERN_NAME<double2, SB_NONUNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (double2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (double2 *)data->bufIn[0]); \
            }\
            else{ \
                hipLaunchKernel(HIP_KERNEL_NAME( IP_BACK_KERN_NAME<double2, SB_NONUNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (double2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (double2 *)data->bufIn[0]); \
            }\
        } \
    }\
    else{ \
        if(data->node->inStride[0] == 1 && data->node->outStride[0] == 1){ \
            if(data->node->direction == -1) {\
                hipLaunchKernel(HIP_KERNEL_NAME( OP_FWD_KERN_NAME<double2, SB_UNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (double2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, data->node->devKernArg + 2*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (double2 *)data->bufIn[0], (double2 *)data->bufOut[0]); \
            }\
            else{ \
                hipLaunchKernel(HIP_KERNEL_NAME( OP_BACK_KERN_NAME<double2, SB_UNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (double2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, data->node->devKernArg + 2*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (double2 *)data->bufIn[0], (double2 *)data->bufOut[0]); \
            }\
        }\
        else{ \
            if(data->node->direction == -1) {\
                hipLaunchKernel(HIP_KERNEL_NAME( OP_FWD_KERN_NAME<double2, SB_NONUNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (double2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, data->node->devKernArg + 2*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (double2 *)data->bufIn[0], (double2 *)data->bufOut[0]); \
            }\
            else{ \
                hipLaunchKernel(HIP_KERNEL_NAME( OP_BACK_KERN_NAME<double2, SB_NONUNIT> ), dim3(data->gridParam.b_x), dim3(data->gridParam.tpb_x), 0, 0, \
                (double2 *)data->node->twiddles, data->node->length.size(), \
                data->node->devKernArg, data->node->devKernArg + 1*KERN_ARGS_ARRAY_WIDTH, data->node->devKernArg + 2*KERN_ARGS_ARRAY_WIDTH, \
                data->node->batch, (double2 *)data->bufIn[0], (double2 *)data->bufOut[0]); \
            }\
        } \
    }\
}


#endif // KERNEL_LAUNCH_SINGLE

