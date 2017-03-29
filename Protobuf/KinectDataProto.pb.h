// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: KinectDataProto.proto

#ifndef PROTOBUF_KinectDataProto_2eproto__INCLUDED
#define PROTOBUF_KinectDataProto_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace KinectDataProto {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_KinectDataProto_2eproto();
void protobuf_AssignDesc_KinectDataProto_2eproto();
void protobuf_ShutdownFile_KinectDataProto_2eproto();

class pbEndTransfer;
class pbReqEnd;
class pbReqStart;
class pbRespStart;

// ===================================================================

class pbReqStart : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:KinectDataProto.pbReqStart) */ {
 public:
  pbReqStart();
  virtual ~pbReqStart();

  pbReqStart(const pbReqStart& from);

  inline pbReqStart& operator=(const pbReqStart& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const pbReqStart& default_instance();

  void Swap(pbReqStart* other);

  // implements Message ----------------------------------------------

  inline pbReqStart* New() const { return New(NULL); }

  pbReqStart* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const pbReqStart& from);
  void MergeFrom(const pbReqStart& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(pbReqStart* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string deviceName = 1;
  void clear_devicename();
  static const int kDeviceNameFieldNumber = 1;
  const ::std::string& devicename() const;
  void set_devicename(const ::std::string& value);
  void set_devicename(const char* value);
  void set_devicename(const char* value, size_t size);
  ::std::string* mutable_devicename();
  ::std::string* release_devicename();
  void set_allocated_devicename(::std::string* devicename);

  // optional int32 color = 2;
  void clear_color();
  static const int kColorFieldNumber = 2;
  ::google::protobuf::int32 color() const;
  void set_color(::google::protobuf::int32 value);

  // optional int32 depth = 3;
  void clear_depth();
  static const int kDepthFieldNumber = 3;
  ::google::protobuf::int32 depth() const;
  void set_depth(::google::protobuf::int32 value);

  // optional int32 skele = 4;
  void clear_skele();
  static const int kSkeleFieldNumber = 4;
  ::google::protobuf::int32 skele() const;
  void set_skele(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:KinectDataProto.pbReqStart)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr devicename_;
  ::google::protobuf::int32 color_;
  ::google::protobuf::int32 depth_;
  ::google::protobuf::int32 skele_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_KinectDataProto_2eproto();
  friend void protobuf_AssignDesc_KinectDataProto_2eproto();
  friend void protobuf_ShutdownFile_KinectDataProto_2eproto();

  void InitAsDefaultInstance();
  static pbReqStart* default_instance_;
};
// -------------------------------------------------------------------

class pbRespStart : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:KinectDataProto.pbRespStart) */ {
 public:
  pbRespStart();
  virtual ~pbRespStart();

  pbRespStart(const pbRespStart& from);

  inline pbRespStart& operator=(const pbRespStart& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const pbRespStart& default_instance();

  void Swap(pbRespStart* other);

  // implements Message ----------------------------------------------

  inline pbRespStart* New() const { return New(NULL); }

  pbRespStart* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const pbRespStart& from);
  void MergeFrom(const pbRespStart& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(pbRespStart* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string deviceName = 1;
  void clear_devicename();
  static const int kDeviceNameFieldNumber = 1;
  const ::std::string& devicename() const;
  void set_devicename(const ::std::string& value);
  void set_devicename(const char* value);
  void set_devicename(const char* value, size_t size);
  ::std::string* mutable_devicename();
  ::std::string* release_devicename();
  void set_allocated_devicename(::std::string* devicename);

  // optional string guid = 2;
  void clear_guid();
  static const int kGuidFieldNumber = 2;
  const ::std::string& guid() const;
  void set_guid(const ::std::string& value);
  void set_guid(const char* value);
  void set_guid(const char* value, size_t size);
  ::std::string* mutable_guid();
  ::std::string* release_guid();
  void set_allocated_guid(::std::string* guid);

  // optional int32 resultType = 3;
  void clear_resulttype();
  static const int kResultTypeFieldNumber = 3;
  ::google::protobuf::int32 resulttype() const;
  void set_resulttype(::google::protobuf::int32 value);

  // optional string failReason = 4;
  void clear_failreason();
  static const int kFailReasonFieldNumber = 4;
  const ::std::string& failreason() const;
  void set_failreason(const ::std::string& value);
  void set_failreason(const char* value);
  void set_failreason(const char* value, size_t size);
  ::std::string* mutable_failreason();
  ::std::string* release_failreason();
  void set_allocated_failreason(::std::string* failreason);

  // optional int32 colorPort = 5;
  void clear_colorport();
  static const int kColorPortFieldNumber = 5;
  ::google::protobuf::int32 colorport() const;
  void set_colorport(::google::protobuf::int32 value);

  // optional int32 depthPort = 6;
  void clear_depthport();
  static const int kDepthPortFieldNumber = 6;
  ::google::protobuf::int32 depthport() const;
  void set_depthport(::google::protobuf::int32 value);

  // optional int32 skelePort = 7;
  void clear_skeleport();
  static const int kSkelePortFieldNumber = 7;
  ::google::protobuf::int32 skeleport() const;
  void set_skeleport(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:KinectDataProto.pbRespStart)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr devicename_;
  ::google::protobuf::internal::ArenaStringPtr guid_;
  ::google::protobuf::internal::ArenaStringPtr failreason_;
  ::google::protobuf::int32 resulttype_;
  ::google::protobuf::int32 colorport_;
  ::google::protobuf::int32 depthport_;
  ::google::protobuf::int32 skeleport_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_KinectDataProto_2eproto();
  friend void protobuf_AssignDesc_KinectDataProto_2eproto();
  friend void protobuf_ShutdownFile_KinectDataProto_2eproto();

  void InitAsDefaultInstance();
  static pbRespStart* default_instance_;
};
// -------------------------------------------------------------------

class pbEndTransfer : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:KinectDataProto.pbEndTransfer) */ {
 public:
  pbEndTransfer();
  virtual ~pbEndTransfer();

