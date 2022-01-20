// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include "../cv-helpers.hpp"
#include <time.h>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

string filename = "";
string filename_depth = "";
string root_rgb = "/home/realsense/lettuce_img/rgb/"; 
string root_depth = "/home/realsense/lettuce_img/depth/"; 

string check_str(int val, string str)
{
    if(val < 10)
    {
        str.append(to_string(0));
        str.append(to_string(val));
    }
    else
    {
        str.append(to_string(val));
    }

    return str;
}

void getCurrentTime(void)
{
    time_t curTime = time(NULL);

    struct tm pLocal;
    localtime_r(&curTime, &pLocal);

    int year = pLocal.tm_year + 1900;
    int month = pLocal.tm_mon + 1;
    int mday = pLocal.tm_mday;
    int hour = pLocal.tm_hour;
    int min = pLocal.tm_min;

    string month_str = "";
    string mday_str = "";
    string hour_str = "";
    string min_str = "";

    month_str = check_str(month, month_str);
    mday_str = check_str(mday, mday_str);
    hour_str = check_str(hour, hour_str);
    min_str = check_str(min, min_str);

    filename.append(to_string(year));
    filename.append(".");
    filename.append(month_str);
    filename.append(".");
    filename.append(mday_str);
    filename.append("_");
    filename.append(hour_str);
    filename.append(min_str);

}

int main(int argc, char * argv[]) try
{
    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();
    
    rs2::align align_to(RS2_STREAM_COLOR);

    using namespace cv;
    const auto window_name = "Display Image--------";
    const auto rgb_window_name = "RGB";
    const auto depth_window_name = "Depth";
    //namedWindow(window_name, WINDOW_AUTOSIZE);
    //namedWindow(rgb_window_name, WINDOW_AUTOSIZE);
    //namedWindow(depth_window_name, WINDOW_AUTOSIZE);

    //while (waitKey(1) < 0 && getWindowProperty(rgb_window_name, WND_PROP_AUTOSIZE) >= 0)
    //{
        rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
        data = align_to.process(data);
        rs2::frame color = data.get_color_frame();
        rs2::frame depth = data.get_depth_frame();
        
        auto color_frame = data.get_color_frame();
        auto depth_frame = data.get_depth_frame();
        
        auto color_mat = frame_to_mat(color_frame);
        auto depth_mat = frame_to_mat(depth_frame);
        
        
        //rs2::frame depth = data.get_depth_frame().apply_filter(color_map);

        // Query frame size (width and height)
        const int w = color.as<rs2::video_frame>().get_width();
        const int h = color.as<rs2::video_frame>().get_height();
        
        const int dw = depth.as<rs2::video_frame>().get_width();
        const int dh = depth.as<rs2::video_frame>().get_height();

        // Create OpenCV matrix of size (w,h) from the colorized depth data
        //Mat image(Size(w, h), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);
        //Mat rgb_image(Size(w, h), CV_8UC3, (void*)color.get_data(), Mat::AUTO_STEP);
        //Mat detph_image(Size(dw, dh), CV_8U, (void*)depth.get_data(), Mat::AUTO_STEP);
        
        //cvtColor(rgb_image, rgb_image, COLOR_BGR2RGB);
        
        depth_mat.convertTo(depth_mat, CV_8U);
        
        //printf("%d\n", depth_mat.type());

        // Update the window with new data
        //imshow(window_name, image);
        //imshow(rgb_window_name, color_mat);
        //imshow(depth_window_name, depth_mat);
        
        string fname_rgb = "";
        string fname_depth = "";
    
        getCurrentTime();
        
        fname_rgb = root_rgb + filename + ".jpg";
        fname_depth = root_depth + filename + ".jpg";
        
        cout << fname_rgb <<endl;
        cout << fname_depth <<endl;
     
        imwrite(fname_rgb, color_mat);
        imwrite(fname_depth, depth_mat);
        
        filename = "";
        
        
    //W }

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}



