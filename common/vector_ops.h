/**
 * @file vector_ops.h
 * @brief Type-generic vector operation macros.
 * @author suman
 * @date 2024/2/28
 *
 * These macros work on arrays of any numeric C type. Each takes
 * explicit type parameters for mixed-type arithmetic with proper
 * promotion/demotion.
 *
 * Example usage:
 *   float a[100], b[100], c[100];
 *   VEC_ADD(a, b, c, 100, float, float, float);
 */

#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H

/** c[i] = a[i] + b[i] */
#define VEC_ADD(a, b, c, len, ta, tb, tc)  do {  \
    const ta *_pa = (a);                          \
    const tb *_pb = (b);                          \
    tc *_pc = (c);                                \
    for (int _i = 0; _i < (len); _i++)            \
        _pc[_i] = (tc)(_pa[_i] + _pb[_i]);       \
} while (0)

/** c[i] = a[i] - b[i] */
#define VEC_SUB(a, b, c, len, ta, tb, tc)  do {  \
    const ta *_pa = (a);                          \
    const tb *_pb = (b);                          \
    tc *_pc = (c);                                \
    for (int _i = 0; _i < (len); _i++)            \
        _pc[_i] = (tc)(_pa[_i] - _pb[_i]);       \
} while (0)

/** c[i] = a[i] * b[i] */
#define VEC_MUL(a, b, c, len, ta, tb, tc)  do {  \
    const ta *_pa = (a);                          \
    const tb *_pb = (b);                          \
    tc *_pc = (c);                                \
    for (int _i = 0; _i < (len); _i++)            \
        _pc[_i] = (tc)(_pa[_i] * _pb[_i]);       \
} while (0)

/** s = sum(a[i] * b[i]) -- dot product */
#define VEC_DOT(a, b, s, len, ta, tb)  do {      \
    const ta *_pa = (a);                          \
    const tb *_pb = (b);                          \
    (s) = _pa[0] * _pb[0];                       \
    for (int _i = 1; _i < (len); _i++)            \
        (s) += _pa[_i] * _pb[_i];                \
} while (0)

/** s = sum(a[i]) */
#define VEC_SUM(a, s, len, ta)  do {             \
    const ta *_pa = (a);                          \
    (s) = _pa[0];                                 \
    for (int _i = 1; _i < (len); _i++)            \
        (s) += _pa[_i];                           \
} while (0)

/** b[i] = s * a[i] -- scale and copy */
#define VEC_SCALE(a, b, s, len, ta, tb)  do {    \
    const ta *_pa = (a);                          \
    tb *_pb = (b);                                \
    for (int _i = 0; _i < (len); _i++)            \
        _pb[_i] = (tb)((s) * _pa[_i]);           \
} while (0)

#endif /* VECTOR_OPS_H */
