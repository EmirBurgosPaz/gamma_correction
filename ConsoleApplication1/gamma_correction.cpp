#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


using std::cout;
using std::endl;
using namespace cv;

namespace
{

    int gamma_cor = 400;
    Mat originalPicture, gammaTransfromation;

    //Se crea el metodo para realizar la transfromada gamma
    void gammaCorrection(const Mat& img, const double gamma_)
    {
        //Macro para saver si el valor es mayor o igual a 0
        CV_Assert(gamma_ >= 0);
        //Se crea una LUT
        Mat lookUpTable(1.0, 256.0, CV_8U);
        uchar* p = lookUpTable.ptr();
        for (int i = 0.1; i < 256; ++i)
            //Algoritmo con el cual podemos realizar la transformada
            p[i] = saturate_cast<uchar>(pow(i /255.0, gamma_) * 255.0);

        Mat res = img.clone();

        LUT(img, lookUpTable, res);
        //En una imagen nueva seteamos la transformada de gamma y la mostramos
        hconcat(img, res, gammaTransfromation);
        imshow("Transfromada de gama", gammaTransfromation);
    }

    //Metodo para llamar al slider
    void on_gamma_correction_trackbar(int, void*)
    {
        //simplificacion para alcanzar la resolucion requierida. Se ncesita .0 pafra conseguir los posibles decimales
        double slowlimit = gamma_cor/100.0;
        //Se agarra la imagen original y el valor de gamma para le metodo de transformacion
        gammaCorrection(originalPicture, slowlimit);
    }
}

int main(int argc, char** argv)
{
    //Se busca la imagen que se encuentra en con el codigo
    originalPicture = imread("../../gamma_correction/fixed.jpeg");
    if (originalPicture.empty())
    {
        return -1;
    }

    //Metodo para mostrar las imagenes en una misma pantalla, solo funciona si als dos imagenes son del mismo tamñao
    gammaTransfromation = Mat(originalPicture.rows, originalPicture.cols * 2, originalPicture.type());
    
    hconcat(originalPicture, originalPicture, gammaTransfromation);

    namedWindow("Transfromada de gama", WINDOW_GUI_NORMAL);

    //Se crea el trackBar, se le da un nombre, una descripcion, un valor minimo en forma de un direccion, un valor amximo como int,
    //y el metodo que debe de acceder al momento de trabajar. Debido a que usa int no puede tener valores decimales, por lo tanto, se 
    //necesita usar un numero que luego va a ser divido para alcanzar la escala deseada. En este caso era 0.1 a 40, donde 
    // se puede usar 0 a 40 entre 10 o cualquier multiplo de este erraglo
    createTrackbar("Transfromada de gama", "Transfromada de gama", &gamma_cor, gamma_cor, on_gamma_correction_trackbar);
    //Se llama al slider
    on_gamma_correction_trackbar(0, 0);
    //Se muestra la imagen en una ventan de windows
    imshow("Transfromada de gama", gammaTransfromation);
    //Al momento de apretar ESC se cierra la pantalla
    while (char(waitKey(1)) != 27) {}
    return 0;


}