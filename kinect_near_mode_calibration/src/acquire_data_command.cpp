#include <ros/ros.h>
#include <topic_tools/MuxAdd.h>
#include <topic_tools/MuxDelete.h>
#include <topic_tools/MuxSelect.h>

topic_tools::MuxAdd add_req;
topic_tools::MuxDelete del_req;
topic_tools::MuxSelect sel_req;
std_srvs::Empty rgb_req;
std_srvs::Empty ir_req;
std_srvs::Empty depth_req;
std::string dir_name;

void start_rgb(){
  add_req.request.topic = "/camera/rgb/image_mono";
  ros::service::call("/image/mux/add",add_req);
  add_req.request.topic = "/camera/rgb/camera_info";
  ros::service::call("/caminfo/mux/add",add_req);
  //
  sel_req.request.topic = "/camera/rgb/image_mono";
  ros::service::call("/image/mux/select",sel_req);
  sel_req.request.topic = "/camera/rgb/camera_info";
  ros::service::call("/caminfo/mux/select",sel_req);
  //
  del_req.request.topic = "/camera/ir/image_raw";
  ros::service::call("/image/mux/delete",del_req);
  del_req.request.topic = "/camera/ir/camera_info";
  ros::service::call("/caminfo/mux/delete",del_req);
  //
}

void start_ir(){
  add_req.request.topic = "/camera/ir/image_raw";
  ros::service::call("/image/mux/add",add_req);
  add_req.request.topic = "/camera/ir/camera_info";
  ros::service::call("/caminfo/mux/add",add_req);
  //
  sel_req.request.topic = "/camera/ir/image_raw";
  ros::service::call("/image/mux/select",sel_req);
  sel_req.request.topic = "/camera/ir/camera_info";
  ros::service::call("/caminfo/mux/select",sel_req);
  //
  del_req.request.topic = "/camera/rgb/image_mono";
  ros::service::call("/image/mux/delete",del_req);
  del_req.request.topic = "/camera/rgb/camera_info";
  ros::service::call("/caminfo/mux/delete",del_req);
}

void saveRGB(int cntr){
  printf("saved rgb image\n");
  ros::service::call("/rgb/save_image",rgb_req);
}
void saveDepth(int cntr){
  printf("saved depth image\n");
  ros::service::call("/depth/save_image",depth_req);
}
void saveIR(int cntr){
  printf("saved IR image\n", str);
  ros::service::call("/ir/save_image",ir_req);
}

int main(int argc, char** argv){
  bool ir_mode = false;
  int ir_num = 0;
  int depth_num = 0;
  int rgb_num = 0;
  char buf[20];

  ros::init(argc, argv, "acquire data manager");
  
  while (ros::ok){
    char key;
    printf("input command : ");
    fgets(buf, sizeof(buf), stdin);
    sscanf(buf,"%s",&key);

    if (key == 27){
      return 0;
    }

    // Toggle the IR mode on/off
    if (key == 'i') {
      if (ir_mode){
        printf("set to rgb mode\n");
        start_rgb ();
      } else {
        printf("set to ir mode\n");
        start_ir ();
      }
      ir_mode = !ir_mode;
    }

    // save images
    if (key == 's') {
      if (ir_mode) {
        saveIR (ir_num);
        ir_num++;
      } else {
        saveRGB (rgb_num);
        saveDepth (depth_num);
        rgb_num++;
        depth_num++;
      }
    }

    // back up image number
    if (key == 'b') {
      printf("number is backed up\n");
      if (ir_mode) ir_num--;
      else { rgb_num--; depth_num--; }
    }
  }
}
