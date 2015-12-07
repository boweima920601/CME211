/* filename: main.cpp
// Two steps:
// (1)-> load the original image, compute and output the sharpness
// (2)-> use different kernel sizes to blur the original file and output the corresponding sharpness
*/

#include <iostream>
#include <string>
#include <boost/multi_array.hpp>
#define BOOST_DISABLE_ASSERTS
#include "hw6.hpp"
#include "image.hpp"

int main( ){

	image picture("stanford.jpg");
	std::cout<<"Original image: "<<picture.Sharpness()<<std::endl;
	//--style_0
  //--Can also just do  for (int i = 3; i < 28; i += 4)
  //--START
	for (int i=0; i<7; i++){
		int size_k;
		size_k= 3+ 4 * i;
		//--END
		image picture("stanford.jpg");
		picture.Boxblur(size_k);
		std::cout<<"BoxBlur("<<size_k<<"):\t"<<picture.Sharpness()<<std::endl; 
		std::string save_filename;
		
		std::string str_number;
		str_number=std::to_string(size_k); //change int to string
		
		save_filename= "BoxBlur"+str_number +".jpg";
		picture.Save(save_filename);   //save file
		
		
	}
     
	return 0;

}

//--style_2
//--Poor git usage
//--END

//--functionality_2
//--Incorrect BoxBlurxx output names, missing leading 0's
//--END