/*******************************************************************************
 * Copyright (C) 2016 Advanced Micro Devices, Inc. All rights reserved.
 ******************************************************************************/

#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include <hip/hip_runtime.h>
#include "common.h"
#include "butterfly_constant.h"


template <typename T>
__device__ T
TWLstep2(T *twiddles, size_t u)
{
	size_t j = u & 255;
	T result = twiddles[j];
	u >>= 8;
	j = u & 255;
	result = lib_make_vector2<T>((result.x * twiddles[256 + j].x - result.y * twiddles[256 + j].y),
		(result.y * twiddles[256 + j].x + result.x * twiddles[256 + j].y));
	return result;
}

template <typename T>
__device__ T
TWLstep3(T *twiddles, size_t u)
{
	size_t j = u & 255;
	T result = twiddles[j];
	u >>= 8;
	j = u & 255;
	result = lib_make_vector2<T>((result.x * twiddles[256 + j].x - result.y * twiddles[256 + j].y),
		(result.y * twiddles[256 + j].x + result.x * twiddles[256 + j].y));
	u >>= 8;
	j = u & 255;
	result = lib_make_vector2<T>((result.x * twiddles[512 + j].x - result.y * twiddles[512 + j].y),
		(result.y * twiddles[512 + j].x + result.x * twiddles[512 + j].y));
	return result;
}

template <typename T>
__device__ T
TWLstep4(T *twiddles, size_t u)
{
	size_t j = u & 255;
	T result = twiddles[j];
	u >>= 8;
	j = u & 255;
	result = lib_make_vector2<T>((result.x * twiddles[256 + j].x - result.y * twiddles[256 + j].y),
		(result.y * twiddles[256 + j].x + result.x * twiddles[256 + j].y));
	u >>= 8;
	j = u & 255;
	result = lib_make_vector2<T>((result.x * twiddles[512 + j].x - result.y * twiddles[512 + j].y),
		(result.y * twiddles[512 + j].x + result.x * twiddles[512 + j].y));
	u >>= 8;
	j = u & 255;
	result = lib_make_vector2<T>((result.x * twiddles[768 + j].x - result.y * twiddles[768 + j].y),
		(result.y * twiddles[768 + j].x + result.x * twiddles[768 + j].y));
	return result;
}

#define TWIDDLE_STEP_MUL_FWD(TWFUNC, TWIDDLES, INDEX, REG) \
	{ \
		T W = TWFUNC(TWIDDLES, INDEX); \
		float TR, TI; \
		TR = (W.x * REG.x) - (W.y * REG.y); \
		TI = (W.y * REG.x) + (W.x * REG.y); \
		REG.x = TR; \
		REG.y = TI; \
	}

#define TWIDDLE_STEP_MUL_INV(TWFUNC, TWIDDLES, INDEX, REG) \
	{ \
		T W = TWFUNC(TWIDDLES, INDEX); \
		float TR, TI; \
		TR =  (W.x * REG.x) + (W.y * REG.y); \
		TI = -(W.y * REG.x) + (W.x * REG.y); \
		REG.x = TR; \
		REG.y = TI; \
	}

#define TWIDDLE_MUL_FWD(TWIDDLES, INDEX, REG) \
{ \
	T W = TWIDDLES[INDEX]; \
	float TR, TI; \
	TR = (W.x * REG.x) - (W.y * REG.y); \
	TI = (W.y * REG.x) + (W.x * REG.y); \
	REG.x = TR; \
	REG.y = TI; \
}

#define TWIDDLE_MUL_INV(TWIDDLES, INDEX, REG) \
{ \
	T W = TWIDDLES[INDEX]; \
	float TR, TI; \
	TR =  (W.x * REG.x) + (W.y * REG.y); \
	TI = -(W.y * REG.x) + (W.x * REG.y); \
	REG.x = TR; \
	REG.y = TI; \
}

template<typename T>
__device__ void
Rad2(T *R0, T *R1)
{

	(*R1) = (*R0) - (*R1);
	(*R0) = 2.0 * (*R0) - (*R1);

}

