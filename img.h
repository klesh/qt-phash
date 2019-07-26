#ifndef IMG_H
#define IMG_H

#include "QtPhash.h"
#include <CImg.h>

using namespace cimg_library;

template<typename T>
CImg<T> qimg2cimg(const QImage &qimg) {
  int C = qimg.depth() / 8;
  int I = C > 1 ? 2 : 0;
  CImg<T> cimg(qimg.width(), qimg.height(), 1, C);
  for (uint c = 0; c < C; c++) {
    for (uint y = 0; y < qimg.height(); y++) {
      for (uint x = 0; x < qimg.width(); x++) {
        uint offset = c > 2 ? 3 : (C > 1 ? 2 - c : 0);
        cimg(x, y, 0, c) = qimg.scanLine(y)[x * C + offset];
      }
    }
  }
  return cimg;
}

template<typename T>
QImage cimg2qimg(const CImg<T> &cimg) {
  int bytesPerPixel = cimg.spectrum() == 1 ? 1 : 4;
  QImage qimg(cimg.width(), cimg.height(), bytesPerPixel == 1 ? QImage::Format_Grayscale8 : QImage::Format_RGB32);
  cimg_forXYC(cimg, x, y, c) {
    uint offset = c > 2 ? 3 : (bytesPerPixel > 1 ? 2 - c : 0);
    qimg.scanLine(y)[x * bytesPerPixel + offset] = cimg(x, y, c);
  }
  return qimg;
}

CImg<float>* ph_dct_matrix(const int N){
  CImg<float> *ptr_matrix = new CImg<float>(N,N,1,1,1/sqrt((float)N));
  const float c1 = sqrt(2.0/N); 
  for (int x=0;x<N;x++){
    for (int y=1;y<N;y++){
      *ptr_matrix->data(x,y) = c1*cos((cimg::PI/2/N)*y*(2*x+1));
    }
  }
  return ptr_matrix;
}

ulong ph_dct_imagehash(const char* file){
  if (!file){
    return -1;
  }
  CImg<uint8_t> src;
  try {
    src.load(file);
  } catch (CImgIOException ex){
    return -1;
  }
  CImg<float> meanfilter(7,7,1,1,1);
  CImg<float> img;
  if (src.spectrum() == 3){
    img = src.RGBtoYCbCr().channel(0).get_convolve(meanfilter);
  } else if (src.spectrum() == 4){
    int width = img.width();
    int height = img.height();
    int depth = img.depth();
    img = src.crop(0,0,0,0,width-1,height-1,depth-1,2).RGBtoYCbCr().channel(0).get_convolve(meanfilter);
  } else {
    img = src.channel(0).get_convolve(meanfilter);
  }

  img.resize(32,32);
  CImg<float> *C  = ph_dct_matrix(32);
  CImg<float> Ctransp = C->get_transpose();

  CImg<float> dctImage = (*C)*img*Ctransp;

  auto ss = dctImage.crop(1,1,8,8);
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
    }
  }
  CImg<float> subsec = ss.unroll('x');;

  float median = subsec.median();
  ulong one = 0x0000000000000001;
  ulong hash = 0x0000000000000000;
  for (int i=0;i< 64;i++){
    float current = subsec(i);
    if (current > median)
      hash |= one;
    one = one << 1;
  }

  delete C;

  return hash;
}

template<typename T>
void cimgPrintMatrix(CImg<T> cimg) {
  for (int row = 0; row < cimg.height(); row++) {
    QStringList list;
    for (int col = 0; col < cimg.width(); col++) {
      list.append(QString::number(*cimg.data(col, row), 'f', 3).rightJustified(6));
    }
    qDebug() << list.join(' ');
  }
}
#endif