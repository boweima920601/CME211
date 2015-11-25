#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include <boost/multi_array.hpp>
#define BOOST_DISABLE_ASSERTS

#include "image.hpp"

int main() {
	auto img = image("stanford.jpg");

	unsigned int sharpness = img.Sharpness();

	std::cout << "Original image: " << sharpness << std::endl;

	for (unsigned int i = 3; i <= 27; i += 4) {
		// reload the initial image
		auto img = image("stanford.jpg");

		// format the output
		std::ostringstream out;
		if (i < 10)
			out << "BoxBlur0" << i << ".jpg";
		else
			out << "BoxBlur" << i << ".jpg";

		img.BoxBlur(i);
		img.Save(out.str());

		// compute and print out the sharpness of the blurred image
		unsigned int sharpness = img.Sharpness();

		std::cout << "BoxBlur(" << std::setw(2) << i << "): \t" << std::setw(3) << sharpness << std::endl;
	}

	return 0;
}

