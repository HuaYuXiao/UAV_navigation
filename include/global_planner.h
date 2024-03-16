#ifndef GLOBAL_PLANNER
#define GLOBAL_PLANNER


#include "A_star.h"


using namespace std;


#define NODE_NAME "Global_Planner"


namespace Global_Planning{
    extern ros::Publisher message_pub;
    class Global_Planner{
    private:
        ros::NodeHandle global_planner_nh;

        double time_per_path;
        // 根据不同的输入（激光雷达输入、相机输入等）生成occupymap
        bool map_input;
        double replan_time;

        // 调用路径规划算法 生成路径
        // 调用轨迹优化算法 规划轨迹

        // 订阅无人机状态、目标点、传感器数据（生成地图）
        ros::Subscriber goal_sub;
        ros::Subscriber drone_state_sub;
        // 支持2维激光雷达、3维激光雷达、D435i等实体传感器
        // 支持直接输入全局已知点云
        ros::Subscriber Gpointcloud_sub;
        ros::Subscriber Lpointcloud_sub;
        ros::Subscriber laserscan_sub;
        // ？

        // 发布控制指令
        ros::Publisher command_pub;
        ros::Publisher path_cmd_pub;

        ros::Timer mainloop_timer;
        ros::Timertrack_path_timer;

        // A星规划器
        Astar::Ptr Astar_ptr;

        prometheus_msgs::DroneState _DroneState;
        nav_msgs::Odometry Drone_odom;
        nav_msgs::Path path_cmd;
        prometheus_msgs::ControlCommand Command_Now;

        double distance_walked;
        double distance_to_goal;

        // 规划器状态
        bool goal_ready;
        bool is_new_path;
        bool path_ok;
        int start_point_index;
        int Num_total_wp;
        int cur_id;

        // 规划初始状态及终端状态
        Eigen::Vector3d start_pos, start_vel, start_acc, goal_pos, goal_vel;

        float desired_yaw;

        // 打印的提示消息
        string message;

        // 3种状态机
        enum EXEC_STATE{
            IDLE,
            PLANNING,
            TRACKING,
        };
        EXEC_STATE exec_state;

        // 回调函数
        void goal_cb(const geometry_msgs::PoseStampedConstPtr& msg);
        void drone_state_cb(const prometheus_msgs::DroneStateConstPtr &msg);
        void Gpointcloud_cb(const sensor_msgs::PointCloud2ConstPtr &msg);
        void Lpointcloud_cb(const sensor_msgs::PointCloud2ConstPtr &msg);
        void laser_cb(const sensor_msgs::LaserScanConstPtr &msg);
        void mainloop_cb(const ros::TimerEvent& e);
        void track_path_cb(const ros::TimerEvent& e);

        int get_start_point_id(void);

    public:
        Global_Planner(void):
            global_planner_nh("~")
        {}~Global_Planner(){}

        void init(ros::NodeHandle& nh);
    };
}

#endif