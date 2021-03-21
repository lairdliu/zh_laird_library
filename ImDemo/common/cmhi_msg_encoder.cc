/***************************************************************************
  * file name cmhi_msg_encoder.cc
  * brief 
  * date 2019/03/21
  *************************************************************************/

#include "cmhi_msg_encoder.h"
#include "common/msg.pb.h"
#include "cmhi_custom_message_body.h"
#include "cmhi_text_message_body.h"
#include "cmhi_audio_message_body.h"
#include "cmhi_read_receipt_message_body.h"
#include "cmhi_location_message_body.h"
#include "cmhi_friend_message_body.h"

using namespace std;
namespace cmhi_iov{
    
bool CCMessageEncoder::serialize_custom_msgbody(CustomMessageBody& from, string& to) {
  common::msg::CustomMessage pb_msg;
  pb_msg.set_subtype(from.get_subtype());
  pb_msg.set_notification(from.get_notification());
  for (int i=0; i<from.get_entity_size(); i++){
    common::msg::CustomEntity *pb_entity = pb_msg.add_custom_entity();
    pb_entity->set_key(from.get_entity(i).key);
    pb_entity->set_value(from.get_entity(i).value);
  }
  
  pb_msg.SerializeToString(&to);
  return true;
}

bool CCMessageEncoder::deserialize_custom_msgbody(const string& from, CustomMessageBody& to) {
  common::msg::CustomMessage pb_msg;
  bool status = pb_msg.ParseFromString(from);

  if(!status) {
    return false;
  }
  to.set_subtype(pb_msg.subtype());
  to.set_notification(pb_msg.notification());
  for(int i=0; i<pb_msg.custom_entity_size(); i++) {
    Custom_Entity entity;
    common::msg::CustomEntity pb_entity = pb_msg.custom_entity(i);
    entity.key = pb_entity.key();
    entity.value = pb_entity.value();
    to.set_entity(entity);
  }
  to.set_message_body_type(kMessageBodyTypeCustom);
  return true;
}


bool CCMessageEncoder::serialize_text_msgbody(TextMessageBody& from, string& to){
    common::msg::TextMessage pb_msg;
    pb_msg.set_text(from.get_text());
    pb_msg.set_burn_after_read(false);
  
    pb_msg.SerializeToString(&to);
    return true;
}

bool CCMessageEncoder::deserialize_text_msgbody(const string& from, TextMessageBody& to){
    common::msg::TextMessage pb_msg;
    bool status = pb_msg.ParseFromString(from);

    if(!status) {
        return false;
    }
    to.set_text(pb_msg.text());
	to.set_message_body_type(kMessageBodyTypeText);
    return true;
}

bool CCMessageEncoder::serialize_audio_msgbody(AudioMessageBody& from, string& to){
    common::msg::AudioMessage pb_msg;
    pb_msg.set_file_name(from.get_file_name());
    pb_msg.set_audio_link(from.get_origin_link());
    pb_msg.set_file_length(from.get_file_length());
    pb_msg.set_duration(from.get_duration());
    pb_msg.set_audio_to_word(from.get_voice_to_word());
  
    pb_msg.SerializeToString(&to);
    return true;
}

bool CCMessageEncoder::deserialize_audio_msgbody(const string& from, AudioMessageBody& to){
    common::msg::AudioMessage pb_msg;
    bool status = pb_msg.ParseFromString(from);

    if(!status) {
        return false;
    }
    to.set_file_name(pb_msg.file_name());
    to.set_origin_link(pb_msg.audio_link());
    to.set_file_length(pb_msg.file_length());
    to.set_duration(pb_msg.duration());
    to.set_voice_to_word(pb_msg.audio_to_word());
	to.set_message_body_type(kMessageBodyTypeVoice);
    return true;
}

bool CCMessageEncoder::serialize_read_receipt_msgbody(ReadReceiptMessageBody& from, string& to){
    common::msg::ReadReceiptMessage pb_msg;
    pb_msg.set_receipt_guid(from.get_receipt_guid());
  
    pb_msg.SerializeToString(&to);
    return true;
}

bool CCMessageEncoder::deserialize_read_receipt_msgbody(const string& from, ReadReceiptMessageBody& to){
    common::msg::ReadReceiptMessage pb_msg;
    bool status = pb_msg.ParseFromString(from);

    if(!status) {
        return false;
    }
    
    to.set_receipt_guid(pb_msg.receipt_guid());

    return true;
}

bool CCMessageEncoder::serialize_location_msgbody(LocationMessageBody& from, string& to)
{
    common::msg::LocationMessage pb_msg;
    pb_msg.set_file_length(from.get_file_length());
  	pb_msg.set_file_name(from.get_file_name());
	pb_msg.set_origin_link(from.get_origin_link());
	pb_msg.set_height(from.get_height());
	pb_msg.set_width(from.get_width());
	pb_msg.set_latitude(from.get_latitude());
	pb_msg.set_longitude(from.get_longitude());
	pb_msg.set_location_desc(from.get_location_desc());
	pb_msg.set_location_address(from.get_location_address());
  	pb_msg.set_poi_id(from.get_poi_id());
    pb_msg.SerializeToString(&to);
    return true;
}
bool CCMessageEncoder::deserialize_location_msgbody(const string& from, LocationMessageBody& to)
{
	common::msg::LocationMessage pb_msg;
	bool status = pb_msg.ParseFromString(from);
	if (!status) return false;
	to.set_file_name(pb_msg.file_name());
	to.set_file_length(pb_msg.file_length());
	to.set_origin_link(pb_msg.origin_link());
	to.set_width(pb_msg.width());
	to.set_height(pb_msg.height());
	to.set_latitude(pb_msg.latitude());
	to.set_longitude(pb_msg.longitude());
	to.set_location_desc(pb_msg.location_desc());
	to.set_location_address(pb_msg.location_address());
	to.set_poi_id(pb_msg.poi_id());
	to.set_message_body_type(kMessageBodyTypeLocation);

	
	return true;
}

bool  CCMessageEncoder::serialize_friend_msgbody(FriendMessageBody& from, string& to)
{
	common::msg::FriendMessage pb_msg;
	pb_msg.set_text(from.get_text());
    pb_msg.SerializeToString(&to);
	return true;
}

bool  CCMessageEncoder::deserialize_friend_msgbody(const string& from, FriendMessageBody& to)
{
	common::msg::FriendMessage pb_msg;
	bool status = pb_msg.ParseFromString(from);
	if (!status) return false;
	to.set_text(pb_msg.text());
	to.set_message_body_type(kMessageBodyTypeFriend);
	return true;
}



}

