#pragma once

#if defined(__clang__)
#define META clang::annotate("meta")
#define RANGE(a, b) clang::annotate("range", a, b)
#define INFO(str) clang::annotate("info", str)
#define STEP(step) clang::annotate("step", step)
#else
#define META
#define RANGE(a, b)
#define INFO(str)
#define STEP(step)
#endif
