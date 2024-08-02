#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cfloat>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include <dlfcn.h>

using namespace std;

typedef int (*snprintf_t)(char *, size_t, const char *, ...);
typedef float (*strtof_t)(char *, char **);
typedef long double (*strtold_t)(char *, char **);

snprintf_t libc_snprintf;
strtof_t libc_strtof;
strtold_t libc_strtold;

void init_libc_func() {
    libc_snprintf = (snprintf_t) dlsym(RTLD_NEXT, "snprintf");
    libc_strtof = (strtof_t) dlsym(RTLD_NEXT, "strtof");
    libc_strtold = (strtold_t) dlsym(RTLD_NEXT, "strtold");
}

/*
 * Note: test outputs SHALL NOT use the cout stream
 * OR snprintf, because those functions are overridden
 * by muslflt. We cannot depend on the correctness of
 * this library in order to run tests for this library.
 *
 * This is why this library uses an ad-hoc simple test
 * framework instead of an established one.
 */

#define DEFINE_TEST(f) int f()

#define TEST_OK return 0;

#define TEST_CHECK(cond) \
    if (!(cond)) {\
        printf("\n\t"#cond" FAILED\n");\
        return -1;\
    }

#define SKIP_TEST_IF(cond) \
    if (cond) return 255

#define DO_TEST(f) \
    printf("Test %s %s ", #f, string(40 - strlen(#f), '.').c_str());\
    int res_##f = f(); \
    if (res_##f == 255) { \
        printf("SKIPPED\n"); \
    } else if (res_##f) { \
        printf("Test %s FAILED\n", #f); \
    } else { \
        printf("PASSED\n"); \
    }

#define _TEST_SNPRINTF_STRTOF(input, type, _snprintf, _strtof, expect_fail) \
    type test = input; \
    char str[255]; \
    _snprintf(str, 255, "%.*g", FLT_DIG, test); \
    type test2 = _strtof(str, NULL); \
    if (expect_fail) { \
        TEST_CHECK(test != test2); \
    } else { \
        TEST_CHECK(test == test2); \
    } \
    TEST_OK
#define TEST_SNPRINTF_STRTOF(input, type, _snprintf, _strtof) \
    _TEST_SNPRINTF_STRTOF(input, type, _snprintf, _strtof, false)

#define TEST_SNPRINTF_STRTOF_CPP(input, type) \
    type test = input; \
    stringstream ss; \
    ss << test; \
    type test2; \
    ss >> test2; \
    TEST_CHECK(test == test2); \
    TEST_OK \

#define _TEST_SNPRINTF(input, type, _snprintf, precision, expect, expect_fail) \
    type test = input; \
    char str[255]; \
    _snprintf(str, 255, "%.*Lf", precision, test); \
    if (expect_fail) { \
        TEST_CHECK(string(str) != expect); \
    } else { \
        TEST_CHECK(string(str) == expect); \
    } \
    TEST_OK
#define TEST_SNPRINTF(input, type, _snprintf, precision, expect) \
    _TEST_SNPRINTF(input, type, _snprintf, precision, expect, false)

#define TEST_SNPRINTF_CPP(input, type, precision, expect) \
    type test = input; \
    stringstream ss; \
    ss << setiosflags(std::ios::fixed) << setprecision(precision) << test; \
    string s; \
    ss >> s; \
    TEST_CHECK(s == expect); \
    TEST_OK

// Confirm the libc versions are indeed broken
DEFINE_TEST(strtof_should_break_1_capnproto) {
    _TEST_SNPRINTF_STRTOF(1e37, float, libc_snprintf, libc_strtof, true);
}

DEFINE_TEST(snprintf_should_break_1_redis) {
#ifdef __aarch64__
    SKIP_TEST_IF(true);
#endif
    _TEST_SNPRINTF(17179869184L + 1.5, long double, libc_snprintf, 17,
        "17179869185.50000000000000000", true);
}

DEFINE_TEST(strtof_1_capnproto) {
    TEST_SNPRINTF_STRTOF(1e37, float, libc_snprintf, strtof);
}

DEFINE_TEST(strtof_snprintf_1_capnproto) {
    TEST_SNPRINTF_STRTOF(1e37, float, snprintf, strtof);
}

DEFINE_TEST(strtof_snprintf_cpp_1_capnproto) {
    TEST_SNPRINTF_STRTOF_CPP(1e37, float);
}

DEFINE_TEST(strtof_2_capnproto) {
    TEST_SNPRINTF_STRTOF(-1e37, float, libc_snprintf, strtof);
}

DEFINE_TEST(strtof_snprintf_2_capnproto) {
    TEST_SNPRINTF_STRTOF(-1e37, float, snprintf, strtof);
}

DEFINE_TEST(strtof_snprintf_cpp_2_capnproto) {
    TEST_SNPRINTF_STRTOF_CPP(-1e37, float);
}

DEFINE_TEST(strtof_3_capnproto) {
    TEST_SNPRINTF_STRTOF(1e306, double, libc_snprintf, strtod);
}

DEFINE_TEST(strtof_snprintf_3_capnproto) {
    TEST_SNPRINTF_STRTOF(1e306, double, snprintf, strtod);
}

DEFINE_TEST(strtof_snprintf_cpp_3_capnproto) {
    TEST_SNPRINTF_STRTOF_CPP(1e306, double);
}

DEFINE_TEST(strtof_4_capnproto) {
    TEST_SNPRINTF_STRTOF(-1e306, double, libc_snprintf, strtod);
}

DEFINE_TEST(strtof_snprintf_4_capnproto) {
    TEST_SNPRINTF_STRTOF(-1e306, double, snprintf, strtod);
}

DEFINE_TEST(strtof_snprintf_cpp_4_capnproto) {
    TEST_SNPRINTF_STRTOF_CPP(-1e306, double);
}

DEFINE_TEST(snprintf_1_redis) {
    TEST_SNPRINTF(17179869184L + 1.5, long double, snprintf, 17,
        "17179869185.50000000000000000");
}

DEFINE_TEST(snprintf_cpp_1_redis) {
    TEST_SNPRINTF_CPP(17179869184L + 1.5, long double, 17,
        "17179869185.50000000000000000");
}

DEFINE_TEST(snprintf_2_redis) {
    TEST_SNPRINTF(17179869184L + 17179869184L, long double, snprintf, 17,
        "34359738368.00000000000000000");
}

DEFINE_TEST(snprintf_cpp_2_redis) {
    TEST_SNPRINTF_CPP(17179869184L + 17179869184L, long double, 17,
        "34359738368.00000000000000000");
}

int main() {
    init_libc_func();
    DO_TEST(strtof_should_break_1_capnproto);
    DO_TEST(strtof_1_capnproto);
    DO_TEST(strtof_snprintf_1_capnproto);
    DO_TEST(strtof_snprintf_cpp_1_capnproto);
    DO_TEST(strtof_2_capnproto);
    DO_TEST(strtof_snprintf_2_capnproto);
    DO_TEST(strtof_snprintf_cpp_2_capnproto);
    DO_TEST(strtof_3_capnproto);
    DO_TEST(strtof_snprintf_3_capnproto);
    DO_TEST(strtof_snprintf_cpp_3_capnproto);
    DO_TEST(strtof_4_capnproto);
    DO_TEST(strtof_snprintf_4_capnproto);
    DO_TEST(strtof_snprintf_cpp_4_capnproto);
    DO_TEST(snprintf_should_break_1_redis);
    DO_TEST(snprintf_1_redis);
    DO_TEST(snprintf_cpp_1_redis);
    DO_TEST(snprintf_2_redis);
    DO_TEST(snprintf_cpp_2_redis);

    return 0;
}
