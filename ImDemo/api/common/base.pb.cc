// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common/base.proto

#include "common/base.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
namespace common {
namespace base {
class MobileBaseDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<MobileBase>
      _instance;
} _MobileBase_default_instance_;
}  // namespace base
}  // namespace common
namespace protobuf_common_2fbase_2eproto {
void InitDefaultsMobileBaseImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::common::base::_MobileBase_default_instance_;
    new (ptr) ::common::base::MobileBase();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::common::base::MobileBase::InitAsDefaultInstance();
}

void InitDefaultsMobileBase() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsMobileBaseImpl);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, sdk_version_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, language_type_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, net_type_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, os_version_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, client_type_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, device_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, imei_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, device_id_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::common::base::MobileBase, terminal_device_type_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::common::base::MobileBase)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::common::base::_MobileBase_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "common/base.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\021common/base.proto\022\013common.base\032\030common"
      "/enumeration.proto\"\321\002\n\nMobileBase\022\023\n\013sdk"
      "_version\030\001 \001(\t\0228\n\rlanguage_type\030\002 \001(\0162!."
      "common.enumeration.ELanguageType\0222\n\010net_"
      "type\030\003 \001(\0162 .common.enumeration.ENetwork"
      "Type\022\022\n\nos_version\030\004 \001(\t\0224\n\013client_type\030"
      "\005 \001(\0162\037.common.enumeration.EClientType\022\016"
      "\n\006device\030\006 \001(\t\022\014\n\004imei\030\007 \001(\t\022\021\n\tdevice_i"
      "d\030\010 \001(\t\022E\n\024terminal_device_type\030\t \001(\0162\'."
      "common.enumeration.ETerminalDeviceTypeB\'"
      "\n\035com.cmcc.littlec.proto.commonB\004BaseP\000b"
      "\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 447);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "common/base.proto", &protobuf_RegisterTypes);
  ::protobuf_common_2fenumeration_2eproto::AddDescriptors();
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_common_2fbase_2eproto
namespace common {
namespace base {

// ===================================================================

void MobileBase::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int MobileBase::kSdkVersionFieldNumber;
const int MobileBase::kLanguageTypeFieldNumber;
const int MobileBase::kNetTypeFieldNumber;
const int MobileBase::kOsVersionFieldNumber;
const int MobileBase::kClientTypeFieldNumber;
const int MobileBase::kDeviceFieldNumber;
const int MobileBase::kImeiFieldNumber;
const int MobileBase::kDeviceIdFieldNumber;
const int MobileBase::kTerminalDeviceTypeFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

MobileBase::MobileBase()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_common_2fbase_2eproto::InitDefaultsMobileBase();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:common.base.MobileBase)
}
MobileBase::MobileBase(const MobileBase& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  sdk_version_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.sdk_version().size() > 0) {
    sdk_version_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.sdk_version_);
  }
  os_version_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.os_version().size() > 0) {
    os_version_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.os_version_);
  }
  device_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.device().size() > 0) {
    device_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.device_);
  }
  imei_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.imei().size() > 0) {
    imei_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.imei_);
  }
  device_id_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.device_id().size() > 0) {
    device_id_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.device_id_);
  }
  ::memcpy(&language_type_, &from.language_type_,
    static_cast<size_t>(reinterpret_cast<char*>(&terminal_device_type_) -
    reinterpret_cast<char*>(&language_type_)) + sizeof(terminal_device_type_));
  // @@protoc_insertion_point(copy_constructor:common.base.MobileBase)
}

void MobileBase::SharedCtor() {
  sdk_version_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  os_version_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  device_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  imei_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  device_id_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&language_type_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&terminal_device_type_) -
      reinterpret_cast<char*>(&language_type_)) + sizeof(terminal_device_type_));
  _cached_size_ = 0;
}

MobileBase::~MobileBase() {
  // @@protoc_insertion_point(destructor:common.base.MobileBase)
  SharedDtor();
}

