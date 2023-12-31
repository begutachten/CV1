// TU Dresden, Inf, CV1 Ex1, Holger Heidrich
// Aufg 1, non-linear grey value transformation, solarization
// --
// This program plays with colors simulating solarization.	
// Clicking two times into the "GreyCurve"-image gives two points.
// Make these points be the extremals of a polynomial of degree three in the range 0..255 x 0..255.\n"
// The "GreyCurve"-image itself has size (512,512). Point (0,0) is upper left corner. \n"
// Use the polynomial y(x) to map values x of the input image to values y of an output image by creating and using a lookup table.\n"
// Saturate to 0 and 255 if the mapped grey / color values exceed these values. \n"
//-----------------------------------------------------------------------------
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <functional>
#include <queue>

#define MaxPoints 2
int nb_points=0;
double LUT [512];

cv::Mat MapCurveimage512;
cv::Mat image, mapped_result_img;
cv::Point    SrcPtInt[MaxPoints];

void help()
{
	std::cout <<
	"\nTU Dresden, Inf, CV1 Ex1, Holger Heidrich\n"
	"This program plays with colors simulating solarization.\n"
	"Clicking two times into the \"GreyCurve\"-image gives two points.\n"
	"Make these points be the extremals of a polynomial of degree three in the range 0..255 x 0..255.\n"
	"The \"GreyCurve\"-image itself has size (512,512). Point (0,0) is upper left corner. \n"
	"Use the polynomial y(x) to map values x of the input image to values y of an output image.\n"
	"Saturate to 0 and 255 if the mapped grey / color values exceed these values. \n"
	"Call:\n"
	"./image Image [image-name Default: fruits.jpg]\n" << std::endl;
}

void on_mouse( int event, int x, int y, int flags, void* param )
{
    switch( event )
    {
        case cv::EVENT_LBUTTONDOWN:
        {
            // ad new point to polygon
            if (nb_points < MaxPoints) 
            {  
               SrcPtInt[nb_points].x=x;
               SrcPtInt[nb_points].y=y;
               if (nb_points)
               {   // second point in SrcPtInt
				   
                  MapCurveimage512 = 0;

                  // read the two extrema points
                  int x1 = SrcPtInt[0].x;	
				      int x2 = SrcPtInt[1].x;	
                  int y1 = 511 - SrcPtInt[0].y; // "511 -" cause we want "0" in lower left corner
                  int y2 = 511 - SrcPtInt[1].y;
                  
                  double dx = x1 - x2;	
                  double dy = y1 - y2;
                  float x0 = (float)(x1 + x2) / 2;
                  float a = (float)(-2.0*dy / pow(dx, 3.0));
                  float b = (float)(-3.0 / 2.0*dy / dx);
                  float c = (float)((y1 + y2) / 2.0 + b*x0);
                for (int i = 0; i < 512; i++)
                {
                    float x = (float)i;
                    float y = a*pow(x - x0, 3.0) - b*x + c;
                    if (y < 0) y = 0;
                    if (y > 512) y = 512;
                    LUT[i] = 512 - y;
                    circle(MapCurveimage512, cv::Point(x, LUT[(int) x] ), 1, CV_RGB(255, 255, 255), 1);
                } 

                    // use the lookup table (LUT) to map the input image to the result image
                    // use the same LUT for each color channel (or fantasize)
                    for (int y = 0; y < image.rows; y++){
                        for (int x = 0; x < image.cols; x++){
                            for (int c = 0; c < 3; c++){
                                mapped_result_img.at<cv::Vec3b>(y, x)[c] = LUT[image.at<cv::Vec3b>(y, x)[c]];
                            }
                        }
                    }

                  // determine polynomial coefficients of y(x):=a*(x-x0)ｳ-b*x+c 
                  // (want to cheat? see end of file)

                  // create the LUT for that polynom and 
                  // draw the polynom in the MapCurveimage (pixelwise)
				      // your code for the polynomial and color transform (instead of the following line)
                  //line(MapCurveimage512, SrcPtInt[0], SrcPtInt[1], CV_RGB(255, 255, 255));	// remove that line example 
                  
                  // use the lookup table (LUT) to map the input image to the result image
                  // use the same LUT for each color channel (or fantasize) 

				      // show non-linear mapping curve
                  imshow("GreyCurve", MapCurveimage512);
				   
				      // show the result
				      imshow( "result image", mapped_result_img );

				      // mapped_result_img = image.clone();
                      nb_points = 0;
               } else nb_points++;
			   }
        }
        break;
    }
}  // void on_mouse( int event, int x, int y, int flags, void* param )


int main( int argc, char** argv )
{
	help();
    
	char* filename = argc == 3 ? argv[1] : (char*)"C:/Users/Linus/Desktop/Linus/Start/CV1/fruits.jpg";
    image = cv::imread(filename, 1);
	mapped_result_img = image.clone();

    cv::namedWindow( "GreyCurve");
    cv::namedWindow( "Fruits!");
	imshow( "Fruits!", mapped_result_img );
	
   MapCurveimage512.create(512, 512, CV_8U);
   MapCurveimage512 = 0;
   imshow("GreyCurve", MapCurveimage512);

   cv::setMouseCallback( "GreyCurve", on_mouse, 0 );
   cv::waitKey();
    
    return 0;
    // all the memory will automatically be released by Vector<>, Mat and Ptr<> destructors.
}




































































































/*
// determine polynomial coefficients of y(x):=a*(x-x0)ｳ-b*x+c;
int x1 = SrcPtInt[0].x;	int y1 = 511 - SrcPtInt[0].y;
int x2 = SrcPtInt[1].x;	int y2 = 511 - SrcPtInt[1].y;
double        dx = x1 - x2;	double		  dy = y1 - y2;
float x0 = (float)(x1 + x2) / 2;
float a = (float)(-2.0*dy / pow(dx, 3.0));
float b = (float)(-3.0 / 2.0*dy / dx);
float c = (float)((y1 + y2) / 2.0 + b*x0);
*/