/***************************************************************************
  * file name cmhi_location_message_body.h
  * brief 位置消息体
  * date 2019/02/12
  *************************************************************************/

#ifndef CMCC_IM_LOCATION_MESSAGE_BODY_H_
#define CMCC_IM_LOCATION_MESSAGE_BODY_H_

#include "cmhi_file_message_body.h"

namespace cmhi_iov{

/******************************************************************************
 * Class definition and declaration
 ******************************************************************************/

/*

	// 位置消息
	message LocationMessage {
	    string file_name = 1; // 位置缩略图名称
	    string origin_link = 2; // 位置图片链接
	    uint64 file_length = 4; // 文件大小，以B为单位
	    uint32 width = 5; // 图片宽度
	    uint32 height = 6; // 图片高度
	    double longitude = 7; // 经度
	    double latitude = 8; // 纬度
	    string location_desc = 9; // 位置描述信息
	    string location_address = 10; // 位置地址
	}


	string file_name;		//文件名称
	string origin_link; 	//原始链接
	uint32_t file_length;	//文件大小
	string local_path;

*/

class LocationMessageBody: public FileMessageBody{
	
		public:
			
			LocationMessageBody(){}
			LocationMessageBody(string local_path);
			LocationMessageBody(string file_name, string local_path);
			~LocationMessageBody(){}
			
			uint32_t get_width(){return width;}
			uint32_t get_height(){return height;}
			double get_longitude(){return longitude;}
			double get_latitude(){return latitude;}
			string get_location_desc(){return location_desc;}
			string get_location_address(){return location_address;}
			string get_poi_id(){return poi_id;}

			void set_width(uint32_t width);
			void set_height(uint32_t height);
			void set_longitude(double longitude);
			void set_latitude(double latitude);
			void set_location_desc(string location_desc);
			void set_location_address(string location_address);
			void set_poi_id(string poi_id);
			
		private:
			uint32_t width;
			uint32_t height;
			double longitude;
			double latitude;
			string location_desc;
			string location_address;
			string poi_id;
		
	};

}
#endif //CMCC_IM_LOCATION_MESSAGE_BODY_H_


