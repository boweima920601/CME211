#include <string>
#include <boost/multi_array.hpp>
#define BOOST_DISABLE_ASSERTS
#include <iostream>

#include "image.hpp"

image::image(std::string input) {
	this->input = input;
	ReadGrayscaleJPEG(input, this->img);
}

void image::Save(std::string output) {
	// check whether the output filename is empty
	if (output.empty()) {
		WriteGrayscaleJPEG(this->input, this->img);
	}
	else {
		WriteGrayscaleJPEG(output, this->img);
	}
}

void image::Convolution(boost::multi_array<unsigned char,2> &input, 
				boost::multi_array<unsigned char,2> &output,
				boost::multi_array<float,2> &kernel) {

	unsigned int nrows = (unsigned int)(input.shape()[0]);
	unsigned int ncols = (unsigned int)(input.shape()[1]);
	unsigned int kn = (unsigned int)(kernel.shape()[0]);
	unsigned int radius = (kn - 1)/2;

/*
	Basic idea: Create a larger image of size (nrows + 2 * radius) * (ncols + 2 * radius) to extend the original image
	and overcome the difficulty of dealing the corner cases

*/
	boost::multi_array<unsigned char,2> large_img(boost::extents[nrows + 2 * radius][ncols + 2 * radius]);

	// check if the kernel conditions are met
	if ((kn != kernel.shape()[1]) || (kn < 3) || (kn % 2 == 0)) {
		std::cerr << "Invalid kernel size. Provide a square kernel with odd dimension at least 3." << std::endl;
		exit(1);
	}

	// check if the input and output have the same dimension
	if (nrows != output.shape()[0] || ncols != output.shape()[1]) {
		std::cerr << "ERROR: Input and output image size does not match." << std::endl;
		exit(1);
	}
	
	// start expand the image
	//upper-left corner
	for (unsigned int i = 0; i < radius ; i++) {
    	for (unsigned int j = 0 ; j < radius; j++) {
    		large_img[i][j] = input[0][0];
    	}
    }

    // lower-left corner
    for (unsigned int i = 0; i < radius ; i++) {
    	for (unsigned int j = 0 ; j < radius; j++) {
    		large_img[nrows + i + radius][j] = input[nrows - 1][0];
    	}
    }
 
 	// upper-right corner 
    for (unsigned int i = 0; i < radius ; i++) {
    	for (unsigned int j = 0 ; j < radius; j++) {
    		large_img[i][ncols + j + radius] = input[0][ncols - 1];
    	}
    }
    
    // lower-right corner
    for (unsigned int i = 0; i < radius ; i++) {
    	for (unsigned int j = 0 ; j < radius; j++) {
    		large_img[nrows + i + radius][ncols + j + radius] = input[nrows - 1][ncols - 1];
    	}
    }

	// middle-left    
    for(unsigned int i = 0; i < nrows; i++) {
		for(unsigned int j = 0; j < radius; j++) {
			large_img[i + radius][j] = input[i][0];
		}
	}

	// middle-right	
	for(unsigned int i = 0; i < nrows; i++) {
		for(unsigned int j = 0; j < radius; j++) {
			large_img[i + radius][ncols + j + radius] = input[i][ncols - 1];
		}
	}
	
	// middle-top
	for(unsigned int i = 0; i < radius; i++) {
		for(unsigned int j = 0; j < ncols; j++) {
			large_img[i][j + radius] = input[0][j];
		}
	}

	// middle-bottom
    for(unsigned int i = 0; i < radius; i++) {
		for(unsigned int j = 0; j < ncols; j++) {
			large_img[nrows + i + radius][j + radius] = input[nrows - 1][j];
		}
	}

	// all the original pixels in the middle	
	for(unsigned int i = 0; i < nrows; i++) {
		for(unsigned int j = 0; j < ncols; j++) {
			large_img[i + radius][j + radius] = input[i][j];
		}
	}      

	// compute the convuluted pixel greyscale value for each position
    for (unsigned int i = 0; i < nrows; i++) {
    	for (unsigned int j = 0; j < ncols; j++) {
    		float blurred = 0;
    		for (unsigned int k = 0 ; k < kn; k++) {
    			for (unsigned int l = 0; l < kn; l++) {
    				blurred += large_img[i + k][j + l] * kernel[k][l];
    			}
    		}

			// deal with under/over-flow issue    		
    		if (blurred < 0)
    			blurred = 0;
    		if (blurred > 255)
    			blurred = 255;

    		
    		output[i][j] = (unsigned char)blurred;	
    		
    	}
    }
}

void image::BoxBlur(unsigned int s) {

	boost::multi_array<float,2> kernel(boost::extents[s][s]);

	for (unsigned int i = 0; i < s; i++) {
		for (unsigned int j = 0; j < s; j++) {
			float dim = (float)s;
			kernel[i][j] = (float)1/(dim * dim);
		}
	}

	auto img_copy = this->img;
	image::Convolution(img_copy, this->img, kernel);

}

unsigned int image::Sharpness() {

	// approximation of the Laplacian operator
	boost::multi_array<float, 2> sharp_ker(boost::extents[3][3]);
	sharp_ker[0][0] = 0;
	sharp_ker[0][2] = 0;
	sharp_ker[2][0] = 0;
	sharp_ker[2][2] = 0;
	sharp_ker[0][1] = 1;
	sharp_ker[1][2] = 1;
	sharp_ker[2][1] = 1;
	sharp_ker[1][0] = 1;
	sharp_ker[1][1] = -4;

	unsigned int nrows = (unsigned int)this->img.shape()[0];
	unsigned int ncols = (unsigned int)this->img.shape()[1];
	boost::multi_array<unsigned char, 2> sharped_img(boost::extents[nrows][ncols]);
	image::Convolution(this->img, sharped_img, sharp_ker);

	// find the maximum sharpness value
	unsigned char max_sharp = 0;
	for (unsigned int i = 0; i < nrows; i++) {
		for (unsigned int j = 0; j < ncols; j++) {
			if (sharped_img[i][j] > max_sharp)
				max_sharp = sharped_img[i][j];
		}
	}
	return (unsigned int)max_sharp;
}