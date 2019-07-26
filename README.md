# Introduction

pHash implementation on Qt with no other dependency.

# Usage
Include `QtPhash.h` and you are golden.
```
#include "QtPhash.h"
#include <stdio.h>

int main() {
  QImage image1("/path/to/your/photo.jpg");
  QImage image2("/path/to/your/photo.jpg");
  ulong hash1 = QtPhash::computePhash(image1);
  ulong hash2 = QtPhash::computePhash(image2);
  int distance = QtPhash::computeDistance(hash1, hash2);
  printf("hash1: %x hash2: %x  distance: %d", hash1, hash2, distance);
  return 0;
}
```

# Test Results
`img_test.cpp` contains all testing logic, check it out if you want to test for yourself.

`misc` vs `rotd`
```
"name                 misc(qt)         blur(qt)           dist(qt) misc             blur               dist(ph)"
"architecture1.bmp    10350cf934cfcd76 10350cf934cfcd76          0 50358c591cef4d76 50318c791cef4d76          2"
"architecture_2.bmp    cf203ff30c3df30  cf203ff30c3df30          0 4cb2432fb443cfb4 4cb2432fb443df34          2"
"bamarket115.bmp      f17ce1a5f1030379 f17ce1a5f1030379          0 b43ce1f4f0c18379 b53ce1f0f0c18379          2"
"butterflywallpaper.b 9a31def661d61129 9a31def661d61129          0 fc11ce7ea1d601a9 fe11ce7e21d601a9          2"
"Chang_PermanentMidni c0cd4db392ccb173 c0cd4db392ccb173          0 c0c96d9b92ccb173 c0cc6d9b92ccb173          2"
"damien_hirst.bmp     f2618f1e0668fe83 f2618f1e0668fe83          0 b271c38e0e34fec2 b271c38e0e3cbec2          2"
"damien_hirst_does_fa 738c53719ccb681e 738c53719ccb681e          0 334cd7719ccf280e 33ccd7719cc7280e          2"
"damien_hirst_virgin_ 170b199a4dcef0ad  74b199a4dcef0ad          2 7763099a4dcab0ad 5763099acdcab0ad          2"
"damienhirst.bmp      7cd20b4bba2d5691 7cd20b4bba2d5691          0 6cd2884ffaac56d0 6cd2884ffaac56d0          0"
"dhirst_a3b9ddea.bmp  bdb57036d00b02df bdf57034d00b02df          2 ddcd7c3cf0a50254 ddcd7c3cd0a5025c          2"
"diamondskull.bmp     e341c31e4e6daed0 e341c31e4e6daed0          0 f341c32e4f2f2ec0 f341c32e4f2f2ec0          0"
"doodle.bmp           1d6d5a716e846c39 1d6d5a717e846c19          2 1b8d4b712ec52c3d 1b8d4b713ec52c1d          2"
"england.bmp          a6606f99d22e91b3 a6606f99d22e91b3          0 46206f9df22ed493 46206f9df22ed493          0"
"englandpath.bmp      b39aac2e3e4a50f4 b39aac2e3e4a50f4          0 b1988c2e3e2adaf4 b1998c2e3e2ad8f4          2"
"Hhirst_BGE.bmp       f04fa16b41adc656 f04fa16b41adc656          0 f04da1ab41bdc656 f04da1ab41bdc656          0"
"jasper_johns.bmp     117ba54ef9135ac4 117ba54ef9135ac4          0 91dbbd0eb8c35a44 91dbbd0eb8c35a44          0"
"johns_portrait_380x3 39397b632489c736 39397b632489c736          0 1b991b633489e736 19991b633589e736          2"
"latrobe.bmp          da8d5978e5788507 da8d5978e5788507          0 d8cd8d7de4789103 d8cd8d7de4789103          0"
"Scotland_castle_wedd 31963b4bec5c05b3 31963b4bec5c05b3          0 b9969b4ba44c45b3 b9969b4ba44c45b3          0"
"targetjasperjohns.bm 31c6f2180ff12bb9 31c6f2180ff12bb9          0 3164f2980f532fb9 3164f2904ed32fb9          4"
"Tower-Bridge-at-nigh 63849f797cc96470 6384db797cc96470          2 76c4ce79ecc92470 77c4ce39ecc92470          2"
"uk-golf-scotland.bmp 8296af2b7970509f 8296af2b7970509f          0 d2d22f0bf970608f d2d22f0bf970608f          0"
"wallacestevens.bmp   67787023a77232cb 67787023a77232cb          0  77d3423a771b24b 477d3423a770b24b          2"

Distance between different photos
"image1               image2                   dist(qt)     dist(ph)"
"architecture1.bmp    architecture_2.bmp             22            8"
"architecture1.bmp    bamarket115.bmp                34           26"
"architecture1.bmp    butterflywallpaper.b           30           28"
"architecture1.bmp    Chang_PermanentMidni           26           32"
"architecture1.bmp    damien_hirst.bmp               34           28"
"architecture1.bmp    damien_hirst_does_fa           28           36"
"architecture1.bmp    damien_hirst_virgin_           32           28"
"architecture1.bmp    damienhirst.bmp                36           34"
"architecture1.bmp    dhirst_a3b9ddea.bmp            34           36"
"architecture1.bmp    diamondskull.bmp               38           32"
"architecture1.bmp    doodle.bmp                     28           40"
"architecture1.bmp    england.bmp                    32           26"
"architecture1.bmp    englandpath.bmp                30           30"
"architecture1.bmp    Hhirst_BGE.bmp                 28           28"
"architecture1.bmp    jasper_johns.bmp               34           28"
"architecture1.bmp    johns_portrait_380x3           22           32"
"architecture1.bmp    latrobe.bmp                    30           22"
"architecture1.bmp    Scotland_castle_wedd           30           30"
"architecture1.bmp    targetjasperjohns.bm           40           28"
"architecture1.bmp    Tower-Bridge-at-nigh           24           34"
"architecture1.bmp    uk-golf-scotland.bmp           36           26"
"architecture1.bmp    wallacestevens.bmp             44           40"
```

