//
//  Detector.h
//  TLD
//
//  Created by 陈裕昕 on 11/5/14.
//  Copyright (c) 2014 Fudan. All rights reserved.
//

#ifndef __TLD__Detector__
#define __TLD__Detector__

#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>

#include "VarClassifier.h"
#include "RandomFernsClassifier.h"
#include "NNClassifier.h"
#include "Learner.h"

using namespace std;
using namespace cv;

class Detector
{
    friend class Learner;
public:
    struct tScanBB
    {
        Rect first;  //bb
        float second;//overlap
        char status;
        
        tScanBB(const Rect &_bb):first(_bb), second(-1), status(0){};
        tScanBB(const Rect &_bb, const float _ol):first(_bb), second(_ol), status(0){};
    };
    
    typedef vector<Rect> tRet;
    typedef vector<tScanBB> tScanBBs;
    typedef pair<Mat, bool> tTrainData;
    typedef vector<tTrainData> tTrainDataSet;
    
    static const int minBBSize = 20;
    constexpr static const float warpNoiseRange = 5;
    constexpr static const float warpRandomBlur = true;
    constexpr static const float warpScale = 0.1;
    constexpr static const float warpAngle = 10. / 180 * CV_PI;
    const bool cPos = true;
    const bool cNeg = false;
    constexpr static const float thGoodBB = 0.6;
    constexpr static const float thBadBB = 0.2;
    static const int thPosData = 10;
    
    static const char bbAC = 1;
    static const char bbRejVar = 2;
    static const char bbRejRF = 3;
    static const char bbRejNN = 4;
    
private:
    RandomFernsClassifier rFClassifier;
    NNClassifier nNClassifier;
    
    Mat img;
    Mat pattern;
    Rect patternBB;
    int imgW, imgH;
    float patternVar;
    PatchGenerator patchGenerator;
    tTrainDataSet trainDataSet;
    
    // return true if p1.x < p2.x and p1.y < p2.y
    float overlap(const Rect &bb1, const Rect &bb2);
    
    // scanning-window grid
    tScanBBs scanBBs;
    void genScanBB();
    
    void sortByOverlap(const Rect &bb, bool rand = false);
    
    void genWarped(const Mat &img, Mat &warped);
    void genPosData(const Mat &img, tTrainDataSet &trainDataSet);
    void genNegData(const Mat &img, tTrainDataSet &trainDataSet);
    
    void update();
    void train(const Mat &img, const Rect &patternBB);
    
public:
    Detector();
    Detector(const Mat &img, const Rect &patternBB);
    
    static bool scanBBCmp(const tScanBB &a, const tScanBB &b)
    {
        return a.second > b.second;
    }
    
    void dectect(const Mat &img, tRet &ret);
    
    float calcSc(const Mat &img);
    
    ~Detector();
};

#endif /* defined(__TLD__Detector__) */
