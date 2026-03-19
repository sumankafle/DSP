/**
 * @file test_framework.h
 * @brief Minimal test macros for DSP library validation.
 * @author suman
 * @date 2024/2/28
 *
 * Lightweight test harness -- no external dependencies.
 * Each test file has its own main() and links against libdsp.
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <math.h>

static int _tests_run    = 0;
static int _tests_passed = 0;
static int _tests_failed = 0;

#define TEST_BEGIN(name)  \
    printf("  %-44s ", name); _tests_run++

#define TEST_PASS()  \
    do { printf("[PASS]\n"); _tests_passed++; } while (0)

#define TEST_FAIL(msg)  \
    do { printf("[FAIL] %s\n", msg); _tests_failed++; } while (0)

#define ASSERT_TRUE(cond) do {                  \
    if (!(cond)) { TEST_FAIL(#cond); return; }  \
} while (0)

#define ASSERT_NEAR(a, b, tol) do {                                     \
    if (fabs((double)(a) - (double)(b)) > (tol)) {                      \
        char _buf[160];                                                  \
        snprintf(_buf, sizeof(_buf), "expected %.6g, got %.6g (tol=%g)", \
                 (double)(b), (double)(a), (double)(tol));               \
        TEST_FAIL(_buf); return;                                         \
    }                                                                    \
} while (0)

#define ASSERT_INT_EQ(a, b) do {                                \
    if ((a) != (b)) {                                            \
        char _buf[80];                                           \
        snprintf(_buf, sizeof(_buf), "expected %d, got %d",      \
                 (int)(b), (int)(a));                             \
        TEST_FAIL(_buf); return;                                 \
    }                                                            \
} while (0)

#define TEST_SUITE_BEGIN(name)  \
    printf("\n=== %s ===\n", name)

#define TEST_SUITE_END()                                                 \
    printf("\n--- Results: %d/%d passed", _tests_passed, _tests_run);    \
    if (_tests_failed > 0) printf(" (%d FAILED)", _tests_failed);        \
    printf(" ---\n\n");                                                  \
    return _tests_failed

#endif /* TEST_FRAMEWORK_H */
