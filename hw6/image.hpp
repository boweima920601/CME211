#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

#include <boost/multi_array.hpp>
#include "hw6.hpp"

class image {
	// private data members
	boost::multi_array<unsigned char, 2> img;

	std::string input;

	// private functions
	void Convolution(boost::multi_array<unsigned char,2> &input, 
				boost::multi_array<unsigned char,2> &output,
				boost::multi_array<float,2> &kernel);

public:
	// constructor
	image(std::string input);

	// public functions for the main() to call
	void BoxBlur(unsigned int s);
	unsigned int Sharpness();
	void Save(std::string output);
};

#endif /* IMAGE_HPP */