template<typename T>
__device__ void
FwdRad4(T *R0, T *R2, T *R1, T *R3)
{

	T res;

	(*R1) = (*R0) - (*R1);
	(*R0) = 2.0 * (*R0) - (*R1);
	(*R3) = (*R2) - (*R3);
	(*R2) = 2.0 * (*R2) - (*R3);

	(*R2) = (*R0) - (*R2);
	(*R0) = 2.0 * (*R0) - (*R2);
	(*R3) = (*R1) + lib_make_vector2<T>(-(*R3).y, (*R3).x);
	(*R1) = 2.0 * (*R1) - (*R3);

	res = (*R1); (*R1) = (*R2); (*R2) = res;

}

template<typename T>
__device__ void
InvRad4(T *R0, T *R2, T *R1, T *R3)
{

	T res;

	(*R1) = (*R0) - (*R1);
	(*R0) = 2.0 * (*R0) - (*R1);
	(*R3) = (*R2) - (*R3);
	(*R2) = 2.0 * (*R2) - (*R3);

	(*R2) = (*R0) - (*R2);
	(*R0) = 2.0 * (*R0) - (*R2);
	(*R3) = (*R1) + lib_make_vector2<T>((*R3).y, -(*R3).x);
	(*R1) = 2.0 * (*R1) - (*R3);

	res = (*R1); (*R1) = (*R2); (*R2) = res;

}



template<typename T>
__device__ void
FwdRad8(T *R0, T *R4, T *R2, T *R6, T *R1, T *R5, T *R3, T *R7)
{

	T res;

	(*R1) = (*R0) - (*R1);
	(*R0) = 2.0 * (*R0) - (*R1);
	(*R3) = (*R2) - (*R3);
	(*R2) = 2.0 * (*R2) - (*R3);
	(*R5) = (*R4) - (*R5);
	(*R4) = 2.0 * (*R4) - (*R5);
	(*R7) = (*R6) - (*R7);
	(*R6) = 2.0 * (*R6) - (*R7);

	(*R2) = (*R0) - (*R2);
	(*R0) = 2.0 * (*R0) - (*R2);
	(*R3) = (*R1) + lib_make_vector2<T>(-(*R3).y, (*R3).x);
	(*R1) = 2.0 * (*R1) - (*R3);
	(*R6) = (*R4) - (*R6);
	(*R4) = 2.0 * (*R4) - (*R6);
	(*R7) = (*R5) + lib_make_vector2<T>(-(*R7).y, (*R7).x);
	(*R5) = 2.0 * (*R5) - (*R7);

	(*R4) = (*R0) - (*R4);
	(*R0) = 2.0 * (*R0) - (*R4);
	(*R5) = ((*R1) - C8Q * (*R5)) - C8Q * lib_make_vector2<T>((*R5).y, -(*R5).x);
	(*R1) = 2.0 * (*R1) - (*R5);
	(*R6) = (*R2) + lib_make_vector2<T>(-(*R6).y, (*R6).x);
	(*R2) = 2.0 * (*R2) - (*R6);
	(*R7) = ((*R3) + C8Q * (*R7)) - C8Q * lib_make_vector2<T>((*R7).y, -(*R7).x);
	(*R3) = 2.0 * (*R3) - (*R7);

	res = (*R1); (*R1) = (*R4); (*R4) = res;
	res = (*R3); (*R3) = (*R6); (*R6) = res;

}

