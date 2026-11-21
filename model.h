

#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;
#include "math.h"
#include "ap_fixed.h"
#include <cstdint>

#define kNumActivationFunctions 2
#define POLY_ORDER 2
#define SIGMOID 0
#define TANH 1

#define NUM_SAMPLES 20

#define ZMIN_LOC -4
#define ZMAX_LOC 4

#define ZMIN -8
#define ZMAX 8

// typedef float Poly_coeff_t;
// typedef float NL_inp_arg_t;
// typedef float NL_out_arg_t;

typedef ap_fixed<16, 6> Poly_coeff_t;

typedef ap_fixed<16, 6> param_coeff_t;
typedef ap_fixed<16, 6> layer_t;

void Activation(
    unsigned int activation_mode,
    layer_t z,
    layer_t *y);

void tanh_reference(double z, double *y);
void sigmoid_reference(double z, double *y);

#define LAYER1 784
#define LAYER2 25
#define LAYER3 10

// float activation(float val);
void Model(const layer_t input[28][28], int *output);

#define IMG_WORDS 25
void mlp_top(const uint32_t img_bits[IMG_WORDS], int *output);