  pbEndTransfer(const pbEndTransfer& from);

  inline pbEndTransfer& operator=(const pbEndTransfer& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const pbEndTransfer& default_instance();

  void Swap(pbEndTransfer* other);

  // implements Message ----------------------------------------------

  inline pbEndTransfer* New() const { return New(NULL); }

  pbEndTransfer* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const pbEndTransfer& from);
  void MergeFrom(const pbEndTransfer& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(pbEndTransfer* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string deviceName = 1;
  void clear_devicename();
  static const int kDeviceNameFieldNumber = 1;
  const ::std::string& devicename() const;
  void set_devicename(const ::std::string& value);
  void set_devicename(const char* value);
  void set_devicename(const char* value, size_t size);
  ::std::string* mutable_devicename();
  ::std::string* release_devicename();
  void set_allocated_devicename(::std::string* devicename);

  // optional string extension = 2;
  void clear_extension();
  static const int kExtensionFieldNumber = 2;
  const ::std::string& extension() const;
  void set_extension(const ::std::string& value);
  void set_extension(const char* value);
  void set_extension(const char* value, size_t size);
  ::std::string* mutable_extension();
  ::std::string* release_extension();
  void set_allocated_extension(::std::string* extension);

  // @@protoc_insertion_point(class_scope:KinectDataProto.pbEndTransfer)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr devicename_;
  ::google::protobuf::internal::ArenaStringPtr extension_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_KinectDataProto_2eproto();
  friend void protobuf_AssignDesc_KinectDataProto_2eproto();
  friend void protobuf_ShutdownFile_KinectDataProto_2eproto();

  void InitAsDefaultInstance();
  static pbEndTransfer* default_instance_;
};
// -------------------------------------------------------------------

class pbReqEnd : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:KinectDataProto.pbReqEnd) */ {
 public:
  pbReqEnd();
  virtual ~pbReqEnd();

  pbReqEnd(const pbReqEnd& from);

