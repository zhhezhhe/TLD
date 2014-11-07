//
//  main.cpp
//  TLD
//
//  Created by 陈裕昕 on 11/4/14.
//  Copyright (c) 2014 Fudan. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "ViewController.h"
#include "VideoController.h"
#include "RandomFernsClassifier.h"
#include "NNClassifier.h"
#include "Detector.h"
#include "TLD.h"

using namespace std;
using namespace cv;

Rect readRect()
{
    int width, height, x, y;
    scanf("[%d x %d from (%d, %d)]\n", &width, &height, &x, &y);
    
    return Rect(x, y, width, height);
}

void testRandomFernsClassifier()
{
    string filename("/Users/Orthocenter/Developments/MedianFlow/car.mpg");
    VideoController videoController(filename);
    ViewController viewController(&videoController);
    
    videoController.readNextFrame();
    
    RandomFernsClassifier randomFernsClassifier(10, 13);
    
    RandomFernsClassifier::tTrainDataSet trainDataSet;
    
    cerr << "Draw 20 positive bounding boxs" << endl;
    
    for(int i = 0; i < 20; i++)
    {
        cerr << "No. " << i << endl;
        
        //Rect rect = viewController.getRect();
        Rect rect = readRect();
        
        cout << rect << endl;
        
        RandomFernsClassifier::tTrainData trainData(make_pair(videoController.getCurrFrame()(rect), randomFernsClassifier.cPos));
        
        trainDataSet.push_back(trainData);
    }
    
    cerr << "Draw 20 negative bounding boxs" << endl;
    
    for(int i = 0; i < 20; i++)
    {
        cerr << "No. " << i << endl;
        
        //Rect rect = viewController.getRect();
        Rect rect = readRect();
        
        cout << rect << endl;
        
        RandomFernsClassifier::tTrainData trainData(make_pair(videoController.getCurrFrame()(rect), randomFernsClassifier.cNeg));
        
        trainDataSet.push_back(trainData);
    }

    cerr << "training..." << endl;
    randomFernsClassifier.train(trainDataSet);
    cerr << "finished..." << endl;
    
    for(;;)
    {
        Rect rect = viewController.getRect();
        
        cout << randomFernsClassifier.getClass(videoController.getCurrFrame()(rect)) << endl;
    }
}

void testNNClassifier()
{
    string filename("/Users/Orthocenter/Developments/MedianFlow/car.mpg");
    VideoController videoController(filename);
    ViewController viewController(&videoController);
    
    videoController.readNextFrame();
    
    NNClassifier nNClassifier;
    
    NNClassifier::tTrainDataSet trainDataSet;
    
    cerr << "Draw 20 positive bounding boxs" << endl;
    
    for(int i = 0; i < 20; i++)
    {
        cerr << "No. " << i << endl;
        
        //Rect rect = viewController.getRect();
        Rect rect = readRect();
        
        cout << rect << endl;
        
        NNClassifier::tTrainData trainData(make_pair(videoController.getCurrFrame()(rect), nNClassifier.cPos));
        
        trainDataSet.push_back(trainData);
    }
    
    cerr << "Draw 20 negative bounding boxs" << endl;
    
    for(int i = 0; i < 20; i++)
    {
        cerr << "No. " << i << endl;
        
        //Rect rect = viewController.getRect();
        Rect rect = readRect();
        
        cout << rect << endl;
        
        NNClassifier::tTrainData trainData(make_pair(videoController.getCurrFrame()(rect), nNClassifier.cNeg));
        
        trainDataSet.push_back(trainData);
    }
    
    cerr << "training..." << endl;
    nNClassifier.train(trainDataSet);
    cerr << "finished..." << endl;
    
    //nNClassifier.showModel();
    
    for(;;)
    {
        Rect rect = viewController.getRect();
        
        cout << nNClassifier.getClass(videoController.getCurrFrame()(rect)) << endl;
    }

}


