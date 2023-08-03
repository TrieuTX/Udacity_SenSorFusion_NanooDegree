
#include <algorithm>
#include <iostream>
#include <numeric>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "camFusion.hpp"
#include "dataStructures.h"

using namespace std;

// Create groups of Lidar points whose projection into the camera falls into the same bounding box
void clusterLidarWithROI(std::vector<BoundingBox> &boundingBoxes, std::vector<LidarPoint> &lidarPoints,
                         float shrinkFactor, cv::Mat &P_rect_xx, cv::Mat &R_rect_xx, cv::Mat &RT)
{
    // loop over all Lidar points and associate them to a 2D bounding box
    cv::Mat X(4, 1, cv::DataType<double>::type);
    cv::Mat Y(3, 1, cv::DataType<double>::type);

    for (auto it1 = lidarPoints.begin(); it1 != lidarPoints.end(); ++it1)
    {
        // assemble vector for matrix-vector-multiplication
        X.at<double>(0, 0) = it1->x;
        X.at<double>(1, 0) = it1->y;
        X.at<double>(2, 0) = it1->z;
        X.at<double>(3, 0) = 1;

        // project Lidar point into camera
        Y = P_rect_xx * R_rect_xx * RT * X;
        cv::Point pt;
        // pixel coordinates
        pt.x = Y.at<double>(0, 0) / Y.at<double>(2, 0);
        pt.y = Y.at<double>(1, 0) / Y.at<double>(2, 0);

        vector<vector<BoundingBox>::iterator>
            enclosingBoxes; // pointers to all bounding boxes which enclose the current Lidar point
        for (vector<BoundingBox>::iterator it2 = boundingBoxes.begin(); it2 != boundingBoxes.end(); ++it2)
        {
            // shrink current bounding box slightly to avoid having too many outlier points around the edges
            cv::Rect smallerBox;
            smallerBox.x = (*it2).roi.x + shrinkFactor * (*it2).roi.width / 2.0;
            smallerBox.y = (*it2).roi.y + shrinkFactor * (*it2).roi.height / 2.0;
            smallerBox.width = (*it2).roi.width * (1 - shrinkFactor);
            smallerBox.height = (*it2).roi.height * (1 - shrinkFactor);

            // check wether point is within current bounding box
            if (smallerBox.contains(pt))
            {
                enclosingBoxes.push_back(it2);
            }

        } // eof loop over all bounding boxes

        // check wether point has been enclosed by one or by multiple boxes
        if (enclosingBoxes.size() == 1)
        {
            // add Lidar point to bounding box
            enclosingBoxes[0]->lidarPoints.push_back(*it1);
        }

    } // eof loop over all Lidar points
}

/*
 * The show3DObjects() function below can handle different output image sizes, but the text output has been manually
 * tuned to fit the 2000x2000 size. However, you can make this function work for other sizes too. For instance, to use a
 * 1000x1000 size, adjusting the text positions by dividing them by 2.
 */
void show3DObjects(std::vector<BoundingBox> &boundingBoxes, cv::Size worldSize, cv::Size imageSize, bool bWait)
{
    // create topview image
    cv::Mat topviewImg(imageSize, CV_8UC3, cv::Scalar(255, 255, 255));

    for (auto it1 = boundingBoxes.begin(); it1 != boundingBoxes.end(); ++it1)
    {
        // create randomized color for current 3D object
        cv::RNG rng(it1->boxID);
        cv::Scalar currColor = cv::Scalar(rng.uniform(0, 150), rng.uniform(0, 150), rng.uniform(0, 150));

        // plot Lidar points into top view image
        int top = 1e8, left = 1e8, bottom = 0.0, right = 0.0;
        float xwmin = 1e8, ywmin = 1e8, ywmax = -1e8;
        for (auto it2 = it1->lidarPoints.begin(); it2 != it1->lidarPoints.end(); ++it2)
        {
            // world coordinates
            float xw = (*it2).x; // world position in m with x facing forward from sensor
            float yw = (*it2).y; // world position in m with y facing left from sensor
            xwmin = xwmin < xw ? xwmin : xw;
            ywmin = ywmin < yw ? ywmin : yw;
            ywmax = ywmax > yw ? ywmax : yw;

            // top-view coordinates
            int y = (-xw * imageSize.height / worldSize.height) + imageSize.height;
            int x = (-yw * imageSize.width / worldSize.width) + imageSize.width / 2;

            // find enclosing rectangle
            top = top < y ? top : y;
            left = left < x ? left : x;
            bottom = bottom > y ? bottom : y;
            right = right > x ? right : x;

            // draw individual point
            cv::circle(topviewImg, cv::Point(x, y), 4, currColor, -1);
        }

        // draw enclosing rectangle
        cv::rectangle(topviewImg, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 0, 0), 2);

        // augment object with some key data
        char str1[200], str2[200];
        sprintf(str1, "id=%d, #pts=%d", it1->boxID, (int)it1->lidarPoints.size());
        putText(topviewImg, str1, cv::Point2f(left - 250, bottom + 50), cv::FONT_ITALIC, 2, currColor);
        sprintf(str2, "xmin=%2.2f m, yw=%2.2f m", xwmin, ywmax - ywmin);
        putText(topviewImg, str2, cv::Point2f(left - 250, bottom + 125), cv::FONT_ITALIC, 2, currColor);
    }

    // plot distance markers
    float lineSpacing = 2.0; // gap between distance markers
    int nMarkers = floor(worldSize.height / lineSpacing);
    for (size_t i = 0; i < nMarkers; ++i)
    {
        int y = (-(i * lineSpacing) * imageSize.height / worldSize.height) + imageSize.height;
        cv::line(topviewImg, cv::Point(0, y), cv::Point(imageSize.width, y), cv::Scalar(255, 0, 0));
    }

    // display image
    string windowName = "3D Objects";
    cv::namedWindow(windowName, 1);
    cv::imshow(windowName, topviewImg);

    if (bWait)
    {
        cv::waitKey(0); // wait for key to be pressed
    }
}

