#ifndef MATH_H
#define MATH_H
#ifdef MAX
#undef MAX
#endif
#ifdef MIN
#undef MIN
#endif
#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) > (b)) ? (a) : (b)

#endif