void testRFNNClassifier()
{
    string filename("/Users/Orthocenter/Developments/MedianFlow/car.mpg");
    VideoController videoController(filename);
    ViewController viewController(&videoController);
    
    videoController.readNextFrame();
    
    RandomFernsClassifier rFClassifier(10, 5);
    
    RandomFernsClassifier::tTrainDataSet rFTrainDataSet;
    
    NNClassifier nNClassifier;
    
    NNClassifier::tTrainDataSet trainDataSet;
    
    cerr << "Draw 20 positive bounding boxs" << endl;
    
    for(int i = 0; i < 20; i++)
    {
        cerr << "No. " << i << endl;
        
        //Rect rect = viewController.getRect();
        Rect rect = readRect();
        
        cout << rect << endl;
        
        RandomFernsClassifier::tTrainData rFTrainData(make_pair(videoController.getCurrFrame()(rect), rFClassifier.cPos));
        NNClassifier::tTrainData trainData(make_pair(videoController.getCurrFrame()(rect), nNClassifier.cPos));
        
        rFTrainDataSet.push_back(rFTrainData);
        trainDataSet.push_back(trainData);
    }
    
    cerr << "Draw 20 negative bounding boxs" << endl;
    
    for(int i = 0; i < 20; i++)
    {
        cerr << "No. " << i << endl;
        
        //Rect rect = viewController.getRect();
        Rect rect = readRect();
        
        cout << rect << endl;
        
        RandomFernsClassifier::tTrainData rFTrainData(make_pair(videoController.getCurrFrame()(rect), rFClassifier.cNeg));

        NNClassifier::tTrainData trainData(make_pair(videoController.getCurrFrame()(rect), nNClassifier.cNeg));
        
        rFTrainDataSet.push_back(rFTrainData);
        trainDataSet.push_back(trainData);
    }
    
    cerr << "training..." << endl;
    nNClassifier.train(trainDataSet);
    rFClassifier.train(rFTrainDataSet);
    cerr << "finished..." << endl;
    
    //nNClassifier.showModel();
    
    for(;;)
    {
        Rect rect = viewController.getRect();
        
        cout << "RF: " << rFClassifier.getClass(videoController.getCurrFrame()(rect)) << endl;
        cout << "NN: " << nNClassifier.getClass(videoController.getCurrFrame()(rect)) << endl;
    }
    
}

void testDetector()
{
    string filename("/Users/Orthocenter/Developments/MedianFlow/scale.m4v");
    VideoController videoController(filename);
    ViewController viewController(&videoController);
    
    videoController.readNextFrame();
    
    Rect rect = viewController.getRect();
    
    Detector detector(videoController.getCurrFrame(), rect);
    
    while(videoController.readNextFrame())
    {
        //viewController.refreshCache();
        //viewController.showCache();
        //waitKey(1);
    
        Detector::tRet ret;
        detector.dectect(videoController.getCurrFrame(), ret);
    
        viewController.refreshCache();
        for(auto bb : ret)
        {
            viewController.drawRect(bb);
        }
        viewController.showCache();
        waitKey(1);
    }
}

void testVarClassifier()
{
    Mat test = (Mat_<uchar>(3, 3) << 0, 1, 2, 3, 4, 1, 2, 3, 4);
    cout << test << endl;
    VarClassifier var(test);
    Rect rect(0, 1, 2, 2);
    cout << var.getVar(rect);
    
    Mat test2(test(rect));
    cout << test2 << endl;
    Scalar mean, std;
    meanStdDev(test2, mean, std);
    
    cout << " " << std.val[0] * std.val[0] << endl;
}

void testTLD()
{
    string filename("/Users/Orthocenter/Developments/MedianFlow/car.mpg");
    VideoController videoController(filename);
    ViewController viewController(&videoController);
    
    videoController.readNextFrame();

    Rect rect = viewController.getRect();
    
    TLD tld(videoController.getCurrFrame(), rect);
    
    while(videoController.readNextFrame())
    {
        tld.setNextFrame(videoController.getCurrFrame());
        
        tld.track();
        
        viewController.refreshCache();
        viewController.drawRect(tld.getBB());
        viewController.showCache();
        waitKey(1);
    }
}

int main()
{
    freopen("data.txt", "r", stdin);
    //testRandomFernsClassifier();
    //testNNClassifier();
    //testRFNNClassifier();
    //testDetector();
    //testVarClassifier();
    testTLD();
    return 0;
}