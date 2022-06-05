#pragma once
#include <cmath>
#include <cinttypes>

static_assert(sizeof(float) == 4, "Assuming that a float is 4 byte long");
static_assert(sizeof(double) == 8, "Assuming that a double is 8 byte long");

typedef unsigned char Byte;
typedef unsigned int uint;
const unsigned int GZIP_BUFFER_SIZE = 128;
const unsigned int WRITE_BUFFER_SIZE = 256;
const unsigned int MIN_TAG = 1;
const unsigned int MAX_TAG = 12;

typedef signed char jbyte;
typedef int16_t jshort;
typedef int32_t jint;
typedef int64_t jlong;
typedef double jdouble;  // Needs 8 byte big - little endian
typedef float jfloat;    // Needs 4 byte big - little endian
