#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


using std::cout;
using std::endl;
using namespace cv;

namespace
{

    int gamma_cor = 400;
    Mat img_original, img_corrected, img_gamma_corrected;

    void gammaCorrection(const Mat& img, const double gamma_)
    {
        CV_Assert(gamma_ >= 0);
 
        Mat lookUpTable(1.0, 256.0, CV_8U);
        uchar* p = lookUpTable.ptr();
        for (int i = 0.1; i < 256; ++i)
            p[i] = saturate_cast<uchar>(pow(i /255.0, gamma_) * 255.0);

        Mat res = img.clone();
        LUT(img, lookUpTable, res);
    

        hconcat(img, res, img_gamma_corrected);
        imshow("Transfromada de gama", img_gamma_corrected);
    }


    void on_gamma_correction_trackbar(int, void*)
    {
        double slowlimit = gamma_cor/100.0;
        gammaCorrection(img_original, slowlimit);
    }
}

int main(int argc, char** argv)
{
    
    img_original = imread("../../gamma_correction/fixed.jpeg");
    if (img_original.empty())
    {
        return -1;
    }

    img_gamma_corrected = Mat(img_original.rows, img_original.cols * 2, img_original.type());

    hconcat(img_original, img_original, img_gamma_corrected);

    namedWindow("Transfromada de gama");

    createTrackbar("Transfromada de gama", "Transfromada de gama", &gamma_cor, gamma_cor, on_gamma_correction_trackbar);

    on_gamma_correction_trackbar(gamma_cor, 0);

    waitKey();

    imshow("Transfromada de gama", img_gamma_corrected);
    while (char(waitKey(1)) != 'w') {}
    return 0;


}