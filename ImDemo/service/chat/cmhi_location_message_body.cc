/***************************************************************************
  * file name cmim_audio_message_body.cpp
  * brief 语音消息内容
  * date 2019/02/12
  *************************************************************************/

#include "cmhi_location_message_body.h"

namespace cmhi_iov{

	LocationMessageBody::LocationMessageBody(string local_path)
	{
	
    set_width(0);
    set_height(0);
    set_longitude(0);
    set_latitude(0);
    set_location_desc("");
    set_location_address("");
  
		int32_t len;
	    string file_name;
	    len = local_path.find_last_of("/");
	    if (len != -1)        
	    {           
	        file_name = local_path.substr(len + 1, local_path.length() - len);
	        this->set_file_name(file_name);
	    }
		this->set_local_path(local_path) ;
		set_message_body_type(kMessageBodyTypeLocation);
	}
	
	LocationMessageBody::LocationMessageBody(string file_name, string local_path)
	{
    set_width(0);
    set_height(0);
    set_longitude(0);
    set_latitude(0);
    set_location_desc("");
    set_location_address("");
    
		this->set_file_name(file_name) ;
		this->set_local_path(local_path) ;
		set_message_body_type(kMessageBodyTypeLocation);
	}

	void LocationMessageBody::set_width(uint32_t width)
		{
			this->width = width;
		}
	void LocationMessageBody::set_height(uint32_t height)
		{
			this->height = height;
		}
	void LocationMessageBody::set_longitude(double longitude)
		{
			this->longitude = longitude;
		}
	void LocationMessageBody::set_latitude(double latitude)
		{
			this->latitude = latitude;
		}
	void LocationMessageBody::set_location_desc(string location_desc)
		{
			this->location_desc = location_desc;
		}
	void LocationMessageBody::set_location_address(string location_address)
		{
			this->location_address = location_address;
		}

	void LocationMessageBody::set_poi_id(string poi_id)
		{
			this->poi_id = poi_id;
		}

}

