/* in class image, we have three methods
// sharpness: use laplacian operator and convolution function 
// Boxblur: define different kernel sizes and then blur the image
// Save: save the result image
*/
#include <boost/multi_array.hpp>
#include <iostream>
#include <string>
#define BOOST_DISABLE_ASSERTS
#include "image.hpp"
#include "hw6.hpp"

void Convolution(boost::multi_array<unsigned char,2> &input,
                   boost::multi_array<unsigned char,2> &output,
                   boost::multi_array<float,2>         &kernel)
{
                   
	auto shape = input.shape();
	unsigned int width   = (unsigned int)shape[1];
    unsigned int height  = (unsigned int)shape[0];
    auto shape2 = output.shape();
	unsigned int width2   = (unsigned int)shape2[1];
    unsigned int height2  = (unsigned int)shape2[0];
    auto shape_k = kernel.shape();
    unsigned int size_k = (unsigned int)shape_k[0];
    unsigned int half_k = (unsigned int) ((size_k -1) / 2);
    boost::multi_array<unsigned char,2> extend_image (boost::extents[height+size_k-1][width + size_k-1]);
    
    if ((width != width2) or (height != height2)){
    	std::cout<<"input and output should be of the same size!"<<std::endl;
    }
    
    if ((size_k % 2 != 1) and (size_k <3)){
    	std::cout<<"size of kernel should be odd and be at least 3!"<<std::endl;
    }
    
/*in order to make the convolution process more easier, i decide to extend the original image to a larger image.
//thus, we can do the convolution on a entire image without taking the boundary conditions seperately
*/    
    for (unsigned int i= 0; i <half_k ; i++){
    	for (unsigned int j= 0 ; j< half_k; j++){
    		extend_image[i][j]=input[0][0];
    	}
    		
    } 
    
    for (unsigned int i= 0; i <half_k ; i++){
    	for (unsigned int j= 0 ; j< half_k; j++){
    		extend_image[height+i+half_k][j]=input[height-1][0];
    	}
    		
    } 
    
    for (unsigned int i= 0; i <half_k ; i++){
    	for (unsigned int j= 0 ; j< half_k; j++){
    		extend_image[i][width+j+half_k]=input[0][width-1];
    	}
    		
    } 
    
    for (unsigned int i= 0; i <half_k ; i++){
    	for (unsigned int j= 0 ; j< half_k; j++){
    		extend_image[height+i+half_k][width+j+half_k]=input[height-1][width-1];
    	}
    		
    } 
    
    for(unsigned int i = 0; i < height; i++){
		for(unsigned int j = 0; j < half_k; j++){
			extend_image[i + half_k][j] = input[i][0];
		}
	}
	
	 for(unsigned int i = 0; i < height; i++){
		for(unsigned int j = 0; j < half_k; j++){
			extend_image[i + half_k][width+j+half_k] = input[i][width-1];
		}
	}
	
	 for(unsigned int i = 0; i < half_k; i++){
		for(unsigned int j = 0; j < width; j++){
			extend_image[i][j+half_k] = input[0][j];
		}
	}
	
    
    for(unsigned int i = 0; i < half_k; i++){
		for(unsigned int j = 0; j < width; j++){
			extend_image[height+ i + half_k][j + half_k] = input[height-1][j];
		}
	}
	
	for(unsigned int i = 0; i < height; i++){
		for(unsigned int j = 0; j < width; j++){
			extend_image[i + half_k][j + half_k] = input[i][j];
		}
	}
    
   
      
//convolution process on a larger image
    for (unsigned int i=0; i<height; i++){
    	for (unsigned int j=0; j<width; j++){
    		float sum=0;
    		for (unsigned int m= 0 ; m<size_k; m++){
    			for (unsigned int n= 0; n< size_k; n++){
    				sum += extend_image[i+m][j+n] * kernel[m][n];
    			}
    		}
    		
    		if ( sum> 255){
    			sum=255;
    		}
    		if (sum< 0){
    			sum=0;
    		}
    		
    		output[i][j]= (unsigned char)sum;	
    		
    	}
    }               
                   
}
//constructor 
image::image(std::string filename){
	ReadGrayscaleJPEG(filename, input);
}

//calculate the sharpness of the image, use laplacian operator
unsigned int image::Sharpness() {
	boost::multi_array<float, 2> laplacian(boost::extents[3][3]);
	laplacian[0][0]=0; laplacian[0][1]=1; laplacian[0][2]=0;
	laplacian[1][0]=1; laplacian[1][1]=-4; laplacian[1][2]=1;
	laplacian[2][0]=0; laplacian[2][1]=1 ; laplacian[2][2]=0;
	auto output = input;
	Convolution(input, output, laplacian);
	auto shape=output.shape();
	unsigned int width=(unsigned int)shape[1];
	unsigned int height=(unsigned int)shape[0];
	unsigned char key=0;
	unsigned int result;
	for (unsigned int i=0; i<height; i++){
		for (unsigned int j=0; j<width; j++){
			if (output[i][j] > key){
				key=output[i][j];
			}
		}
	}
	result=(unsigned int)key;
	return result;
	
	
}

void image::Boxblur (unsigned int size_k){
	boost::multi_array<float,2> kernel(boost::extents[size_k][size_k]);
	for (unsigned int i=0; i<size_k; i++){
		for (unsigned int j=0;j<size_k;j++){
			kernel[i][j]=(float)1/ (float)(size_k * size_k);
		}
	}
	auto output= input;
	Convolution (output, input, kernel); //to avoid reload the image, the convolution will make directly on input
	
}

void image::Save(std::string save_filename){
	if(save_filename.empty()){
  		save_filename = "stanford.jpg";
	}
   	WriteGrayscaleJPEG(save_filename, input);
  
}




