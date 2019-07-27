#include "img.h"
#include <QtCore>
#include <QtDebug>

// Image Sets can be downloaded from http://www.phash.org/download

const static QDir imageSetsFolder("/home/klesh/Desktop/imagesets");
const static QString qimgPath = imageSetsFolder.filePath("blur/architecture1.bmp");
const static QImage qimg(qimgPath);

void try_qimg2cimg() {
  CImg<uchar> cimg = qimg2cimg<uchar>(qimg);
  cimg.save(imageSetsFolder.filePath("qimg2cimg.bmp").toLatin1().constData());
}

void try_cimg2qimg() {
  CImg<uchar> cimg(qimgPath.toLatin1().constData());
  QImage qimg = cimg2qimg(cimg);
  qimg.save(imageSetsFolder.filePath("cimg2qimg.bmp"));
}

void try_qimg2matrix() {
  auto qimg2 = qimg.scaled(32, 32).convertToFormat(QImage::Format_Grayscale8);
  auto matrix = QtPhash::qimg2matrix<32, 32, uchar>(qimg);

  auto qimg3 = QtPhash::matrix2qimg<32, 32, uchar>(matrix);
  qimg3.save(imageSetsFolder.filePath("qimg2matrix.bmp"));
}

void try_qimg2blur() {
  auto qimg2 = QtPhash::blur(qimg.convertToFormat(QImage::Format_Grayscale8));
  qimg2.save(imageSetsFolder.filePath("qimg2blur.bmp"));
}

void compare_dct_matrices() {
  const int N = 32;
  auto qimgMatrix = QtPhash::dctMatrix<N, float>();
  qDebug() << "qimg dct matrix";
  QtPhash::printMatrix(qimgMatrix);

  auto cimgMatrix = ph_dct_matrix(N);
  qDebug() << "cimg dct matrix";
  cimgPrintMatrix(*cimgMatrix);
}

void test_qimgMatrixSubset() {
  float values[] = {
    1, 2, 3, 4,
    1, 2, 3, 4,
    2, 4, 1, 3,
    1, 2, 3, 4,
  };
  QGenericMatrix<4, 4, float> matrix(values);
  auto subset = QtPhash::matrixSubset<4, 2, float>(matrix, 1, 1);
  // qimgPrintMatrix(subset);
  assert(subset(0, 0) == 2);
  assert(subset(0, 1) == 3);
  assert(subset(1, 0) == 4);
  assert(subset(1, 1) == 1);
}

void test_qimgMatrixMedian() {
  float values[] = {
    4, 8, 6, 1, 3, 2
  };
  QGenericMatrix<2, 3, float> matrix(values);
  assert(QtPhash::matrixMedian(matrix) == 3.5);

  float values2[] = {
    1, 2, 3
  };
  QGenericMatrix<1, 3, float> matrix2(values2);
  assert(QtPhash::matrixMedian(matrix2) == 2);

  float values3[] = {
    1, 2, 3, 4
  };
  QGenericMatrix<1, 4, float> matrix3(values3);
  assert(QtPhash::matrixMedian(matrix3) == 2.5);

  float values4[] = {
    4, 3, 2, 1
  };
  QGenericMatrix<1, 4, float> matrix4(values4);
  assert(QtPhash::matrixMedian(matrix4) == 2.5);
}

void try_qimgPhash() {
  QString folderName1 = "rotd", folderName2 = "blur";
  QDir folder1 = imageSetsFolder.filePath(folderName1), folder2 = imageSetsFolder.filePath(folderName2);

  QStringList names;
  QList<quint64> hashes, hashes2;

  QStringList headBuilder;
  headBuilder.append(QString("name").leftJustified(20));
  headBuilder.append(QString("%1(qt)").arg(folderName1).leftJustified(16));
  headBuilder.append(QString("%1(qt)").arg(folderName2).leftJustified(16));
  headBuilder.append(QString("dist(qt)").rightJustified(10));
  headBuilder.append(folderName1.leftJustified(16));
  headBuilder.append(folderName2.leftJustified(16));
  headBuilder.append(QString("dist(ph)").rightJustified(10));
  qDebug() << headBuilder.join(" ");

  for (const auto &name : folder1.entryList({"*.bmp"})) {
    // if (name != "Scotland_castle_wedding.bmp") continue;
    QString img1Path = folder1.filePath(name), img2Path = folder2.filePath(name);
    QImage img1(img1Path), img2(img2Path);
    const quint64 hash1 = QtPhash::computePhash(img1), hash2 = QtPhash::computePhash(img2);
    const quint64 hash3 = ph_dct_imagehash(img1Path.toLatin1().constData()), hash4 = ph_dct_imagehash(img2Path.toLatin1().constData());

    QStringList lineBuilder;
    lineBuilder.append(name.mid(0, 20).leftJustified(20));
    lineBuilder.append(QString::number(hash1, 16).rightJustified(16));
    lineBuilder.append(QString::number(hash2, 16).rightJustified(16));
    lineBuilder.append(QString::number(QtPhash::computeDistance(hash1, hash2)).rightJustified(10));
    lineBuilder.append(QString::number(hash3, 16).rightJustified(16));
    lineBuilder.append(QString::number(hash4, 16).rightJustified(16));
    lineBuilder.append(QString::number(QtPhash::computeDistance(hash3, hash4)).rightJustified(10));
    qDebug() << lineBuilder.join(" ");

    names.append(name);
    hashes.append(hash1);
    hashes2.append(hash3);
  }

  qDebug() << "";
  qDebug() << "Distance between different photos";

  QStringList headBuilder2;
  headBuilder2.append(QString("image1").leftJustified(20));
  headBuilder2.append(QString("image2").leftJustified(20));
  headBuilder2.append(QString("dist(qt)").rightJustified(12));
  headBuilder2.append(QString("dist(ph)").rightJustified(12));
  qDebug() << headBuilder2.join(" ");
  QString onesName = names.takeFirst().mid(0, 20).leftJustified(20);
  quint64 onesHash = hashes.takeFirst();
  while (!names.isEmpty()) {
    QStringList lineBuilder;
    lineBuilder.append(onesName);
    lineBuilder.append(names.takeFirst().mid(0, 20).leftJustified(20));
    lineBuilder.append(QString::number(QtPhash::computeDistance(onesHash, hashes.takeFirst())).rightJustified(12));
    lineBuilder.append(QString::number(QtPhash::computeDistance(onesHash, hashes2.takeFirst())).rightJustified(12));
    qDebug() << lineBuilder.join(" ");
  }
}

int main() {
  // try_qimg2cimg();
  // try_cimg2qimg();
  // try_qimg2matrix();
  // try_qimg2blur();
  // compare_dct_matrices();
  // test_qimgMatrixSubset();
  // test_qimgMatrixMedian();
  try_qimgPhash();
  return 0;
}
