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


using namespace DBoW2;
using namespace std;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void ChangeStructure(const cv::Mat &plain, vector<cv::Mat> &out);
void VocabularyCreation(const vector<vector<cv::Mat > > &features,  int numImages, std::string vocabularyName, 
                        const int k = 6, const int L = 4, const WeightingType weight = TF_IDF,const ScoringType scoring = L1_NORM);
unsigned int LoadFeaturesFromVideo(vector<vector<cv::Mat > > &features, std::string fileName);
void Localization(std::string databaseName, std::string videoName);
void VisualMemoryToBoW(const vector<vector<cv::Mat > > &features, std::string vocabularyName,
                        int numImages,std::string databaseName);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void wait()
{
  cout << endl << "Press enter to continue" << endl;
  getchar();
}

// ----------------------------------------------------------------------------

int main()
{
  //vector<vector<cv::Mat > > features;

  std::string videoName = "videos/video_03.mp4";

  //int numImages =  LoadFeaturesFromVideo(features,videoName);

  //std::string vocabularyName = "voc.yml.gz";
  //VocabularyCreation(features,numImages,vocabularyName);

  // std::string databaseName = "db.yml.gz";
  // VisualMemoryToBoW(features, vocabularyName, numImages,databaseName);

  std::string databaseName = "db.yml.gz";
  Localization(databaseName, videoName);

  return 0;
}

// ----------------------------------------------------------------------------

unsigned int LoadFeaturesFromVideo(vector<vector<cv::Mat > > &features, std::string fileName)
{
    cv::VideoCapture capture(fileName);
    cv::Mat frame;

    features.clear();

    if( !capture.isOpened() )
        throw "Error when reading steam_avi";

    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    cout << "Extracting ORB features..." << endl;

    unsigned int i = 1;
    for( ; ; )
    {
        capture >> frame;
        if(frame.empty())
            break;

        cv::Mat mask;
        vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;

        orb->detectAndCompute(frame, mask, keypoints, descriptors);

        features.push_back(vector<cv::Mat >());
        ChangeStructure(descriptors, features.back());

        i++;
    }
    cout << "Number of images: " << i << endl;

    return i;
}



// ----------------------------------------------------------------------------

void ChangeStructure(const cv::Mat &plain, vector<cv::Mat> &out)
{
  out.resize(plain.rows);

  for(int i = 0; i < plain.rows; ++i)
  {
    out[i] = plain.row(i);
  }
}

// ----------------------------------------------------------------------------

void VocabularyCreation(const vector<vector<cv::Mat > > &features,  int numImages, std::string vocabularyName, 
                        const int k, const int L, const WeightingType weight,const ScoringType scoring)
{
    // branching factor and depth levels 

    OrbVocabulary voc(k, L, weight, scoring);

    cout << "Creating a  " << k << "^" << L << " vocabulary..." << endl;
    voc.create(features);
    cout << "... done!" << endl;

    cout << "Vocabulary information: " << endl
    << voc << endl << endl;

    OrbDatabase db(voc, false, 0); 


    cout << "Number of images: " << numImages << endl;

    // save the vocabulary to disk
    cout << endl << "Saving vocabulary..." << endl;
    voc.save(vocabularyName);
    cout << "Done" << endl;
}

//-----------------------------------------------------------------------------
void VisualMemoryToBoW(const vector<vector<cv::Mat > > &features, std::string vocabularyName,
                        int numImages,std::string databaseName){

  // load the vocabulary from disk
  OrbVocabulary voc(vocabularyName);

  OrbDatabase db(voc, false, 0); // false = do not use direct index

    // add images to the database
  for(int i = 0; i < numImages; i++)
  {
    db.add(features[i]);
  }

  cout << "... done!" << endl;

  cout << "Database information: " << endl << db << endl;

  // we can save the database. The created file includes the vocabulary
  // and the entries added
  cout << "Saving database..." << endl;
  db.save(databaseName);
  cout << "... done!" << endl;

}


// ----------------------------------------------------------------------------

void Localization(std::string databaseName, std::string videoName)
{

  cout << "Loading database..." << endl;
  OrbDatabase db(databaseName); 
  cout << "... done!" << endl;

  //********************** Loading a video to compare images *******************

  cv::VideoCapture capture(videoName);
  cv::Mat frame;

  if( !capture.isOpened() )
    throw "Error when reading steam_avi";

  vector<vector<cv::Mat > > features;
  features.clear();
  features.reserve( 1 );


  cv::Ptr<cv::ORB> orb = cv::ORB::create();
  cout << "Extracting ORB features..." << endl;

  unsigned int i = 1;
  for( ; ; )
  {
      capture >> frame;
      if(frame.empty())
          break;

      cv::Mat mask;
      vector<cv::KeyPoint> keypoints;
      cv::Mat descriptors;

      orb->detectAndCompute(frame, mask, keypoints, descriptors);

      features.push_back(vector<cv::Mat >());
      ChangeStructure(descriptors, features.back());


      QueryResults ret;

      db.query( features[0], ret, -1, -1 ); // Da las imágenes que más se parecen a la imagen de prueba
      cout << "Searching for Image " << i << ". " << ret[0] << endl;

      features.clear();

      i++;
  }

  cout << "Number of tests: " << i << endl;

}
// ----------------------------------------------------------------------------