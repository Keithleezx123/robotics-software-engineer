#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;

void drive_robot(float lin_x, float ang_z)
{
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (client.call(srv))
    {
        ROS_INFO("%s", srv.response.msg_feedback.c_str());
    }
    else
    {
        ROS_ERROR("Failed to call service /ball_chaser/command_robot");
    }
}

void process_image_callback(const sensor_msgs::Image img)
{
    int white_pixel = 255;

    int image_width = img.width;
    int image_height = img.height;
    int row_step = img.step;

    bool found_white = false;

    for (int y = 0; y < image_height; y++)
    {
        for (int x = 0; x < image_width; x++)
        {
            int i = y * row_step + x * 3;

            if (img.data[i] == white_pixel &&
                img.data[i + 1] == white_pixel &&
                img.data[i + 2] == white_pixel)
            {
                found_white = true;

                if (x < image_width / 3)
                {
                    ROS_INFO("Ball detected on the left");
                    drive_robot(0.5, 0.5);
                }
                else if (x < 2 * image_width / 3)
                {
                    ROS_INFO("Ball detected in the center");
                    drive_robot(0.5, 0.0);
                }
                else
                {
                    ROS_INFO("Ball detected on the right");
                    drive_robot(0.5, -0.5);
                }
                return;
            }
        }
    }

    if (!found_white)
    {
        ROS_INFO("No white ball detected");
        drive_robot(0.0, 0.0);
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;
}
