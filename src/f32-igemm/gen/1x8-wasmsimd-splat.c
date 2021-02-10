// Auto-generated file. Do not edit!
//   Template: src/f32-igemm/wasmsimd-splat.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <wasm_simd128.h>

#include <xnnpack/igemm.h>


void xnn_f32_igemm_ukernel_1x8__wasmsimd_splat(
    size_t mr,
    size_t nc,
    size_t kc,
    size_t ks,
    const float**restrict a,
    const float*restrict w,
    float*restrict c,
    size_t cm_stride,
    size_t cn_stride,
    size_t a_offset,
    const float* zero,
    const union xnn_f32_default_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(mr != 0);
  assert(mr <= 1);
  assert(nc != 0);
  assert(kc != 0);
  assert(kc % sizeof(float) == 0);
  assert(ks != 0);
  assert(ks % (1 * sizeof(void*)) == 0);
  assert(a_offset % sizeof(float) == 0);
  assert(a != NULL);
  assert(w != NULL);
  assert(c != NULL);

  float* c0 = c;

  do {
    v128_t vacc0x0123 = wasm_v128_load(w);
    v128_t vacc0x4567 = wasm_v128_load(w + 4);
    w += 8;

    size_t p = ks;
    do {
      const float* restrict a0 = a[0];
      assert(a0 != NULL);
      if XNN_UNPREDICTABLE(a0 != zero) {
        a0 = (const float*) ((uintptr_t) a0 + a_offset);
      }
      a += 1;

      size_t k = kc;
      while (k >= 4 * sizeof(float)) {
        const v128_t va0 = wasm_v128_load(a0);
        a0 += 4;

        const v128_t va0c0 = wasm_v32x4_shuffle(va0, va0, 0, 0, 0, 0);

        const v128_t vb0123c0 = wasm_v128_load(w + 0);
        const v128_t vb4567c0 = wasm_v128_load(w + 4);

        vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0c0, vb0123c0));
        vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0c0, vb4567c0));
        const v128_t va0c1 = wasm_v32x4_shuffle(va0, va0, 1, 1, 1, 1);

        const v128_t vb0123c1 = wasm_v128_load(w + 8);
        const v128_t vb4567c1 = wasm_v128_load(w + 12);

        vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0c1, vb0123c1));
        vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0c1, vb4567c1));
        const v128_t va0c2 = wasm_v32x4_shuffle(va0, va0, 2, 2, 2, 2);

        const v128_t vb0123c2 = wasm_v128_load(w + 16);
        const v128_t vb4567c2 = wasm_v128_load(w + 20);

        vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0c2, vb0123c2));
        vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0c2, vb4567c2));
        const v128_t va0c3 = wasm_v32x4_shuffle(va0, va0, 3, 3, 3, 3);

        const v128_t vb0123c3 = wasm_v128_load(w + 24);
        const v128_t vb4567c3 = wasm_v128_load(w + 28);

        vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0c3, vb0123c3));
        vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0c3, vb4567c3));

        w += 32;
        k -= 4 * sizeof(float);
      }
      if XNN_UNLIKELY(k != 0) {
        do {
          const v128_t vb0123 = wasm_v128_load(w);
          const v128_t vb4567 = wasm_v128_load(w + 4);
          w += 8;

          const v128_t va0 = wasm_v32x4_load_splat(a0);
          a0 += 1;

          vacc0x0123 = wasm_f32x4_add(vacc0x0123, wasm_f32x4_mul(va0, vb0123));
          vacc0x4567 = wasm_f32x4_add(vacc0x4567, wasm_f32x4_mul(va0, vb4567));
          k -= sizeof(float);
        } while (k != 0);
      }
      p -= 1 * sizeof(void*);
    } while (p != 0);


    if XNN_LIKELY(nc >= 8) {
      wasm_v128_store(c0, vacc0x0123);
      wasm_v128_store(c0 + 4, vacc0x4567);
      c0 = (float*) ((uintptr_t) c0 + cn_stride);

      a = (const float**restrict) ((uintptr_t) a - ks);
      nc -= 8;
    } else {
      if (nc & 4) {
        wasm_v128_store(c0, vacc0x0123);

        vacc0x0123 = vacc0x4567;

        c0 += 4;
      }
      if (nc & 2) {
        *((double*) c0) = wasm_f64x2_extract_lane(vacc0x0123, 0);

        vacc0x0123 = wasm_v32x4_shuffle(vacc0x0123, vacc0x0123, 2, 3, 2, 3);

        c0 += 2;
      }
      if (nc & 1) {
        *c0 = wasm_f32x4_extract_lane(vacc0x0123, 0);
      }

      nc = 0;
    }
  } while (nc != 0);
}