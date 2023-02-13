/**
 * File: Localization.cpp
 * Date: February
 * Author: Noé Guadalupe Aldana Murillo
 * Description: Visual localization using a bag of visual words
 * License: see the LICENSE.txt file
 *
 */

#ifndef __D_T_LOCALIZATION__
#define __D_T_LOCALIZATION__

#include <vector>

// DBoW2
#include "DBoW2.h" // defines OrbVocabulary and OrbDatabase

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>


using namespace DBoW2;
using namespace std;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void ChangeStructure(const cv::Mat &plain, vector<cv::Mat> &out);
void VocabularyCreation(const vector<vector<cv::Mat > > &features,  int numImages, std::string vocabularyName, 
                        const int k = 10, const int L = 5, const WeightingType weight = TF_IDF,const ScoringType scoring = L1_NORM);
unsigned int LoadFeaturesFromVideo(vector<vector<cv::Mat > > &features, std::string fileName);
void Localization(std::string databaseName, std::string videoName);
void VisualMemoryToBoW(const vector<vector<cv::Mat > > &features, std::string vocabularyName,
                        int numImages,std::string databaseName);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#endif