  inline pbReqEnd& operator=(const pbReqEnd& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const pbReqEnd& default_instance();

  void Swap(pbReqEnd* other);

  // implements Message ----------------------------------------------

  inline pbReqEnd* New() const { return New(NULL); }

  pbReqEnd* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const pbReqEnd& from);
  void MergeFrom(const pbReqEnd& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(pbReqEnd* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string deviceName = 1;
  void clear_devicename();
  static const int kDeviceNameFieldNumber = 1;
  const ::std::string& devicename() const;
  void set_devicename(const ::std::string& value);
  void set_devicename(const char* value);
  void set_devicename(const char* value, size_t size);
  ::std::string* mutable_devicename();
  ::std::string* release_devicename();
  void set_allocated_devicename(::std::string* devicename);

  // optional string extension = 2;
  void clear_extension();
  static const int kExtensionFieldNumber = 2;
  const ::std::string& extension() const;
  void set_extension(const ::std::string& value);
  void set_extension(const char* value);
  void set_extension(const char* value, size_t size);
  ::std::string* mutable_extension();
  ::std::string* release_extension();
  void set_allocated_extension(::std::string* extension);

  // @@protoc_insertion_point(class_scope:KinectDataProto.pbReqEnd)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr devicename_;
  ::google::protobuf::internal::ArenaStringPtr extension_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_KinectDataProto_2eproto();
  friend void protobuf_AssignDesc_KinectDataProto_2eproto();
  friend void protobuf_ShutdownFile_KinectDataProto_2eproto();

  void InitAsDefaultInstance();
  static pbReqEnd* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// pbReqStart

// optional string deviceName = 1;
inline void pbReqStart::clear_devicename() {
  devicename_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& pbReqStart::devicename() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbReqStart.deviceName)
  return devicename_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbReqStart::set_devicename(const ::std::string& value) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:KinectDataProto.pbReqStart.deviceName)
}
inline void pbReqStart::set_devicename(const char* value) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:KinectDataProto.pbReqStart.deviceName)
}
inline void pbReqStart::set_devicename(const char* value, size_t size) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:KinectDataProto.pbReqStart.deviceName)
}
inline ::std::string* pbReqStart::mutable_devicename() {
  
  // @@protoc_insertion_point(field_mutable:KinectDataProto.pbReqStart.deviceName)
  return devicename_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* pbReqStart::release_devicename() {
  // @@protoc_insertion_point(field_release:KinectDataProto.pbReqStart.deviceName)
  
  return devicename_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbReqStart::set_allocated_devicename(::std::string* devicename) {
  if (devicename != NULL) {
    
  } else {
    
  }
  devicename_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), devicename);
  // @@protoc_insertion_point(field_set_allocated:KinectDataProto.pbReqStart.deviceName)
}

// optional int32 color = 2;
inline void pbReqStart::clear_color() {
  color_ = 0;
}
inline ::google::protobuf::int32 pbReqStart::color() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbReqStart.color)
  return color_;
}
inline void pbReqStart::set_color(::google::protobuf::int32 value) {
  
  color_ = value;
  // @@protoc_insertion_point(field_set:KinectDataProto.pbReqStart.color)
}

// optional int32 depth = 3;
inline void pbReqStart::clear_depth() {
  depth_ = 0;
}
inline ::google::protobuf::int32 pbReqStart::depth() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbReqStart.depth)
  return depth_;
}
inline void pbReqStart::set_depth(::google::protobuf::int32 value) {
  
  depth_ = value;
  // @@protoc_insertion_point(field_set:KinectDataProto.pbReqStart.depth)
}

// optional int32 skele = 4;
inline void pbReqStart::clear_skele() {
  skele_ = 0;
}
inline ::google::protobuf::int32 pbReqStart::skele() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbReqStart.skele)
  return skele_;
}
inline void pbReqStart::set_skele(::google::protobuf::int32 value) {
  
  skele_ = value;
  // @@protoc_insertion_point(field_set:KinectDataProto.pbReqStart.skele)
}

// -------------------------------------------------------------------

// pbRespStart

// optional string deviceName = 1;
inline void pbRespStart::clear_devicename() {
  devicename_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& pbRespStart::devicename() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbRespStart.deviceName)
  return devicename_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbRespStart::set_devicename(const ::std::string& value) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:KinectDataProto.pbRespStart.deviceName)
}
inline void pbRespStart::set_devicename(const char* value) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:KinectDataProto.pbRespStart.deviceName)
}
inline void pbRespStart::set_devicename(const char* value, size_t size) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:KinectDataProto.pbRespStart.deviceName)
}
inline ::std::string* pbRespStart::mutable_devicename() {
  
  // @@protoc_insertion_point(field_mutable:KinectDataProto.pbRespStart.deviceName)
  return devicename_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* pbRespStart::release_devicename() {
  // @@protoc_insertion_point(field_release:KinectDataProto.pbRespStart.deviceName)
  
  return devicename_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbRespStart::set_allocated_devicename(::std::string* devicename) {
  if (devicename != NULL) {
    
  } else {
    
  }
  devicename_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), devicename);
  // @@protoc_insertion_point(field_set_allocated:KinectDataProto.pbRespStart.deviceName)
}

