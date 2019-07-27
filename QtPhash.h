#ifndef QTPHASH_H
#define QTPHASH_H

#include <QtCore>
#include <QImage>
#include <QGenericMatrix>
#include <QtDebug>
#include <utility>
#include <math.h>

namespace QtPhash {

  template<int N, int M, typename T>
  void printMatrix(const QGenericMatrix<N, M, T> &matrix) {
    for (int row = 0; row < N; row++) {
      QStringList list;
      for (int col = 0; col < N; col++) {
        list.append(QString::number(matrix(row, col), 'f', 3).rightJustified(6));
      }
      qDebug() << list.join(' ');
    }
  }

  template<int M, int N, typename T>
  QGenericMatrix<M,N,T> qimg2matrix(const QImage &qimg) {
    assert(qimg.format() == QImage::Format_Grayscale8);
    QGenericMatrix<M,N,T>  matrix;
    for (int row = 0; row < M; row++) {
      for (int col = 0; col < N; col++) {
        matrix(row, col) = qimg.scanLine(row)[col];
      }
    }
    return matrix;
  }

  template<int M, int N, typename T>
  QImage matrix2qimg(const QGenericMatrix<M, N, T> &matrix) {
    QImage qimg(N, M, QImage::Format_Grayscale8);
    for (int row = 0; row < M; row++) {
      for (int col = 0; col < N; col++) {
        qimg.scanLine(row)[col] = matrix(row, col);
      }
    }
    return qimg;
  }

  QImage blur(const QImage &source) {
    assert(source.format() == QImage::Format_Grayscale8);

    const uint KS = 7, w = source.width(), h = source.height();
    QImage target(w, h, source.format());  

    const float kernel[KS][KS] = { // box blur kernel
      {1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1},
    };
    const int kcx = KS/2, kcy = kcx; // center position of the kernel

    #pragma omp parallel for
    for (int y = 0; y < h; y++) { // fill out target pixel one by one.
      for (int x = 0; x < w; x++) {
        int ksx = x - kcx, ksy = y - kcy, kex = x + kcx + 1, key = y + kcy + 1;
        assert ( kex - ksx == KS );
        assert ( key - ksy == KS );
        ksx = ksx < 0 ? 0 : ksx; // kernel crop. (called Neumann method in CImg)
        ksy = ksy < 0 ? 0 : ksy; // ksx = kernel starting x, kex = kernel ending x
        kex = kex > w ? w : kex;
        key = key > h ? h : key;

        // calculate pixel value by image kernel
        float totalValue = 0.0, totalWeight = 0.0;
        for (int ky = ksy; ky < key; ky++) {
          for (int kx = ksx; kx < kex; kx++) {
            float weight = kernel[kx - ksx][ky - ksy];
            totalWeight += weight;
            float value = source.scanLine(ky)[kx];
            totalValue += weight * value;
          }
        }
        target.scanLine(y)[x] = totalValue / totalWeight;
      }
    }
    return target;
  }

  template<int N, typename T>
  QGenericMatrix<N, N, T> dctMatrix() {
    QGenericMatrix<N, N, T> dctMatrix;
    const float c0 = 1 / std::sqrt((float)N), c1 = std::sqrt(2.0/N);
    for (int row = 0; row < N; row++) {
      for (int col = 0; col < N; col++) {
        dctMatrix(row, col) = row == 0 ? c0 : c1 * std::cos((M_PI / 2 / N) * row * (2 * col + 1));
      }
    }
    return dctMatrix;
  }

  template<int N, int M, typename T>
  QGenericMatrix<M, M, T> matrixSubset(QGenericMatrix<N, N, T> source, int startRow = 0, int startCol = 0) {
    QGenericMatrix<M, M, T> target;
    for (int row = startRow; row < M + startRow; row++) {
      for (int col = startCol; col < M + startCol; col++) {
        target(row - startRow, col - startCol) = source(row, col);
      }
    }
    return target;
  }

  template<typename T>
  T kthSmallest(T *array, int index, int low, int high) {
    int l = low, r = high;

    while (true) {
      int m = (l + r + 1) >> 1;
      // middle-of-three: guess median value and set up boundary
      // l stores smallest, m stores largest, r stores median as pivot
      if (array[l] > array[m]) std::swap(array[l], array[m]);
      if (array[r] > array[m]) std::swap(array[r], array[m]);
      if (array[l] > array[r]) std::swap(array[l], array[r]);

      int  p = r--;
      while (true) {
        while (array[l] < array[p]) l++;
        while (array[r] > array[p]) r--;
        if (l > r) break;
        std::swap(array[l], array[r]);
      }

      std::swap(array[l], array[p]);
      if (l == index) break;
      else if (index < l) r = high = l - 1, l = low; 
      else if (index > l) l = low = l + 1, r = high;
    }

    return array[index];
  };

  template<int N, int M, typename T>
  T matrixMedian(QGenericMatrix<N, M, T> matrix) {
    const int S = N * M, m = S >> 1, n = m - 1, e = S - 1;
    T array[S];
    matrix.copyDataTo(array);
    const T mv = kthSmallest(array, m, 0, e);
    if (S % 2) return mv;
    const T nv = kthSmallest(array, n, 0, n);
    return (mv + nv) / 2;
  }

  quint64 computePhash(const QImage &image) {
    const int N = 32, M = 8;
    // step 1. convert to grayscale
    QImage img = image.convertToFormat(QImage::Format_Grayscale8);
    // step 2. apply box blur
    img = blur(img);
    // step 3. resize to 32x32
    img = img.scaled(N, N, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_Grayscale8);
    // step 4. convert image to matrix
    auto imgMat = qimg2matrix<N, N, float>(img);
    // step 5. apply dct
    auto dctMat = dctMatrix<N, float>();
    auto freqTable = dctMat * imgMat * dctMat.transposed();
    // step 6. fetch lower frequencies and compose the hash
    auto lowerFreqs = matrixSubset<N, M, float>(freqTable, 1, 1);
    auto median = matrixMedian(lowerFreqs);
    long hash = 0;
    long flag = 1;

    for (int row = 0; row < M; row++) {
      for (int col = 0; col < M; col++) {
        if (lowerFreqs(row, col) > median) hash |= flag;
        flag = flag << 1;
      }
    }

    return hash;
  }

  int computeDistance(const quint64 hash1,const quint64 hash2){
    quint64 x = hash1^hash2;
    const quint64 m1  = 0x5555555555555555ULL;
    const quint64 m2  = 0x3333333333333333ULL;
    const quint64 h01 = 0x0101010101010101ULL;
    const quint64 m4  = 0x0f0f0f0f0f0f0f0fULL;
    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    return (x * h01)>>56;
  }

}
#endif
