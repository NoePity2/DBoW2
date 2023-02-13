/**
 * File: Localization.cpp
 * Date: January 2023
 * Author: Noé GUadalupe Aldana Murillo
 * Description: Localization in a visual memory using DBoW2
 * License: see the LICENSE.txt file
 */

#include <iostream>
#include <vector>

// DBoW2
#include "DBoW2.h" // defines OrbVocabulary and OrbDatabase

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>


#include "Localization.h"


using namespace DBoW2;
using namespace std;



int main()
{
  vector<vector<cv::Mat > > features;

  std::string videoName = "videos/video_01.mp4";

  int numImages =  LoadFeaturesFromVideo(features,videoName);

  std::string vocabularyName = "voc.yml.gz";

  std::string databaseName = "db.yml.gz";
  VisualMemoryToBoW(features, vocabularyName, numImages,databaseName);

  return 0;
}

