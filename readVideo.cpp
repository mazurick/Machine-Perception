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
                                     false);
        }

        Mat cvtFrame;
        cvtColor(frame, cvtFrame, COLOR_BGR2GRAY);
        output->write(cvtFrame);
    }

    input.release();
    output->release();
    return 0;
}
