#Image Analysis Filters

A simple C++ project for applying filters to raw images in terminal

Author: Alberto De Bortoli  
Date: august 2008  
Course: Elaborazione delle Immagini (Image Analysis) Università degli studi di Padova, Prof. Paolo Cattani  
Website: [albertodebortoli.it](http://www.albertodebortoli.it)   

Compile the project with 'make' command with g++ (tested on version 4.2.1).
Executable file 'ImageFilters' is generated for your architecture. 

##Description and usage

Source code files in the project:

- Main.cpp
- Image.{h,cpp}
- FiltersPunctual.{h,cpp}
- FiltersConvolution.{h,cpp}
- FiltersAdj.{h,cpp}
- FiltersStripes.{h,cpp}
- FT.{h,cpp}
- FFT.{h,cpp}

The project implement the following:

* Custom image size;
* FT and FFT for high or low pass filter with cutoff value;
* Implemented filters: Brightness, Auto Contrast, Gamma Correction, Gamma Correction on channels, Invert, Mirror, Pixelazer, Smooth, Noise Removal, Blur, Outliner (Emboss), Sharpen, Rotation;
* Low pass filter without using FT.

Execution needs at least 7 command line arguments. 6 base arguments and 1 for the chosen filter to apply to the source image. First 6 base arguments are described as follow:

1. path source image (*.raw)
2. path immagine destinazione (*.raw)
3. source image width
4. source image height
5. kind of source image ('0' for grayscale, '1' for color RGB interleaved)
6. kind of destination image ('0' for grayscale, '1' for color RGB interleaved)

Here is an example of usage with the first 6 arguments:

	./ImageFilters ./img/400x300RGB.raw ./img/output.raw 400 300 1 0

One more argument (at least) is needed for filter application. Filters can be applied sequentially. Filters description follows.

###Equalize
Spread the hystogram of gray over all the range of color space.  
syntax: `e`  
example: `e`  

###Brightness
Modify the image brightness using a given value.    
syntax: `l <mod_value>`  
example: `l 2.0`  

###Automatic Contrast
Apply auto contrast filter. Spread the hystogram to use the entire range.  
syntax: `a`  
example: `a`   

###Gamma Correction
Apply Gamma Correction filter with a given value.  
syntax: `g <mod_value>`  
example: `g 0.8`  

###Channel
Apply Gamma Correction filter on a given RGB channel with a given value. Only applicable to color images.  
syntax: `h <mod_value> <channel>`    
example: `h 0.8 R`  

###Invert
Invert the colors in the image.    
syntax: `i`  
example: `i`   

###Mirror
Mirror the image. If \<mod_value\> is the direction ('X' for horizontally, 'Y' for vertically).  
syntax: `m <mod_value>`  
example: `m X`  

###Pixelizer
Apply the pixelize filter. \<mod_value\> is the size of the macro pixel in the output image.  
syntax: `p <mod_value>`  
example: `p 6`  

###Smooth
Apply smooth effect using a \<kernel_dim\>x\<kernel_dim\> kernel matrix.   
syntax: `s <kernel_dim>`  
example: `s 5`  

###Remove noise
Remove the noise using a \<kernel_dim\>x\<kernel_dim\> kernel matrix.    
syntax: `r <kernel_dim>`  
example: `r 5`  

###Sharpen - Blur - Outline
Apply a convolution filter using a 3x3 kernel matrix.
\<filter_type\> can be:  
'S' for Sharpen filter  
'B' for Blur filter  
'O' for Outliner (emboss filter)    
syntax: `c <filter_type>`  
example: `c S`  

###Rotation
Rotate the image of <rotation_angle> degrees.  
syntax: `q <rotation_angle>`  
example: `q 45`  

###Zoom
Zoom in or out. \<zoom_flag\> can be '-' for zoom in or '+' for zoom out. \<mod_value\> is the zoom percentage.    
syntax: `z <zoom_flag> <mod_value>`  
example: `z + 30`  

###Resize
Resize the image. Output image has <mod_value> width, height is proportionally derived.
syntax:	`Z <mod_value>`  
example: `Z 250`  

###Grayscale
Convert a color image (RGB) to grayscale. Only applicable to grayscale images.  
syntax:	`b`  
example: `b`  

###Low-Pass filter
Cuts off high frequencies. \<cutoff_value\> (the cut off value) and \<kernel_dim\> (the kernel dimension) result in a stronger or weaker filter effect.  
syntax:	`- <cutoff_value> <kernel_dim>`  
example: `- 15 5`  

##Fourier Transform
Two kind of Fourier Transform can be used: FT and FFT. They are used for frequency filtering (low-pass/high-pass filter).
Classic FT runs in N^2, FFT runs in N log N.

syntax: `<FT_type> <filter_type> <cutoff_value> <filtering_type>`  
example: `f l 30 I`  

\<FT_type\> is for FT ('f') or for FFT ('F').  
\<filter_type\> can be 'l' o 'h' based on low or high filter application.  
\<cutoff_value\> is the cutoff value.  
\<filtering_type\> is for ideal filtering (ILPF/IHPF) ('I') or bell shaped 'B' one.  

Application of filter automatically saves spectrum in ‘magnitude.raw’ file for later access.

Here are example of usage of FT. Source image is 128x128 grayscale.

![Original image](http://www.albertodebortoli.it/CS/ImgAn/img/image001.png)  
Original image

![Transform spectrum](http://www.albertodebortoli.it/CS/ImgAn/img/image002.png)  
Transform spectrum

Here are the spectrums and the image as result of filter application.  
Low-pass filter, ideal, threshold = 30  
syntax: `./ImageFilters ./img/128x128BN.raw ./img/output.raw 128 128 0 0 F l 30 I`

![](http://www.albertodebortoli.it/CS/ImgAn/img/image003.png)

![](http://www.albertodebortoli.it/CS/ImgAn/img/image004.png)

Low-pass filter, bell shaped, threshold = 30  
syntax: `./ImageFilters ./img/128x128BN.raw ./img/output.raw 128 128 0 0 F l 30 C`

![](http://www.albertodebortoli.it/CS/ImgAn/img/image005.png)

![](http://www.albertodebortoli.it/CS/ImgAn/img/image006.png)

High-pass filter, ideal, threshold = 10  
syntax: `./ImageFilters ./img/128x128BN.raw ./img/output.raw 128 128 0 0 F h 10 I`

![](http://www.albertodebortoli.it/CS/ImgAn/img/image007.png)

![](http://www.albertodebortoli.it/CS/ImgAn/img/image008.png)

High-pass filter, ideal, threshold = 10  
syntax: `./ImageFilters ./img/128x128BN.raw ./img/output.raw 128 128 0 0 F h 10 C`

![](http://www.albertodebortoli.it/CS/ImgAn/img/image009.png)

![](http://www.albertodebortoli.it/CS/ImgAn/img/image010.png)

Source images need to be power of 2 size to apply FFT. If not, scaling to next power of 2 is applied filling empty spaces with 0 (black). The image is then trimmed back to original size fter FFT.

![Source image 160x100](http://www.albertodebortoli.it/CS/ImgAn/img/image010.png)  
Source image 160x100

![Scaled image to power of 2 size, i.e 256x128](http://www.albertodebortoli.it/CS/ImgAn/img/image012.png)  
Scaled image to power of 2 size, i.e 256x128


![Scaled image spectrum](http://www.albertodebortoli.it/CS/ImgAn/img/image013.png)    
Scaled image spectrum


![Image after backward FFT](http://www.albertodebortoli.it/CS/ImgAn/img/image014.png)   
Image after backward FFT


![Cut image to original size 160x100](http://www.albertodebortoli.it/CS/ImgAn/img/image015.png)  
Cut image to original size 160x100

Following images show the differences between hystogram equalization of pre and post equalization.

![Equalizzazione pre filtraggio](http://www.albertodebortoli.it/CS/ImgAn/img/image016.png)
	![](http://www.albertodebortoli.it/CS/ImgAn/img/image018.png)  
Pre filtering equalization

![Equalizzazione post filtraggio](http://www.albertodebortoli.it/CS/ImgAn/img/image017.png)
	![](http://www.albertodebortoli.it/CS/ImgAn/img/image019.png)  
Post filtering equalization 

##License

Licensed under the New BSD License.

Copyright (c) 2008, Alberto De Bortoli at
Università degli Studi di Padova. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Università degli Studi di Padova nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Alberto De Bortoli ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL Alberto De Bortoli BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
