// #include "model.h"
// #include <fstream>
// #include <iostream>
// #include <cstdlib>

// int main(int argc, char **argv) {
//     const int sample_limit = argc > 1 ? std::atoi(argv[1]) : 50;
//     if (sample_limit < 1 || sample_limit > 10000) {
//         std::cerr << "Sample count must be between 1 and 10000\n";
//         return 1;
//     }
//     std::ifstream data("test_data.dat");
//     if (!data) {
//         std::cerr << "Cannot open test_data.dat\n";
//         return 1;
//     }

//     layer_t image[28][28];
//     int label, predicted;
//     int count = 0;
//     int correct = 0;

//     while (data >> label) {
//     	// Ingest
//         for (int row = 0; row < 28; row++) {
//             for (int col = 0; col < 28; col++) {
//                 float pixel;
//                 if (!(data >> pixel)) return 1;
//                 image[row][col] = pixel;
//             }
//         }

//         // Run Model
//         Model(image, &predicted);
//         // std::cout << predicted << " = " << label << std::endl;
//         if (predicted == label) correct++;
//         count++;

//         if (count % 100 == 0) {
//         	std::cout << "Processed " << count << std::endl;
//         }
//         // test break
//         if(count >= sample_limit) {
//         	break;
//         }
//     }

//     if (count != sample_limit) {
//         std::cerr << "Expected " << sample_limit << " images, got " << count << "\n";
//         return 1;
//     }
//     std::cout << "Samples: " << count << "\n";
//     if (count)
//         std::cout << "Accuracy: "
//                   << 100.0 * correct / count << "%\n";
//     return 0;
// }

// 1 bit packing test bench
#include "model.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdint>

static constexpr float BIN_THRESH = 0.5f;

int main(int argc, char **argv)
{
    const int sample_limit = argc > 1 ? std::atoi(argv[1]) : 50;

    std::ifstream data("test_data.dat");
    if (!data)
    {
        std::cerr << "Cannot open test_data.dat\n";
        return 1;
    }

    uint32_t img_bits[IMG_WORDS];
    int label = 0, predicted = 0;
    int count = 0;
    int correct = 0;

    while (data >> label)
    {
        // Clear all 25 x 32-bit words
        for (int w = 0; w < IMG_WORDS; ++w)
        {
            img_bits[w] = 0;
        }

        // Read 784 pixels and pack 1 bit per pixel
        for (int idx = 0; idx < LAYER1; ++idx)
        {
            float pixel;
            if (!(data >> pixel))
                return 1;
            if (pixel > BIN_THRESH)
            {
                img_bits[idx >> 5] |= (1U << (idx & 31));
            }
        }

        // Calling AXI-Lite wrapper top function
        mlp_top(img_bits, &predicted);

        if (predicted == label)
            correct++;
        count++;

        if (count % 500 == 0)
        {
            std::cout << "Processed " << count << std::endl;
        }
        if (count >= sample_limit)
        {
            break;
        }
    }

    if (count != sample_limit)
    {
        std::cerr << "Expected " << sample_limit << " images, got " << count << "\n";
        return 1;
    }

    std::cout << "Samples: " << count << "\n";
    std::cout << "Correct: " << correct << "\n";
    std::cout << "Accuracy: " << (100.0 * correct / count) << "%\n";
    return 0;
}