template<typename T>
__device__ void
InvRad8(T *R0, T *R4, T *R2, T *R6, T *R1, T *R5, T *R3, T *R7)
{

	T res;

	(*R1) = (*R0) - (*R1);
	(*R0) = 2.0 * (*R0) - (*R1);
	(*R3) = (*R2) - (*R3);
	(*R2) = 2.0 * (*R2) - (*R3);
	(*R5) = (*R4) - (*R5);
	(*R4) = 2.0 * (*R4) - (*R5);
	(*R7) = (*R6) - (*R7);
	(*R6) = 2.0 * (*R6) - (*R7);

	(*R2) = (*R0) - (*R2);
	(*R0) = 2.0 * (*R0) - (*R2);
	(*R3) = (*R1) + lib_make_vector2<T>((*R3).y, -(*R3).x);
	(*R1) = 2.0 * (*R1) - (*R3);
	(*R6) = (*R4) - (*R6);
	(*R4) = 2.0 * (*R4) - (*R6);
	(*R7) = (*R5) + lib_make_vector2<T>((*R7).y, -(*R7).x);
	(*R5) = 2.0 * (*R5) - (*R7);

	(*R4) = (*R0) - (*R4);
	(*R0) = 2.0 * (*R0) - (*R4);
	(*R5) = ((*R1) - C8Q * (*R5)) + C8Q * lib_make_vector2<T>((*R5).y, -(*R5).x);
	(*R1) = 2.0 * (*R1) - (*R5);
	(*R6) = (*R2) + lib_make_vector2<T>((*R6).y, -(*R6).x);
	(*R2) = 2.0 * (*R2) - (*R6);
	(*R7) = ((*R3) + C8Q * (*R7)) + C8Q * lib_make_vector2<T>((*R7).y, -(*R7).x);
	(*R3) = 2.0 * (*R3) - (*R7);

	res = (*R1); (*R1) = (*R4); (*R4) = res;
	res = (*R3); (*R3) = (*R6); (*R6) = res;

}




template<typename T>
__device__ void
FwdRad16(T *R0, T *R8, T *R4, T *R12, T *R2, T *R10, T *R6, T *R14, T *R1, T *R9, T *R5, T *R13, T *R3, T *R11, T *R7, T *R15)
{

	T res;

	(*R1) = (*R0) - (*R1);
	(*R0) = 2.0 * (*R0) - (*R1);
	(*R3) = (*R2) - (*R3);
	(*R2) = 2.0 * (*R2) - (*R3);
	(*R5) = (*R4) - (*R5);
	(*R4) = 2.0 * (*R4) - (*R5);
	(*R7) = (*R6) - (*R7);
	(*R6) = 2.0 * (*R6) - (*R7);
	(*R9) = (*R8) - (*R9);
	(*R8) = 2.0 * (*R8) - (*R9);
	(*R11) = (*R10) - (*R11);
	(*R10) = 2.0 * (*R10) - (*R11);
	(*R13) = (*R12) - (*R13);
	(*R12) = 2.0 * (*R12) - (*R13);
	(*R15) = (*R14) - (*R15);
	(*R14) = 2.0 * (*R14) - (*R15);

	(*R2) = (*R0) - (*R2);
	(*R0) = 2.0 * (*R0) - (*R2);
	(*R3) = (*R1) + lib_make_vector2<T>(-(*R3).y, (*R3).x);
	(*R1) = 2.0 * (*R1) - (*R3);
	(*R6) = (*R4) - (*R6);
	(*R4) = 2.0 * (*R4) - (*R6);
	(*R7) = (*R5) + lib_make_vector2<T>(-(*R7).y, (*R7).x);
	(*R5) = 2.0 * (*R5) - (*R7);
	(*R10) = (*R8) - (*R10);
	(*R8) = 2.0 * (*R8) - (*R10);
	(*R11) = (*R9) + lib_make_vector2<T>(-(*R11).y, (*R11).x);
	(*R9) = 2.0 * (*R9) - (*R11);
	(*R14) = (*R12) - (*R14);
	(*R12) = 2.0 * (*R12) - (*R14);
	(*R15) = (*R13) + lib_make_vector2<T>(-(*R15).y, (*R15).x);
	(*R13) = 2.0 * (*R13) - (*R15);

	(*R4) = (*R0) - (*R4);
	(*R0) = 2.0 * (*R0) - (*R4);
	(*R5) = ((*R1) - C8Q * (*R5)) - C8Q * lib_make_vector2<T>((*R5).y, -(*R5).x);
	(*R1) = 2.0 * (*R1) - (*R5);
	(*R6) = (*R2) + lib_make_vector2<T>(-(*R6).y, (*R6).x);
	(*R2) = 2.0 * (*R2) - (*R6);
	(*R7) = ((*R3) + C8Q * (*R7)) - C8Q * lib_make_vector2<T>((*R7).y, -(*R7).x);
	(*R3) = 2.0 * (*R3) - (*R7);
	(*R12) = (*R8) - (*R12);
	(*R8) = 2.0 * (*R8) - (*R12);
	(*R13) = ((*R9) - C8Q * (*R13)) - C8Q * lib_make_vector2<T>((*R13).y, -(*R13).x);
	(*R9) = 2.0 * (*R9) - (*R13);
	(*R14) = (*R10) + lib_make_vector2<T>(-(*R14).y, (*R14).x);
	(*R10) = 2.0 * (*R10) - (*R14);
	(*R15) = ((*R11) + C8Q * (*R15)) - C8Q * lib_make_vector2<T>((*R15).y, -(*R15).x);
	(*R11) = 2.0 * (*R11) - (*R15);

	(*R8) = (*R0) - (*R8);
	(*R0) = 2.0 * (*R0) - (*R8);
	(*R9) = ((*R1) - C16A * (*R9)) - C16B * lib_make_vector2<T>((*R9).y, -(*R9).x);
	res = (*R8);
	(*R1) = 2.0 * (*R1) - (*R9);


	(*R10) = ((*R2) - C8Q * (*R10)) - C8Q * lib_make_vector2<T>((*R10).y, -(*R10).x);
	(*R2) = 2.0 * (*R2) - (*R10);
	(*R11) = ((*R3) - C16B * (*R11)) - C16A * lib_make_vector2<T>((*R11).y, -(*R11).x);
	(*R3) = 2.0 * (*R3) - (*R11);


	(*R12) = (*R4) + lib_make_vector2<T>(-(*R12).y, (*R12).x);
	(*R4) = 2.0 * (*R4) - (*R12);
	(*R13) = ((*R5) + C16B * (*R13)) - C16A * lib_make_vector2<T>((*R13).y, -(*R13).x);
	(*R5) = 2.0 * (*R5) - (*R13);


	(*R14) = ((*R6) + C8Q * (*R14)) - C8Q * lib_make_vector2<T>((*R14).y, -(*R14).x);
	(*R6) = 2.0 * (*R6) - (*R14);
	(*R15) = ((*R7) + C16A * (*R15)) - C16B * lib_make_vector2<T>((*R15).y, -(*R15).x);
	(*R7) = 2.0 * (*R7) - (*R15);

	res = (*R1); (*R1) = (*R8); (*R8) = res;
	res = (*R2); (*R2) = (*R4); (*R4) = res;
	res = (*R3); (*R3) = (*R12); (*R12) = res;
	res = (*R5); (*R5) = (*R10); (*R10) = res;
	res = (*R7); (*R7) = (*R14); (*R14) = res;
	res = (*R11); (*R11) = (*R13); (*R13) = res;

}

