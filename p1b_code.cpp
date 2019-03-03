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
    Vec3b new_pixel_value(20, 143, 247);

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

        for(int j = 90; j < frame.rows; ++j)
        {
            for(int i = 0; i < frame.cols; ++i)
            {
                const Vec3b pixel = frame.at<Vec3b>(j, i);
                if (pixel[1] == 0 && pixel[0] < 30 && pixel[2] >= 100)
                    frame.at<Vec3b>(j, i) = new_pixel_value;
            }
        }

        output->write(frame);
    }

    input.release();
    output->release();
    return 0;
}