// optional string guid = 2;
inline void pbRespStart::clear_guid() {
  guid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& pbRespStart::guid() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbRespStart.guid)
  return guid_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbRespStart::set_guid(const ::std::string& value) {
  
  guid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:KinectDataProto.pbRespStart.guid)
}
inline void pbRespStart::set_guid(const char* value) {
  
  guid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:KinectDataProto.pbRespStart.guid)
}
inline void pbRespStart::set_guid(const char* value, size_t size) {
  
  guid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:KinectDataProto.pbRespStart.guid)
}
inline ::std::string* pbRespStart::mutable_guid() {
  
  // @@protoc_insertion_point(field_mutable:KinectDataProto.pbRespStart.guid)
  return guid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* pbRespStart::release_guid() {
  // @@protoc_insertion_point(field_release:KinectDataProto.pbRespStart.guid)
  
  return guid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbRespStart::set_allocated_guid(::std::string* guid) {
  if (guid != NULL) {
    
  } else {
    
  }
  guid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), guid);
  // @@protoc_insertion_point(field_set_allocated:KinectDataProto.pbRespStart.guid)
}

// optional int32 resultType = 3;
inline void pbRespStart::clear_resulttype() {
  resulttype_ = 0;
}
inline ::google::protobuf::int32 pbRespStart::resulttype() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbRespStart.resultType)
  return resulttype_;
}
inline void pbRespStart::set_resulttype(::google::protobuf::int32 value) {
  
  resulttype_ = value;
  // @@protoc_insertion_point(field_set:KinectDataProto.pbRespStart.resultType)
}

// optional string failReason = 4;
inline void pbRespStart::clear_failreason() {
  failreason_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& pbRespStart::failreason() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbRespStart.failReason)
  return failreason_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbRespStart::set_failreason(const ::std::string& value) {
  
  failreason_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:KinectDataProto.pbRespStart.failReason)
}
inline void pbRespStart::set_failreason(const char* value) {
  
  failreason_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:KinectDataProto.pbRespStart.failReason)
}
inline void pbRespStart::set_failreason(const char* value, size_t size) {
  
  failreason_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:KinectDataProto.pbRespStart.failReason)
}
inline ::std::string* pbRespStart::mutable_failreason() {
  
  // @@protoc_insertion_point(field_mutable:KinectDataProto.pbRespStart.failReason)
  return failreason_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* pbRespStart::release_failreason() {
  // @@protoc_insertion_point(field_release:KinectDataProto.pbRespStart.failReason)
  
  return failreason_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbRespStart::set_allocated_failreason(::std::string* failreason) {
  if (failreason != NULL) {
    
  } else {
    
  }
  failreason_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), failreason);
  // @@protoc_insertion_point(field_set_allocated:KinectDataProto.pbRespStart.failReason)
}

// optional int32 colorPort = 5;
inline void pbRespStart::clear_colorport() {
  colorport_ = 0;
}
inline ::google::protobuf::int32 pbRespStart::colorport() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbRespStart.colorPort)
  return colorport_;
}
inline void pbRespStart::set_colorport(::google::protobuf::int32 value) {
  
  colorport_ = value;
  // @@protoc_insertion_point(field_set:KinectDataProto.pbRespStart.colorPort)
}

// optional int32 depthPort = 6;
inline void pbRespStart::clear_depthport() {
  depthport_ = 0;
}
inline ::google::protobuf::int32 pbRespStart::depthport() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbRespStart.depthPort)
  return depthport_;
}
inline void pbRespStart::set_depthport(::google::protobuf::int32 value) {
  
  depthport_ = value;
  // @@protoc_insertion_point(field_set:KinectDataProto.pbRespStart.depthPort)
}

// optional int32 skelePort = 7;
inline void pbRespStart::clear_skeleport() {
  skeleport_ = 0;
}
inline ::google::protobuf::int32 pbRespStart::skeleport() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbRespStart.skelePort)
  return skeleport_;
}
inline void pbRespStart::set_skeleport(::google::protobuf::int32 value) {
  
  skeleport_ = value;
  // @@protoc_insertion_point(field_set:KinectDataProto.pbRespStart.skelePort)
}

// -------------------------------------------------------------------

// pbEndTransfer