// associate a given bounding box with the keypoints it contains
void clusterKptMatchesWithROI(BoundingBox &boundingBox, std::vector<cv::KeyPoint> &kptsPrev,
                              std::vector<cv::KeyPoint> &kptsCurr, std::vector<cv::DMatch> &kptMatches)
{
    std::vector<double> euclideanDistance;
    for (auto &match : kptMatches)
    {
        if (boundingBox.roi.contains(kptsCurr[match.trainIdx].pt))
        {
            euclideanDistance.push_back(match.distance); // match.distance = distance between trainIdx and queryIdx
                                                         // (currKeypoint and preKeypoint)
        }
    }
    double euclideanDistanceMean =
        std::accumulate(euclideanDistance.begin(), euclideanDistance.end(), 0.0) / euclideanDistance.size();
    for (auto &match : kptMatches)
    {
        if (boundingBox.roi.contains(kptsCurr[match.trainIdx].pt))
        {
            double distanceCurrAndPreKeyPt = match.distance;
            if (distanceCurrAndPreKeyPt <
                euclideanDistanceMean * 1.2) // remove those that are too far away from the mean, > 1.2*mean
            {
                boundingBox.keypoints.push_back(kptsCurr[match.trainIdx]);
                boundingBox.kptMatches.push_back(match);
            }
        }
    }
}

// Compute time-to-collision (TTC) based on keypoint correspondences in successive images
void computeTTCCamera(std::vector<cv::KeyPoint> &kptsPrev, std::vector<cv::KeyPoint> &kptsCurr,
                      std::vector<cv::DMatch> kptMatches, double frameRate, double &TTC, cv::Mat *visImg)
{
    // ...
}

void computeTTCLidar(std::vector<LidarPoint> &lidarPointsPrev, std::vector<LidarPoint> &lidarPointsCurr,
                     double frameRate, double &TTC)
{
    // check equation in Section 2 in Engineering a Collision Detection System
    double dT = 0.1 / frameRate; // time between two measurements
    double lane_width = 4.0;     // width of the ego lane

    // find closest point in Lidarpoints cloud within ego lane, Point have x smallest value and y within (-2,2)
    double minXPrev = 1e9, minXCurr = 1e9;

    for (auto it = lidarPointsPrev.begin(); it != lidarPointsPrev.end(); ++it)
    {
        if (abs(it->y) <= lane_width / 2.0)
        {
            minXPrev = minXPrev > it->x ? it->x : minXPrev;
        }
    }

    for (auto it = lidarPointsCurr.begin(); it != lidarPointsCurr.end(); ++it)
    {
        if (abs(it->y) <= lane_width / 2.0)
        {
            minXCurr = minXCurr > it->x ? it->x : minXCurr;
        }
    }
    // compute TTC from both measurements
    TTC = minXCurr * dT / (minXPrev - minXCurr);
}

void matchBoundingBoxes(std::vector<cv::DMatch> &matches, std::map<int, int> &bbBestMatches, DataFrame &prevFrame,
                        DataFrame &currFrame)
{
    // match Bounding Box in prev DataFame and curr Datafame
    int number_bb_prevFrame = prevFrame.boundingBoxes.size();
    int number_bb_currFrame = currFrame.boundingBoxes.size();
    int point_count[number_bb_prevFrame]
                   [number_bb_currFrame]; // count number of points in common between pre_bb and curr_bb

    for (int i = 0; i < number_bb_prevFrame; i++)
    {
        for (int j = 0; j < number_bb_currFrame; j++)
        {
            point_count[i][j] = 0;
        }
    }

    // counting points per bounding box,
    for (auto itr = matches.begin(); itr != matches.end(); itr++)
    {
        // previous frame
        cv::KeyPoint query = prevFrame.keypoints[itr->queryIdx];
        auto queryPoint = cv::Point(query.pt.x, query.pt.y);
        bool queryPointFound = false;

        // current frame
        cv::KeyPoint train = currFrame.keypoints[itr->trainIdx];
        auto trainPoint = cv::Point(train.pt.x, train.pt.y);
        bool trainPointFound = false;

        std::vector<int> queryId, trainId;

        // if bounding boxes contain matched points then add to vector
        for (int i = 0; i < number_bb_prevFrame; i++)
        {
            if (prevFrame.boundingBoxes[i].roi.contains(queryPoint))
            {
                queryPointFound = true;
                queryId.push_back(i);
            }
        }

        for (int i = 0; i < number_bb_currFrame; i++)
        {
            if (currFrame.boundingBoxes[i].roi.contains(trainPoint))
            {
                trainPointFound = true;
                trainId.push_back(i);
            }
        }

        if (queryPointFound && trainPointFound)
        {
            for (auto id_prev : queryId)
            {
                for (auto id_curr : trainId)
                {
                    point_count[id_prev][id_curr] += 1;
                }
            }
        }
    }

    // best match for each bounding box
    for (int i = 0; i < number_bb_prevFrame; i++)
    {
        int max_count = 0;
        int id_max = 0;

        for (int j = 0; j < number_bb_currFrame; j++)
        {
            if (point_count[i][j] > max_count)
            {
                max_count = point_count[i][j];
                id_max = j;
            }
        }
        // map Bounding box i in pre Frame with Bounding Box j in curr Frame, they have most number keypoint in common
        bbBestMatches[i] = id_max;
    }
}
