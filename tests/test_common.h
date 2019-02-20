#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#undef NDEBUG
#include <cassert>
#include <cmath>
#include <cstdio>

static void handle_assert(bool passed, const char* assert, const char* file, long line) {
    if (!passed) {
        printf("Assertion failed %s in %s at line %d\n", assert, file, (int)line);
        exit(-1);
    }
}

#define require(x) handle_assert(x, #x , __FILE__, __LINE__ )

#define require_throws(expr) {          \
            bool _ = false;             \
            try {                       \
                expr;                   \
            } catch (...) {             \
                _ = true;               \
            }                           \
            require(_);                 \
        }

#define require_nothrow(expr) {         \
            bool _ = false;             \
            try {                       \
                expr;                   \
            } catch (...) {             \
                _ = true;               \
            }                           \
            require(!_);                \
        }

class Approx {
public:
    Approx(double val, double eps = 0.001) : val(val), eps(eps) {
        assert(eps > 0);
    }

    double val, eps;
};

inline bool operator==(double val, const Approx &app) {
    return std::abs(val - app.val) < app.eps;
}

inline bool operator==(float val, const Approx &app) {
    return std::abs(val - app.val) < app.eps;
}

#endif