// optional string deviceName = 1;
inline void pbEndTransfer::clear_devicename() {
  devicename_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& pbEndTransfer::devicename() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbEndTransfer.deviceName)
  return devicename_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbEndTransfer::set_devicename(const ::std::string& value) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:KinectDataProto.pbEndTransfer.deviceName)
}
inline void pbEndTransfer::set_devicename(const char* value) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:KinectDataProto.pbEndTransfer.deviceName)
}
inline void pbEndTransfer::set_devicename(const char* value, size_t size) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:KinectDataProto.pbEndTransfer.deviceName)
}
inline ::std::string* pbEndTransfer::mutable_devicename() {
  
  // @@protoc_insertion_point(field_mutable:KinectDataProto.pbEndTransfer.deviceName)
  return devicename_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* pbEndTransfer::release_devicename() {
  // @@protoc_insertion_point(field_release:KinectDataProto.pbEndTransfer.deviceName)
  
  return devicename_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbEndTransfer::set_allocated_devicename(::std::string* devicename) {
  if (devicename != NULL) {
    
  } else {
    
  }
  devicename_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), devicename);
  // @@protoc_insertion_point(field_set_allocated:KinectDataProto.pbEndTransfer.deviceName)
}

// optional string extension = 2;
inline void pbEndTransfer::clear_extension() {
  extension_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& pbEndTransfer::extension() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbEndTransfer.extension)
  return extension_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbEndTransfer::set_extension(const ::std::string& value) {
  
  extension_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:KinectDataProto.pbEndTransfer.extension)
}
inline void pbEndTransfer::set_extension(const char* value) {
  
  extension_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:KinectDataProto.pbEndTransfer.extension)
}
inline void pbEndTransfer::set_extension(const char* value, size_t size) {
  
  extension_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:KinectDataProto.pbEndTransfer.extension)
}
inline ::std::string* pbEndTransfer::mutable_extension() {
  
  // @@protoc_insertion_point(field_mutable:KinectDataProto.pbEndTransfer.extension)
  return extension_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* pbEndTransfer::release_extension() {
  // @@protoc_insertion_point(field_release:KinectDataProto.pbEndTransfer.extension)
  
  return extension_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbEndTransfer::set_allocated_extension(::std::string* extension) {
  if (extension != NULL) {
    
  } else {
    
  }
  extension_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), extension);
  // @@protoc_insertion_point(field_set_allocated:KinectDataProto.pbEndTransfer.extension)
}

// -------------------------------------------------------------------

// pbReqEnd

// optional string deviceName = 1;
inline void pbReqEnd::clear_devicename() {
  devicename_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& pbReqEnd::devicename() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbReqEnd.deviceName)
  return devicename_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbReqEnd::set_devicename(const ::std::string& value) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:KinectDataProto.pbReqEnd.deviceName)
}
inline void pbReqEnd::set_devicename(const char* value) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:KinectDataProto.pbReqEnd.deviceName)
}
inline void pbReqEnd::set_devicename(const char* value, size_t size) {
  
  devicename_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:KinectDataProto.pbReqEnd.deviceName)
}
inline ::std::string* pbReqEnd::mutable_devicename() {
  
  // @@protoc_insertion_point(field_mutable:KinectDataProto.pbReqEnd.deviceName)
  return devicename_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* pbReqEnd::release_devicename() {
  // @@protoc_insertion_point(field_release:KinectDataProto.pbReqEnd.deviceName)
  
  return devicename_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbReqEnd::set_allocated_devicename(::std::string* devicename) {
  if (devicename != NULL) {
    
  } else {
    
  }
  devicename_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), devicename);
  // @@protoc_insertion_point(field_set_allocated:KinectDataProto.pbReqEnd.deviceName)
}

// optional string extension = 2;
inline void pbReqEnd::clear_extension() {
  extension_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& pbReqEnd::extension() const {
  // @@protoc_insertion_point(field_get:KinectDataProto.pbReqEnd.extension)
  return extension_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbReqEnd::set_extension(const ::std::string& value) {
  
  extension_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:KinectDataProto.pbReqEnd.extension)
}
inline void pbReqEnd::set_extension(const char* value) {
  
  extension_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:KinectDataProto.pbReqEnd.extension)
}
inline void pbReqEnd::set_extension(const char* value, size_t size) {
  
  extension_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:KinectDataProto.pbReqEnd.extension)
}
inline ::std::string* pbReqEnd::mutable_extension() {
  
  // @@protoc_insertion_point(field_mutable:KinectDataProto.pbReqEnd.extension)
  return extension_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* pbReqEnd::release_extension() {
  // @@protoc_insertion_point(field_release:KinectDataProto.pbReqEnd.extension)
  
  return extension_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void pbReqEnd::set_allocated_extension(::std::string* extension) {
  if (extension != NULL) {
    
  } else {
    
  }
  extension_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), extension);
  // @@protoc_insertion_point(field_set_allocated:KinectDataProto.pbReqEnd.extension)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace KinectDataProto

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_KinectDataProto_2eproto__INCLUDED