void MobileBase::SharedDtor() {
  sdk_version_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  os_version_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  device_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  imei_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  device_id_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void MobileBase::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MobileBase::descriptor() {
  ::protobuf_common_2fbase_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_common_2fbase_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const MobileBase& MobileBase::default_instance() {
  ::protobuf_common_2fbase_2eproto::InitDefaultsMobileBase();
  return *internal_default_instance();
}

MobileBase* MobileBase::New(::google::protobuf::Arena* arena) const {
  MobileBase* n = new MobileBase;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void MobileBase::Clear() {
// @@protoc_insertion_point(message_clear_start:common.base.MobileBase)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  sdk_version_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  os_version_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  device_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  imei_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  device_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&language_type_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&terminal_device_type_) -
      reinterpret_cast<char*>(&language_type_)) + sizeof(terminal_device_type_));
  _internal_metadata_.Clear();
}

bool MobileBase::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:common.base.MobileBase)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // string sdk_version = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_sdk_version()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->sdk_version().data(), static_cast<int>(this->sdk_version().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "common.base.MobileBase.sdk_version"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .common.enumeration.ELanguageType language_type = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          set_language_type(static_cast< ::common::enumeration::ELanguageType >(value));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .common.enumeration.ENetworkType net_type = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          set_net_type(static_cast< ::common::enumeration::ENetworkType >(value));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string os_version = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u /* 34 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_os_version()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->os_version().data(), static_cast<int>(this->os_version().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "common.base.MobileBase.os_version"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .common.enumeration.EClientType client_type = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(40u /* 40 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          set_client_type(static_cast< ::common::enumeration::EClientType >(value));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string device = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(50u /* 50 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_device()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->device().data(), static_cast<int>(this->device().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "common.base.MobileBase.device"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string imei = 7;
      case 7: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(58u /* 58 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_imei()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->imei().data(), static_cast<int>(this->imei().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "common.base.MobileBase.imei"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string device_id = 8;
      case 8: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(66u /* 66 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_device_id()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->device_id().data(), static_cast<int>(this->device_id().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "common.base.MobileBase.device_id"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .common.enumeration.ETerminalDeviceType terminal_device_type = 9;
      case 9: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(72u /* 72 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          set_terminal_device_type(static_cast< ::common::enumeration::ETerminalDeviceType >(value));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:common.base.MobileBase)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:common.base.MobileBase)
  return false;
#undef DO_
}

void MobileBase::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:common.base.MobileBase)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string sdk_version = 1;
  if (this->sdk_version().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->sdk_version().data(), static_cast<int>(this->sdk_version().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.sdk_version");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->sdk_version(), output);
  }

  // .common.enumeration.ELanguageType language_type = 2;
  if (this->language_type() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      2, this->language_type(), output);
  }

  // .common.enumeration.ENetworkType net_type = 3;
  if (this->net_type() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      3, this->net_type(), output);
  }

  // string os_version = 4;
  if (this->os_version().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->os_version().data(), static_cast<int>(this->os_version().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.os_version");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      4, this->os_version(), output);
  }

  // .common.enumeration.EClientType client_type = 5;
  if (this->client_type() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      5, this->client_type(), output);
  }

  // string device = 6;
  if (this->device().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->device().data(), static_cast<int>(this->device().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.device");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      6, this->device(), output);
  }

  // string imei = 7;
  if (this->imei().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->imei().data(), static_cast<int>(this->imei().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.imei");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      7, this->imei(), output);
  }

  // string device_id = 8;
  if (this->device_id().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->device_id().data(), static_cast<int>(this->device_id().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.device_id");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      8, this->device_id(), output);
  }

  // .common.enumeration.ETerminalDeviceType terminal_device_type = 9;
  if (this->terminal_device_type() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      9, this->terminal_device_type(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:common.base.MobileBase)
}

::google::protobuf::uint8* MobileBase::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:common.base.MobileBase)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string sdk_version = 1;
  if (this->sdk_version().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->sdk_version().data(), static_cast<int>(this->sdk_version().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.sdk_version");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->sdk_version(), target);
  }

  // .common.enumeration.ELanguageType language_type = 2;
  if (this->language_type() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      2, this->language_type(), target);
  }

  // .common.enumeration.ENetworkType net_type = 3;
  if (this->net_type() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      3, this->net_type(), target);
  }

  // string os_version = 4;
  if (this->os_version().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->os_version().data(), static_cast<int>(this->os_version().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.os_version");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->os_version(), target);
  }

  // .common.enumeration.EClientType client_type = 5;
  if (this->client_type() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      5, this->client_type(), target);
  }

  // string device = 6;
  if (this->device().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->device().data(), static_cast<int>(this->device().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.device");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        6, this->device(), target);
  }

  // string imei = 7;
  if (this->imei().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->imei().data(), static_cast<int>(this->imei().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.imei");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        7, this->imei(), target);
  }

  // string device_id = 8;
  if (this->device_id().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->device_id().data(), static_cast<int>(this->device_id().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "common.base.MobileBase.device_id");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        8, this->device_id(), target);
  }

  // .common.enumeration.ETerminalDeviceType terminal_device_type = 9;
  if (this->terminal_device_type() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      9, this->terminal_device_type(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:common.base.MobileBase)
  return target;
}

size_t MobileBase::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:common.base.MobileBase)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // string sdk_version = 1;
  if (this->sdk_version().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->sdk_version());
  }

  // string os_version = 4;
  if (this->os_version().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->os_version());
  }

  // string device = 6;
  if (this->device().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->device());
  }

  // string imei = 7;
  if (this->imei().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->imei());
  }

  // string device_id = 8;
  if (this->device_id().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->device_id());
  }

  // .common.enumeration.ELanguageType language_type = 2;
  if (this->language_type() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->language_type());
  }

  // .common.enumeration.ENetworkType net_type = 3;
  if (this->net_type() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->net_type());
  }

  // .common.enumeration.EClientType client_type = 5;
  if (this->client_type() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->client_type());
  }

  // .common.enumeration.ETerminalDeviceType terminal_device_type = 9;
  if (this->terminal_device_type() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->terminal_device_type());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void MobileBase::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:common.base.MobileBase)
  GOOGLE_DCHECK_NE(&from, this);
  const MobileBase* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const MobileBase>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:common.base.MobileBase)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:common.base.MobileBase)
    MergeFrom(*source);
  }
}

void MobileBase::MergeFrom(const MobileBase& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:common.base.MobileBase)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.sdk_version().size() > 0) {

    sdk_version_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.sdk_version_);
  }
  if (from.os_version().size() > 0) {

    os_version_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.os_version_);
  }
  if (from.device().size() > 0) {

    device_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.device_);
  }
  if (from.imei().size() > 0) {

    imei_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.imei_);
  }
  if (from.device_id().size() > 0) {

    device_id_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.device_id_);
  }
  if (from.language_type() != 0) {
    set_language_type(from.language_type());
  }
  if (from.net_type() != 0) {
    set_net_type(from.net_type());
  }
  if (from.client_type() != 0) {
    set_client_type(from.client_type());
  }
  if (from.terminal_device_type() != 0) {
    set_terminal_device_type(from.terminal_device_type());
  }
}

void MobileBase::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:common.base.MobileBase)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MobileBase::CopyFrom(const MobileBase& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:common.base.MobileBase)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MobileBase::IsInitialized() const {
  return true;
}

void MobileBase::Swap(MobileBase* other) {
  if (other == this) return;
  InternalSwap(other);
}
void MobileBase::InternalSwap(MobileBase* other) {
  using std::swap;
  sdk_version_.Swap(&other->sdk_version_);
  os_version_.Swap(&other->os_version_);
  device_.Swap(&other->device_);
  imei_.Swap(&other->imei_);
  device_id_.Swap(&other->device_id_);
  swap(language_type_, other->language_type_);
  swap(net_type_, other->net_type_);
  swap(client_type_, other->client_type_);
  swap(terminal_device_type_, other->terminal_device_type_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata MobileBase::GetMetadata() const {
  protobuf_common_2fbase_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_common_2fbase_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace base
}  // namespace common

// @@protoc_insertion_point(global_scope)
