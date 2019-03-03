#include <iostream>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Invalid arguments:\nUsage:\n\t" << argv[0] << " InputVideoFile" << endl;
        exit(EXIT_FAILURE);
    }

    VideoWriter *output = NULL;
    VideoCapture input(argv[1]);
    Scalar red(0, 0, 255);

    if (!input.isOpened())
    {
        cout << "Failed to open video file: " << argv[1] << endl;
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        Mat frame;
        input >> frame;

        if (frame.empty())
            break;

        if (output == NULL)
        {
            output = new VideoWriter("output.mp4",
                                     CV_FOURCC('H','2','6','4'),
                                     input.get(CV_CAP_PROP_FPS),
                                     Size(input.get(CV_CAP_PROP_FRAME_WIDTH), input.get(CV_CAP_PROP_FRAME_HEIGHT)),
                                     true);
        }

        Mat cvtFrame;
        cvtColor(frame, cvtFrame, COLOR_BGR2GRAY);
        cv::threshold(cvtFrame, cvtFrame, 140, 200, CV_THRESH_BINARY);

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(cvtFrame, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

        vector<int> bricks;
        for (size_t idx = 0; idx < contours.size(); idx++)
        {
            double area = contourArea(contours[idx]);
            if (area > 700)
                bricks.push_back(contours[idx][0].x);
        }

        std::sort(bricks.begin(), bricks.end());
        for (int i = 1; i < (int)bricks.size(); i++)
        {
            if ((bricks[i] - bricks[i - 1]) < 50)
                bricks[i] = bricks[i - 1];
        }

        int count = 1;
        for (int i = 1; i <= (int)bricks.size(); i++)
        {
            if (i == bricks.size() || bricks[i] != bricks[i - 1])
            {
                putText(frame, to_string(count).c_str(), Point2f(bricks[i - 1] + 10, 150), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255), 3);
                count = 1;
            }
            else
                count++;
        }

        output->write(frame);
    }

    input.release();
    output->release();
    return 0;
}