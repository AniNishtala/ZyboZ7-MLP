#include "model.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv) {
    const int sample_limit = argc > 1 ? std::atoi(argv[1]) : 50;
    if (sample_limit < 1 || sample_limit > 10000) {
        std::cerr << "Sample count must be between 1 and 10000\n";
        return 1;
    }
    std::ifstream data("test_data.dat");
    if (!data) {
        std::cerr << "Cannot open test_data.dat\n";
        return 1;
    }

    layer_t image[28][28];
    int label, predicted;
    int count = 0;
    int correct = 0;

    while (data >> label) {
    	// Ingest
        for (int row = 0; row < 28; row++) {
            for (int col = 0; col < 28; col++) {
                float pixel;
                if (!(data >> pixel)) return 1;
                image[row][col] = pixel;
            }
        }

        // Run Model
        Model(image, &predicted);
        // std::cout << predicted << " = " << label << std::endl;
        if (predicted == label) correct++;
        count++;

        if (count % 100 == 0) {
        	std::cout << "Processed " << count << std::endl;
        }
        // test break
        if(count >= sample_limit) {
        	break;
        }
    }

    if (count != sample_limit) {
        std::cerr << "Expected " << sample_limit << " images, got " << count << "\n";
        return 1;
    }
    std::cout << "Samples: " << count << "\n";
    if (count)
        std::cout << "Accuracy: "
                  << 100.0 * correct / count << "%\n";
    return 0;
}