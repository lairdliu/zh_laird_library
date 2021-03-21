/***************************************************************************
 * file name cmhi_msg_encoder.h
 * brief SDK消息体到protobuf的序列化和反序列化
 * date 2019/03/21
 *************************************************************************/
 
#ifndef COMMON_CMHI_MSG_ENCODER_H
#define COMMON_CMHI_MSG_ENCODER_H

#include <string>
#include "base_typedefs.h"
#include "common_types.h"

using namespace std;

namespace cmhi_iov {
  
class CustomMessageBody;
class AudioMessageBody;
class TextMessageBody;
class ReadReceiptMessageBody;
class LocationMessageBody;
class FriendMessageBody;

class CCMessageEncoder {
public:
  static bool serialize_custom_msgbody(CustomMessageBody& from, string& to);
  static bool deserialize_custom_msgbody(const string& from, CustomMessageBody& to);

  static bool serialize_text_msgbody(TextMessageBody& from, string& to);
  static bool deserialize_text_msgbody(const string& from, TextMessageBody& to);

  static bool serialize_audio_msgbody(AudioMessageBody& from, string& to);
  static bool deserialize_audio_msgbody(const string& from, AudioMessageBody& to);
  
  static bool serialize_read_receipt_msgbody(ReadReceiptMessageBody& from, string& to);
  static bool deserialize_read_receipt_msgbody(const string& from, ReadReceiptMessageBody& to);


  static bool serialize_location_msgbody(LocationMessageBody& from, string& to);
  static bool deserialize_location_msgbody(const string& from, LocationMessageBody& to);


  static bool serialize_friend_msgbody(FriendMessageBody& from, string& to);
  static bool deserialize_friend_msgbody(const string& from, FriendMessageBody& to);

  
};

}

#endif //COMMON_CMHI_MSG_ENCODER_H

