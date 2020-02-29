#include "QtPhash2.h"

using namespace QtPhash;


QString run_combination(const BlurMethod blur, const SplitMethod split, float (&result)[4]) {
    QString blurName = "none";
    if (blur == Box)
        blurName = "box";
    else if (blur == Gaussian)
        blurName = "gaussian";

    QString splitName = "median";
    if (split == Mean)
        splitName = "mean";

    QString name = QString("%1-%2").arg(blurName).arg(splitName);

    QFile csvFile(QString("/home/klesh/Desktop/imagesets/results/result-%1.csv").arg(name));
    csvFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&csvFile);
    QDir rootDir("/home/klesh/Desktop/imagesets");
    QDir samplesDir = rootDir.filePath("samples");
    QDir testsetsDir = rootDir.filePath("testsets");

    QStringList samples = samplesDir.entryList(QDir::Filter::Files);
    QStringList testsets = testsetsDir.entryList(QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot);

    // output headers
    out << "similar" << endl;
    out << "image";
    for (const auto &testset: testsets) {
        out << "," << testset;
    }
    out << endl;

    float total = 0, squareTotal = 0;
    int count = 0, i = 0;
    int ss = samples.size();
    quint64 phashes[ss];
    // dist between similar images
    for (const auto &sample: samples) {
        auto samplePHash = computePhash(QImage(samplesDir.absoluteFilePath(sample)), blur, split);
        phashes[i] = samplePHash;
        out << sample;
        for (const auto &testset: testsets) {
            QDir testsetDir = testsetsDir.filePath(testset);
            auto testsetPHash = computePhash(QImage(testsetDir.absoluteFilePath(sample)), blur, split);
            auto dist = computeDistance(samplePHash, testsetPHash);
            out << "," << dist;
            total += dist;
            squareTotal += dist * dist;
            count++;
        }
        out << endl;
    }
    float mean = total / count;
    float var = squareTotal / count - mean * mean;
    out << "MEAN," << mean << endl;
    out << "VAR," << var << endl;
    result[0] = mean;
    result[1] = var;

    out << endl << endl;
    out << "dissimilar" << endl;

    // dist between dissimilar images
    total = 0, squareTotal = 0, count = 0;
    for (int i = 0; i < ss; i++) {
        for (int j = 0; j < i; j++) {
            auto dist = computeDistance(phashes[i], phashes[j]);
            out << samples[i] << "," << samples[j] << "," << dist << endl;
            total += dist;
            squareTotal += dist * dist;
            count++;
        }
    }
    mean = total / count;
    var = squareTotal / count - mean * mean;
    out << "MEAN," << mean << endl;
    out << "VAR," << var << endl;
    result[2] = mean;
    result[3] = var;
    csvFile.close();

    return name;
}


int main() {
    QFile csvFile(QString("/home/klesh/Desktop/imagesets/results/result.csv"));
    csvFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&csvFile);

    out << "combination, similar-mean, similar-var, dissimlar-mean, dissimlar-var" << endl;

    BlurMethod blurs[] = { None, Box, Gaussian };
    SplitMethod splits[] = { Mean, Median };
    float result[4];
    for (const auto &blur : blurs) {
        for (const auto &split : splits) {
            auto name = run_combination(blur, split, result);
            out << name << "," << result[0] << "," << result[1] << "," << result[2] << "," << result[3] << "," << endl;
        }
    }

    csvFile.close();
}