template<typename T>
__device__ void
InvRad16(T *R0, T *R8, T *R4, T *R12, T *R2, T *R10, T *R6, T *R14, T *R1, T *R9, T *R5, T *R13, T *R3, T *R11, T *R7, T *R15)
{

	T res;

	(*R1) = (*R0) - (*R1);
	(*R0) = 2.0 * (*R0) - (*R1);
	(*R3) = (*R2) - (*R3);
	(*R2) = 2.0 * (*R2) - (*R3);
	(*R5) = (*R4) - (*R5);
	(*R4) = 2.0 * (*R4) - (*R5);
	(*R7) = (*R6) - (*R7);
	(*R6) = 2.0 * (*R6) - (*R7);
	(*R9) = (*R8) - (*R9);
	(*R8) = 2.0 * (*R8) - (*R9);
	(*R11) = (*R10) - (*R11);
	(*R10) = 2.0 * (*R10) - (*R11);
	(*R13) = (*R12) - (*R13);
	(*R12) = 2.0 * (*R12) - (*R13);
	(*R15) = (*R14) - (*R15);
	(*R14) = 2.0 * (*R14) - (*R15);

	(*R2) = (*R0) - (*R2);
	(*R0) = 2.0 * (*R0) - (*R2);
	(*R3) = (*R1) + lib_make_vector2<T>((*R3).y, -(*R3).x);
	(*R1) = 2.0 * (*R1) - (*R3);
	(*R6) = (*R4) - (*R6);
	(*R4) = 2.0 * (*R4) - (*R6);
	(*R7) = (*R5) + lib_make_vector2<T>((*R7).y, -(*R7).x);
	(*R5) = 2.0 * (*R5) - (*R7);
	(*R10) = (*R8) - (*R10);
	(*R8) = 2.0 * (*R8) - (*R10);
	(*R11) = (*R9) + lib_make_vector2<T>((*R11).y, -(*R11).x);
	(*R9) = 2.0 * (*R9) - (*R11);
	(*R14) = (*R12) - (*R14);
	(*R12) = 2.0 * (*R12) - (*R14);
	(*R15) = (*R13) + lib_make_vector2<T>((*R15).y, -(*R15).x);
	(*R13) = 2.0 * (*R13) - (*R15);

	(*R4) = (*R0) - (*R4);
	(*R0) = 2.0 * (*R0) - (*R4);
	(*R5) = ((*R1) - C8Q * (*R5)) + C8Q * lib_make_vector2<T>((*R5).y, -(*R5).x);
	(*R1) = 2.0 * (*R1) - (*R5);
	(*R6) = (*R2) + lib_make_vector2<T>((*R6).y, -(*R6).x);
	(*R2) = 2.0 * (*R2) - (*R6);
	(*R7) = ((*R3) + C8Q * (*R7)) + C8Q * lib_make_vector2<T>((*R7).y, -(*R7).x);
	(*R3) = 2.0 * (*R3) - (*R7);
	(*R12) = (*R8) - (*R12);
	(*R8) = 2.0 * (*R8) - (*R12);
	(*R13) = ((*R9) - C8Q * (*R13)) + C8Q * lib_make_vector2<T>((*R13).y, -(*R13).x);
	(*R9) = 2.0 * (*R9) - (*R13);
	(*R14) = (*R10) + lib_make_vector2<T>((*R14).y, -(*R14).x);
	(*R10) = 2.0 * (*R10) - (*R14);
	(*R15) = ((*R11) + C8Q * (*R15)) + C8Q * lib_make_vector2<T>((*R15).y, -(*R15).x);
	(*R11) = 2.0 * (*R11) - (*R15);

	(*R8) = (*R0) - (*R8);
	(*R0) = 2.0 * (*R0) - (*R8);
	(*R9) = ((*R1) - C16A * (*R9)) + C16B * lib_make_vector2<T>((*R9).y, -(*R9).x);
	(*R1) = 2.0 * (*R1) - (*R9);
	(*R10) = ((*R2) - C8Q * (*R10)) + C8Q * lib_make_vector2<T>((*R10).y, -(*R10).x);
	(*R2) = 2.0 * (*R2) - (*R10);
	(*R11) = ((*R3) - C16B * (*R11)) + C16A * lib_make_vector2<T>((*R11).y, -(*R11).x);
	(*R3) = 2.0 * (*R3) - (*R11);
	(*R12) = (*R4) + lib_make_vector2<T>((*R12).y, -(*R12).x);
	(*R4) = 2.0 * (*R4) - (*R12);
	(*R13) = ((*R5) + C16B * (*R13)) + C16A * lib_make_vector2<T>((*R13).y, -(*R13).x);
	(*R5) = 2.0 * (*R5) - (*R13);
	(*R14) = ((*R6) + C8Q * (*R14)) + C8Q * lib_make_vector2<T>((*R14).y, -(*R14).x);
	(*R6) = 2.0 * (*R6) - (*R14);
	(*R15) = ((*R7) + C16A * (*R15)) + C16B * lib_make_vector2<T>((*R15).y, -(*R15).x);
	(*R7) = 2.0 * (*R7) - (*R15);

	res = (*R1); (*R1) = (*R8); (*R8) = res;
	res = (*R2); (*R2) = (*R4); (*R4) = res;
	res = (*R3); (*R3) = (*R12); (*R12) = res;
	res = (*R5); (*R5) = (*R10); (*R10) = res;
	res = (*R7); (*R7) = (*R14); (*R14) = res;
	res = (*R11); (*R11) = (*R13); (*R13) = res;

}

#endif // BUTTERFLY_H

