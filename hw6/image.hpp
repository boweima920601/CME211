#include <boost/multi_array.hpp>
#include <string>

#ifndef IMAGE_HPP
#define IMAGE_HPP

class image {
	boost::multi_array<unsigned char,2> input;
	
public:
 	image(std::string filename);
 	unsigned int Sharpness();
 	void Boxblur (unsigned int size_k);
 	void Save(std::string save_filename);
  
};

#endif /* IMAGE_HPP */