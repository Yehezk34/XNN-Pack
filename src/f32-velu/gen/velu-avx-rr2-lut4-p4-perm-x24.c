// Auto-generated file. Do not edit!
//   Template: src/f32-velu/avx-rr2-lut4-p4-perm.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <immintrin.h>

#include <xnnpack/common.h>
#include <xnnpack/vunary.h>


static const int32_t mask_table[14] = {-1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0};

void xnn_f32_velu_ukernel__avx_rr2_lut4_p4_perm_x24(
    size_t n,
    const float* x,
    float* y,
    const union xnn_f32_elu_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(n % sizeof(float) == 0);

  const __m256 vprescale = _mm256_broadcast_ps((const __m128*) params->sse.prescale);
  const __m256 valpha = _mm256_broadcast_ps((const __m128*) params->sse.alpha);
  const __m256 vbeta = _mm256_broadcast_ps((const __m128*) params->sse.beta);

  const __m256 vsat_cutoff = _mm256_set1_ps(-0x1.154246p+4f);
  const __m256 vmagic_bias = _mm256_set1_ps(0x1.8003F8p21f);
  const __m256 vlog2e = _mm256_set1_ps(0x1.715476p+0f);
  const __m256 vindex_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x3));
  const __m256 vtable = _mm256_set_ps(
    0x1.AE89FAp+0f, 0x1.6A09E6p+0f, 0x1.306FE0p+0f, 0x1.000000p+0f,
    0x1.AE89FAp+0f, 0x1.6A09E6p+0f, 0x1.306FE0p+0f, 0x1.000000p+0f);
  const __m256 vminus_ln2_hi = _mm256_set1_ps(-0x1.62E400p-1f);
  const __m256 vminus_ln2_lo = _mm256_set1_ps(-0x1.7F7D1Cp-20f);
  const __m256 vc4 = _mm256_set1_ps(0x1.554F9Ap-5f);
  const __m256 vc3 = _mm256_set1_ps(0x1.557082p-3f);
  const __m256 vc2 = _mm256_set1_ps(0x1.000002p-1f);
  const __m256 vone = _mm256_set1_ps(1.0f);

  for (; n >= 24 * sizeof(float); n -= 24 * sizeof(float)) {
    __m256 vx0 = _mm256_loadu_ps(x);
    __m256 vx1 = _mm256_loadu_ps(x + 8);
    __m256 vx2 = _mm256_loadu_ps(x + 16);
    x += 24;

    const __m256 vz0 = _mm256_max_ps(vsat_cutoff, _mm256_mul_ps(vx0, vprescale));
    const __m256 vz1 = _mm256_max_ps(vsat_cutoff, _mm256_mul_ps(vx1, vprescale));
    const __m256 vz2 = _mm256_max_ps(vsat_cutoff, _mm256_mul_ps(vx2, vprescale));

    __m256 vn0 = _mm256_add_ps(_mm256_mul_ps(vz0, vlog2e), vmagic_bias);
    __m256 vn1 = _mm256_add_ps(_mm256_mul_ps(vz1, vlog2e), vmagic_bias);
    __m256 vn2 = _mm256_add_ps(_mm256_mul_ps(vz2, vlog2e), vmagic_bias);

    __m256 ven0 = _mm256_andnot_ps(vindex_mask, vn0);
    const __m256 vl0 = _mm256_permutevar_ps(vtable, _mm256_castps_si256(vn0));
    const __m128 ven0_lo = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_castps256_ps128(ven0)), 21));
    __m256 ven1 = _mm256_andnot_ps(vindex_mask, vn1);
    const __m256 vl1 = _mm256_permutevar_ps(vtable, _mm256_castps_si256(vn1));
    const __m128 ven1_lo = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_castps256_ps128(ven1)), 21));
    __m256 ven2 = _mm256_andnot_ps(vindex_mask, vn2);
    const __m256 vl2 = _mm256_permutevar_ps(vtable, _mm256_castps_si256(vn2));
    const __m128 ven2_lo = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_castps256_ps128(ven2)), 21));

    vn0 = _mm256_sub_ps(vn0, vmagic_bias);
    const __m128 ven0_hi = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_extractf128_ps(ven0, 1)), 21));
    vn1 = _mm256_sub_ps(vn1, vmagic_bias);
    const __m128 ven1_hi = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_extractf128_ps(ven1, 1)), 21));
    vn2 = _mm256_sub_ps(vn2, vmagic_bias);
    const __m128 ven2_hi = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_extractf128_ps(ven2, 1)), 21));

    __m256 vt0 = _mm256_add_ps(_mm256_mul_ps(vn0, vminus_ln2_hi), vz0);
    ven0 = _mm256_insertf128_ps(_mm256_castps128_ps256(ven0_lo), ven0_hi, 1);
    __m256 vt1 = _mm256_add_ps(_mm256_mul_ps(vn1, vminus_ln2_hi), vz1);
    ven1 = _mm256_insertf128_ps(_mm256_castps128_ps256(ven1_lo), ven1_hi, 1);
    __m256 vt2 = _mm256_add_ps(_mm256_mul_ps(vn2, vminus_ln2_hi), vz2);
    ven2 = _mm256_insertf128_ps(_mm256_castps128_ps256(ven2_lo), ven2_hi, 1);

    vt0 = _mm256_add_ps(_mm256_mul_ps(vn0, vminus_ln2_lo), vt0);
    __m256 vs0 = _mm256_mul_ps(vl0, ven0);
    vt1 = _mm256_add_ps(_mm256_mul_ps(vn1, vminus_ln2_lo), vt1);
    __m256 vs1 = _mm256_mul_ps(vl1, ven1);
    vt2 = _mm256_add_ps(_mm256_mul_ps(vn2, vminus_ln2_lo), vt2);
    __m256 vs2 = _mm256_mul_ps(vl2, ven2);

    __m256 vp0 = _mm256_add_ps(_mm256_mul_ps(vc4, vt0), vc3);
    __m256 vp1 = _mm256_add_ps(_mm256_mul_ps(vc4, vt1), vc3);
    __m256 vp2 = _mm256_add_ps(_mm256_mul_ps(vc4, vt2), vc3);

    vp0 = _mm256_add_ps(_mm256_mul_ps(vp0, vt0), vc2);
    vp1 = _mm256_add_ps(_mm256_mul_ps(vp1, vt1), vc2);
    vp2 = _mm256_add_ps(_mm256_mul_ps(vp2, vt2), vc2);

    vp0 = _mm256_mul_ps(vp0, vt0);
    vp1 = _mm256_mul_ps(vp1, vt1);
    vp2 = _mm256_mul_ps(vp2, vt2);

    vt0 = _mm256_mul_ps(vt0, vs0);
    vs0 = _mm256_sub_ps(vs0, vone);
    vt1 = _mm256_mul_ps(vt1, vs1);
    vs1 = _mm256_sub_ps(vs1, vone);
    vt2 = _mm256_mul_ps(vt2, vs2);
    vs2 = _mm256_sub_ps(vs2, vone);

    vp0 = _mm256_add_ps(_mm256_mul_ps(vp0, vt0), vt0);
    vp1 = _mm256_add_ps(_mm256_mul_ps(vp1, vt1), vt1);
    vp2 = _mm256_add_ps(_mm256_mul_ps(vp2, vt2), vt2);

    const __m256 ve0 = _mm256_mul_ps(_mm256_add_ps(vp0, vs0), valpha);
    vx0 = _mm256_mul_ps(vx0, vbeta);
    const __m256 ve1 = _mm256_mul_ps(_mm256_add_ps(vp1, vs1), valpha);
    vx1 = _mm256_mul_ps(vx1, vbeta);
    const __m256 ve2 = _mm256_mul_ps(_mm256_add_ps(vp2, vs2), valpha);
    vx2 = _mm256_mul_ps(vx2, vbeta);

    const __m256 vy0 = _mm256_blendv_ps(vx0, ve0, vx0);
    const __m256 vy1 = _mm256_blendv_ps(vx1, ve1, vx1);
    const __m256 vy2 = _mm256_blendv_ps(vx2, ve2, vx2);

    _mm256_storeu_ps(y, vy0);
    _mm256_storeu_ps(y + 8, vy1);
    _mm256_storeu_ps(y + 16, vy2);
    y += 24;
  }
  for (; n >= 8 * sizeof(float); n -= 8 * sizeof(float)) {
    __m256 vx = _mm256_loadu_ps(x);
    x += 8;

    const __m256 vz = _mm256_max_ps(vsat_cutoff, _mm256_mul_ps(vx, vprescale));

    __m256 vn = _mm256_add_ps(_mm256_mul_ps(vz, vlog2e), vmagic_bias);
    __m256 ven = _mm256_andnot_ps(vindex_mask, vn);
    const __m256 vl = _mm256_permutevar_ps(vtable, _mm256_castps_si256(vn));
    const __m128 ven_lo = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_castps256_ps128(ven)), 21));
    vn = _mm256_sub_ps(vn, vmagic_bias);
    const __m128 ven_hi = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_extractf128_ps(ven, 1)), 21));

    __m256 vt = _mm256_add_ps(_mm256_mul_ps(vn, vminus_ln2_hi), vz);
    ven = _mm256_insertf128_ps(_mm256_castps128_ps256(ven_lo), ven_hi, 1);
    vt = _mm256_add_ps(_mm256_mul_ps(vn, vminus_ln2_lo), vt);
    __m256 vs = _mm256_mul_ps(vl, ven);

    __m256 vp = _mm256_add_ps(_mm256_mul_ps(vc4, vt), vc3);
    vp = _mm256_add_ps(_mm256_mul_ps(vp, vt), vc2);
    vp = _mm256_mul_ps(vp, vt);

    vt = _mm256_mul_ps(vt, vs);
    vs = _mm256_sub_ps(vs, vone);
    vp = _mm256_add_ps(_mm256_mul_ps(vp, vt), vt);

    const __m256 ve = _mm256_mul_ps(_mm256_add_ps(vp, vs), valpha);
    vx = _mm256_mul_ps(vx, vbeta);
    const __m256 vy = _mm256_blendv_ps(vx, ve, vx);

    _mm256_storeu_ps(y, vy);
    y += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    assert(n >= 1 * sizeof(float));
    assert(n <= 7 * sizeof(float));
    __m256i vmask = _mm256_loadu_si256((const __m256i*) ((uintptr_t) &mask_table[7] - n));

    __m256 vx = _mm256_maskload_ps(x, vmask);

    const __m256 vz = _mm256_max_ps(vsat_cutoff, _mm256_mul_ps(vx, vprescale));

    __m256 vn = _mm256_add_ps(_mm256_mul_ps(vz, vlog2e), vmagic_bias);
    __m256 ven = _mm256_andnot_ps(vindex_mask, vn);
    const __m256 vl = _mm256_permutevar_ps(vtable, _mm256_castps_si256(vn));
    const __m128 ven_lo = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_castps256_ps128(ven)), 21));
    vn = _mm256_sub_ps(vn, vmagic_bias);
    const __m128 ven_hi = _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(_mm256_extractf128_ps(ven, 1)), 21));

    __m256 vt = _mm256_add_ps(_mm256_mul_ps(vn, vminus_ln2_hi), vz);
    ven = _mm256_insertf128_ps(_mm256_castps128_ps256(ven_lo), ven_hi, 1);
    vt = _mm256_add_ps(_mm256_mul_ps(vn, vminus_ln2_lo), vt);
    __m256 vs = _mm256_mul_ps(vl, ven);

    __m256 vp = _mm256_add_ps(_mm256_mul_ps(vc4, vt), vc3);
    vp = _mm256_add_ps(_mm256_mul_ps(vp, vt), vc2);
    vp = _mm256_mul_ps(vp, vt);

    vt = _mm256_mul_ps(vt, vs);
    vs = _mm256_sub_ps(vs, vone);
    vp = _mm256_add_ps(_mm256_mul_ps(vp, vt), vt);

    const __m256 ve = _mm256_mul_ps(_mm256_add_ps(vp, vs), valpha);
    vx = _mm256_mul_ps(vx, vbeta);
    const __m256 vy = _mm256_blendv_ps(vx, ve, vx);

    // _mm256_maskstore_ps(y, vmask, vf) could be used here, but triggers msan failures (probably an msan bug).
    __m128 vy_lo = _mm256_castps256_ps128(vy);
    if (n & (4 * sizeof(float))) {
      _mm_storeu_ps(y, vy_lo);
      vy_lo = _mm256_extractf128_ps(vy, 1);
      y += 4;
    }
    if (n & (2 * sizeof(float))) {
      _mm_storel_pi((__m64*) y, vy_lo);
      vy_lo = _mm_movehl_ps(vy_lo, vy_lo);
      y += 2;
    }
    if (n & (1 * sizeof(float))) {
      _mm_store_ss(y, vy_lo);
    }
  }
}
