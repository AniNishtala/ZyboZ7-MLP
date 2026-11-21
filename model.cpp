#include "model.h"

#include "weights.dat"
// #include "Y_test.dat"

const Poly_coeff_t coeff[2][3] = {
	{0.50548498, 0.25716315, -0.03532942},
	{0.505485001, 0.25716313, -0.03532932}};

void Activation(
	unsigned int activation_mode,
	layer_t z,
	layer_t *y)
{
	layer_t abs_z;
	layer_t y_tmp;

	// if (activation_mode == SIGMOID)
	// 	abs_z = abs(float(z));
	// if (activation_mode == TANH)
	// 	abs_z = abs(float(2*z));

	// changing from float to fixed points
	layer_t arg = (activation_mode == TANH) ? layer_t(2 * z) : z;
	abs_z = (arg < layer_t(0)) ? layer_t(-arg) : arg;

	y_tmp = coeff[activation_mode][0] + coeff[activation_mode][1] * abs_z + coeff[activation_mode][2] * abs_z * abs_z;

	if (z < 0.0)
		y_tmp = 1 - y_tmp;

	if (activation_mode == SIGMOID)
	{
		if (z > ZMAX_LOC)
			y_tmp = 1;
		else if (z < ZMIN_LOC)
			y_tmp = 0;

		*y = y_tmp;
	}
	else if (activation_mode == TANH)
	{
		if (z > ZMAX_LOC / 2)
			y_tmp = 1;
		else if (z < ZMIN_LOC / 2)
			y_tmp = 0;

		*y = (2 * y_tmp) - 1;
	}
}

void Model(const layer_t input[28][28], int *output)
{
#pragma HLS INLINE off
	layer_t b1[LAYER2] = {0.0};
	layer_t b2[LAYER3] = {0.0};

LAYER1_LOOP:
	for (int j = 0; j < LAYER1; ++j)
	{
		layer_t pixel = input[j / 28][j % 28];
	NODES1:
		for (int i = 0; i < LAYER2; i++)
		{
			b1[i] += pixel * w1[j][i];
		}
	}

LAYER1_ACT:
	for (int i = 0; i < LAYER2; ++i)
	{
		b1[i] = b1[i] + w1_b[i];
		Activation(SIGMOID, b1[i], &b1[i]);
	}

LAYER2_LOOP:
	for (int j = 0; j < LAYER2; ++j)
	{
	NODES2:
		for (int i = 0; i < LAYER3; i++)
		{
			b2[i] += b1[j] * w2[j][i];
		}
	}

LAYER2_ACT:
	for (int i = 0; i < LAYER3; ++i)
	{
		b2[i] = b2[i] + w2_b[i];
		Activation(SIGMOID, b2[i], &b2[i]);
	}

	int max_i = -1;
	layer_t max = -1.0;
COPMARE:
	for (int i = 0; i < LAYER3; ++i)
	{
		if (b2[i] > max)
		{
			max = b2[i];
			max_i = i;
		}
		// std::cout << b2[i] << " " << i << ", ";
	}
	// std::cout << std::endl;
	*output = max_i;
}

// axi-	lite wrapper
void mlp_top(const uint32_t img_bits[IMG_WORDS], int *output)
{
#pragma HLS INTERFACE mode = s_axilite port = return bundle = control
#pragma HLS INTERFACE mode = s_axilite port = img_bits bundle = control
#pragma HLS INTERFACE mode = s_axilite port = output bundle = control

	layer_t input[28][28];

UNPACK_BITS:
	for (int idx = 0; idx < LAYER1; ++idx)
	{
#pragma HLS PIPELINE II = 1
		uint32_t word = img_bits[idx >> 5];
		bool bit = (word >> (idx & 31)) & 1U;
		input[idx / 28][idx % 28] = bit ? layer_t(1.0) : layer_t(0.0);
	}

	Model(input, output);
}