`rotd` vs `blur`
```
"name                 rotd(qt)         blur(qt)           dist(qt) rotd             blur               dist(ph)"
"architecture1.bmp    183d8c7934cf4d66 10350cf934cfcd76          6  8b5847d1ce74d76 50318c791cef4d76          8"
"architecture_2.bmp   4df20bfc30cbde20  cf203ff30c3df30          8  cf243ef30cbcf30 4cb2432fb443df34         10"
"bamarket115.bmp      a17ce1b4f1c30379 f17ce1a5f1030379          6 b43c71f4f0c18379 b53ce1f0f0c18379          4"
"butterflywallpaper.b 9831fef261d69129 9a31def661d61129          4 9c19fe7621c691a9 fe11ce7e21d601a9         10"
"Chang_PermanentMidni 80cdcf3392cc3373 c0cd4db392ccb173          6 c0cd4d9392ccb573 c0cc6d9b92ccb173          4"
"damien_hirst.bmp     e2618f9e0ea0fe46 f2618f1e0668fe83         10 d261c7ce0e34fe42 b271c38e0e3cbec2          8"
"damien_hirst_does_fa 738c51719ccf681e 738c53719ccb681e          2 334cd5719ccf281e 33ccd7719cc7280e          4"
"damien_hirst_virgin_ 170f199acd4ef02d  74b199a4dcef0ad          6 172f099acccef08d 5763099acdcab0ad          8"
"damienhirst.bmp      6ed20b4bb2ac56d1 7cd20b4bba2d5691          6 6cd2896fbaac52d0 6cd2884ffaac56d0          4"
"dhirst_a3b9ddea.bmp  bdf57016900b02ff bdf57034d00b02df          4 9cbd703cd0a502df ddcd7c3cd0a5025c         10"
"diamondskull.bmp     e341cb2e0fa5add0 e341c31e4e6daed0         10 f341c3ae0f35adc0 f341c32e4f2f2ec0          8"
"doodle.bmp           1c6c5a716e846cf9 1d6d5a717e846c19          6  fec4b702fc42c5d 1b8d4b713ec52c1d         10"
"england.bmp          26606f99d2ae91b3 a6606f99d22e91b3          2 66206f9dd22ed493 46206f9df22ed493          2"
"englandpath.bmp      b39aac2e3a5a50f4 b39aac2e3e4a50f4          2 b19a8cae2e2ad8f4 b1998c2e3e2ad8f4          4"
"Hhirst_BGE.bmp       f04fa12f41adc656 f04fa16b41adc656          2 f04da1bb419dc656 f04da1ab41bdc656          2"
"jasper_johns.bmp     857fa44ef9115ac4 117ba54ef9135ac4          6 115fb50ebb535a44 91dbbd0eb8c35a44          8"
"johns_portrait_380x3 39397b632489c676 39397b632489c736          2 19b93b633489c736 19991b633589e736          4"
"latrobe.bmp          dccd5978e5f80107 da8d5978e5788507          6 cccd0d7da4f89113 d8cd8d7de4789103          6"
"Scotland_castle_wedd 3b961b4bec5805b3 31963b4bec5c05b3          4 3b969a4be44c45b3 b9969b4ba44c45b3          4"
"targetjasperjohns.bm 31c6d2180ff12fb9 31c6f2180ff12bb9          2 3164d2380ff12fb9 3164f2904ed32fb9          8"
"Tower-Bridge-at-nigh 629c9e79ecc96470 6384db797cc96470          8 7fc48e79a4c92472 77c4ce39ecc92470          6"
"uk-golf-scotland.bmp 82963f29f960d09f 8296af2b7970509f          6 92d25f0fb9f0401f d2d22f0bf970608f         10"
"wallacestevens.bmp   6f707403a77222db 67787023a77232cb          6 677d3023a77032cb 477d3423a770b24b          4"

Distance between different photos
"image1               image2                   dist(qt)     dist(ph)"
"architecture1.bmp    architecture_2.bmp             26           10"
"architecture1.bmp    bamarket115.bmp                32           26"
"architecture1.bmp    butterflywallpaper.b           28           32"
"architecture1.bmp    Chang_PermanentMidni           28           28"
"architecture1.bmp    damien_hirst.bmp               34           30"
"architecture1.bmp    damien_hirst_does_fa           26           36"
"architecture1.bmp    damien_hirst_virgin_           34           24"
"architecture1.bmp    damienhirst.bmp                36           32"
"architecture1.bmp    dhirst_a3b9ddea.bmp            34           34"
"architecture1.bmp    diamondskull.bmp               38           30"
"architecture1.bmp    doodle.bmp                     26           40"
"architecture1.bmp    england.bmp                    36           30"
"architecture1.bmp    englandpath.bmp                30           38"
"architecture1.bmp    Hhirst_BGE.bmp                 30           30"
"architecture1.bmp    jasper_johns.bmp               32           28"
"architecture1.bmp    johns_portrait_380x3           22           30"
"architecture1.bmp    latrobe.bmp                    28           20"
"architecture1.bmp    Scotland_castle_wedd           32           28"
"architecture1.bmp    targetjasperjohns.bm           38           26"
"architecture1.bmp    Tower-Bridge-at-nigh           22           34"
"architecture1.bmp    uk-golf-scotland.bmp           38           22"
"architecture1.bmp    wallacestevens.bmp             42           38"
```