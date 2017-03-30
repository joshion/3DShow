// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ConnectProto.proto

#ifndef PROTOBUF_ConnectProto_2eproto__INCLUDED
#define PROTOBUF_ConnectProto_2eproto__INCLUDED

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

namespace ConnectProto {
    // Internal implementation detail -- do not call these.
    void protobuf_AddDesc_ConnectProto_2eproto();
    void protobuf_AssignDesc_ConnectProto_2eproto();
    void protobuf_ShutdownFile_ConnectProto_2eproto();

    class pbRespConnect;
    class pbRespDevices;

    // ===================================================================

    class pbRespConnect : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ConnectProto.pbRespConnect) */ {
    public:
        pbRespConnect();
        virtual ~pbRespConnect();

        pbRespConnect(const pbRespConnect& from);

        inline pbRespConnect& operator=(const pbRespConnect& from) {
            CopyFrom(from);
            return *this;
        }

        static const ::google::protobuf::Descriptor* descriptor();
        static const pbRespConnect& default_instance();

        void Swap(pbRespConnect* other);

        // implements Message ----------------------------------------------

        inline pbRespConnect* New() const {
            return New(NULL);
        }

        pbRespConnect* New(::google::protobuf::Arena* arena) const;
        void CopyFrom(const ::google::protobuf::Message& from);
        void MergeFrom(const ::google::protobuf::Message& from);
        void CopyFrom(const pbRespConnect& from);
        void MergeFrom(const pbRespConnect& from);
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
        int GetCachedSize() const {
            return _cached_size_;
        }
    private:
        void SharedCtor();
        void SharedDtor();
        void SetCachedSize(int size) const;
        void InternalSwap(pbRespConnect* other);
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

        // optional int32 resultType = 1;
        void clear_resulttype();
        static const int kResultTypeFieldNumber = 1;
        ::google::protobuf::int32 resulttype() const;
        void set_resulttype(::google::protobuf::int32 value);

        // optional string failReason = 2;
        void clear_failreason();
        static const int kFailReasonFieldNumber = 2;
        const ::std::string& failreason() const;
        void set_failreason(const ::std::string& value);
        void set_failreason(const char* value);
        void set_failreason(const char* value, size_t size);
        ::std::string* mutable_failreason();
        ::std::string* release_failreason();
        void set_allocated_failreason(::std::string* failreason);

        // optional string guid = 3;
        void clear_guid();
        static const int kGuidFieldNumber = 3;
        const ::std::string& guid() const;
        void set_guid(const ::std::string& value);
        void set_guid(const char* value);
        void set_guid(const char* value, size_t size);
        ::std::string* mutable_guid();
        ::std::string* release_guid();
        void set_allocated_guid(::std::string* guid);

        // optional int32 colorPort = 4;
        void clear_colorport();
        static const int kColorPortFieldNumber = 4;
        ::google::protobuf::int32 colorport() const;
        void set_colorport(::google::protobuf::int32 value);

        // optional int32 depthPort = 5;
        void clear_depthport();
        static const int kDepthPortFieldNumber = 5;
        ::google::protobuf::int32 depthport() const;
        void set_depthport(::google::protobuf::int32 value);

        // optional int32 skelePort = 6;
        void clear_skeleport();
        static const int kSkelePortFieldNumber = 6;
        ::google::protobuf::int32 skeleport() const;
        void set_skeleport(::google::protobuf::int32 value);

        // @@protoc_insertion_point(class_scope:ConnectProto.pbRespConnect)
    private:

        ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
        bool _is_default_instance_;
        ::google::protobuf::internal::ArenaStringPtr failreason_;
        ::google::protobuf::int32 resulttype_;
        ::google::protobuf::int32 colorport_;
        ::google::protobuf::internal::ArenaStringPtr guid_;
        ::google::protobuf::int32 depthport_;
        ::google::protobuf::int32 skeleport_;
        mutable int _cached_size_;
        friend void  protobuf_AddDesc_ConnectProto_2eproto();
        friend void protobuf_AssignDesc_ConnectProto_2eproto();
        friend void protobuf_ShutdownFile_ConnectProto_2eproto();

        void InitAsDefaultInstance();
        static pbRespConnect* default_instance_;
    };
    // -------------------------------------------------------------------

    class pbRespDevices : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:ConnectProto.pbRespDevices) */ {
    public:
        pbRespDevices();
        virtual ~pbRespDevices();

        pbRespDevices(const pbRespDevices& from);

        inline pbRespDevices& operator=(const pbRespDevices& from) {
            CopyFrom(from);
            return *this;
        }

        static const ::google::protobuf::Descriptor* descriptor();
        static const pbRespDevices& default_instance();

        void Swap(pbRespDevices* other);

        // implements Message ----------------------------------------------

        inline pbRespDevices* New() const {
            return New(NULL);
        }

        pbRespDevices* New(::google::protobuf::Arena* arena) const;
        void CopyFrom(const ::google::protobuf::Message& from);
        void MergeFrom(const ::google::protobuf::Message& from);
        void CopyFrom(const pbRespDevices& from);
        void MergeFrom(const pbRespDevices& from);
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
        int GetCachedSize() const {
            return _cached_size_;
        }
    private:
        void SharedCtor();
        void SharedDtor();
        void SetCachedSize(int size) const;
        void InternalSwap(pbRespDevices* other);
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

        // optional string deviceList = 1;
        void clear_devicelist();
        static const int kDeviceListFieldNumber = 1;
        const ::std::string& devicelist() const;
        void set_devicelist(const ::std::string& value);
        void set_devicelist(const char* value);
        void set_devicelist(const char* value, size_t size);
        ::std::string* mutable_devicelist();
        ::std::string* release_devicelist();
        void set_allocated_devicelist(::std::string* devicelist);

        // optional string separator = 2;
        void clear_separator();
        static const int kSeparatorFieldNumber = 2;
        const ::std::string& separator() const;
        void set_separator(const ::std::string& value);
        void set_separator(const char* value);
        void set_separator(const char* value, size_t size);
        ::std::string* mutable_separator();
        ::std::string* release_separator();
        void set_allocated_separator(::std::string* separator);

        // @@protoc_insertion_point(class_scope:ConnectProto.pbRespDevices)
    private:

        ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
        bool _is_default_instance_;
        ::google::protobuf::internal::ArenaStringPtr devicelist_;
        ::google::protobuf::internal::ArenaStringPtr separator_;
        mutable int _cached_size_;
        friend void  protobuf_AddDesc_ConnectProto_2eproto();
        friend void protobuf_AssignDesc_ConnectProto_2eproto();
        friend void protobuf_ShutdownFile_ConnectProto_2eproto();

        void InitAsDefaultInstance();
        static pbRespDevices* default_instance_;
    };
    // ===================================================================

    // ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// pbRespConnect

// optional int32 resultType = 1;
    inline void pbRespConnect::clear_resulttype() {
        resulttype_ = 0;
    }
    inline ::google::protobuf::int32 pbRespConnect::resulttype() const {
        // @@protoc_insertion_point(field_get:ConnectProto.pbRespConnect.resultType)
        return resulttype_;
    }
    inline void pbRespConnect::set_resulttype(::google::protobuf::int32 value) {
        resulttype_ = value;
        // @@protoc_insertion_point(field_set:ConnectProto.pbRespConnect.resultType)
    }

    // optional string failReason = 2;
    inline void pbRespConnect::clear_failreason() {
        failreason_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline const ::std::string& pbRespConnect::failreason() const {
        // @@protoc_insertion_point(field_get:ConnectProto.pbRespConnect.failReason)
        return failreason_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline void pbRespConnect::set_failreason(const ::std::string& value) {
        failreason_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
        // @@protoc_insertion_point(field_set:ConnectProto.pbRespConnect.failReason)
    }
    inline void pbRespConnect::set_failreason(const char* value) {
        failreason_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
        // @@protoc_insertion_point(field_set_char:ConnectProto.pbRespConnect.failReason)
    }
    inline void pbRespConnect::set_failreason(const char* value, size_t size) {
        failreason_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
            ::std::string(reinterpret_cast<const char*>(value), size));
        // @@protoc_insertion_point(field_set_pointer:ConnectProto.pbRespConnect.failReason)
    }
    inline ::std::string* pbRespConnect::mutable_failreason() {
        // @@protoc_insertion_point(field_mutable:ConnectProto.pbRespConnect.failReason)
        return failreason_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline ::std::string* pbRespConnect::release_failreason() {
        // @@protoc_insertion_point(field_release:ConnectProto.pbRespConnect.failReason)

        return failreason_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline void pbRespConnect::set_allocated_failreason(::std::string* failreason) {
        if (failreason != NULL) {
        }
        else {
        }
        failreason_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), failreason);
        // @@protoc_insertion_point(field_set_allocated:ConnectProto.pbRespConnect.failReason)
    }

    // optional string guid = 3;
    inline void pbRespConnect::clear_guid() {
        guid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline const ::std::string& pbRespConnect::guid() const {
        // @@protoc_insertion_point(field_get:ConnectProto.pbRespConnect.guid)
        return guid_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline void pbRespConnect::set_guid(const ::std::string& value) {
        guid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
        // @@protoc_insertion_point(field_set:ConnectProto.pbRespConnect.guid)
    }
    inline void pbRespConnect::set_guid(const char* value) {
        guid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
        // @@protoc_insertion_point(field_set_char:ConnectProto.pbRespConnect.guid)
    }
    inline void pbRespConnect::set_guid(const char* value, size_t size) {
        guid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
            ::std::string(reinterpret_cast<const char*>(value), size));
        // @@protoc_insertion_point(field_set_pointer:ConnectProto.pbRespConnect.guid)
    }
    inline ::std::string* pbRespConnect::mutable_guid() {
        // @@protoc_insertion_point(field_mutable:ConnectProto.pbRespConnect.guid)
        return guid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline ::std::string* pbRespConnect::release_guid() {
        // @@protoc_insertion_point(field_release:ConnectProto.pbRespConnect.guid)

        return guid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline void pbRespConnect::set_allocated_guid(::std::string* guid) {
        if (guid != NULL) {
        }
        else {
        }
        guid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), guid);
        // @@protoc_insertion_point(field_set_allocated:ConnectProto.pbRespConnect.guid)
    }

    // optional int32 colorPort = 4;
    inline void pbRespConnect::clear_colorport() {
        colorport_ = 0;
    }
    inline ::google::protobuf::int32 pbRespConnect::colorport() const {
        // @@protoc_insertion_point(field_get:ConnectProto.pbRespConnect.colorPort)
        return colorport_;
    }
    inline void pbRespConnect::set_colorport(::google::protobuf::int32 value) {
        colorport_ = value;
        // @@protoc_insertion_point(field_set:ConnectProto.pbRespConnect.colorPort)
    }

    // optional int32 depthPort = 5;
    inline void pbRespConnect::clear_depthport() {
        depthport_ = 0;
    }
    inline ::google::protobuf::int32 pbRespConnect::depthport() const {
        // @@protoc_insertion_point(field_get:ConnectProto.pbRespConnect.depthPort)
        return depthport_;
    }
    inline void pbRespConnect::set_depthport(::google::protobuf::int32 value) {
        depthport_ = value;
        // @@protoc_insertion_point(field_set:ConnectProto.pbRespConnect.depthPort)
    }

    // optional int32 skelePort = 6;
    inline void pbRespConnect::clear_skeleport() {
        skeleport_ = 0;
    }
    inline ::google::protobuf::int32 pbRespConnect::skeleport() const {
        // @@protoc_insertion_point(field_get:ConnectProto.pbRespConnect.skelePort)
        return skeleport_;
    }
    inline void pbRespConnect::set_skeleport(::google::protobuf::int32 value) {
        skeleport_ = value;
        // @@protoc_insertion_point(field_set:ConnectProto.pbRespConnect.skelePort)
    }

    // -------------------------------------------------------------------

    // pbRespDevices

    // optional string deviceList = 1;
    inline void pbRespDevices::clear_devicelist() {
        devicelist_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline const ::std::string& pbRespDevices::devicelist() const {
        // @@protoc_insertion_point(field_get:ConnectProto.pbRespDevices.deviceList)
        return devicelist_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline void pbRespDevices::set_devicelist(const ::std::string& value) {
        devicelist_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
        // @@protoc_insertion_point(field_set:ConnectProto.pbRespDevices.deviceList)
    }
    inline void pbRespDevices::set_devicelist(const char* value) {
        devicelist_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
        // @@protoc_insertion_point(field_set_char:ConnectProto.pbRespDevices.deviceList)
    }
    inline void pbRespDevices::set_devicelist(const char* value, size_t size) {
        devicelist_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
            ::std::string(reinterpret_cast<const char*>(value), size));
        // @@protoc_insertion_point(field_set_pointer:ConnectProto.pbRespDevices.deviceList)
    }
    inline ::std::string* pbRespDevices::mutable_devicelist() {
        // @@protoc_insertion_point(field_mutable:ConnectProto.pbRespDevices.deviceList)
        return devicelist_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline ::std::string* pbRespDevices::release_devicelist() {
        // @@protoc_insertion_point(field_release:ConnectProto.pbRespDevices.deviceList)

        return devicelist_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline void pbRespDevices::set_allocated_devicelist(::std::string* devicelist) {
        if (devicelist != NULL) {
        }
        else {
        }
        devicelist_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), devicelist);
        // @@protoc_insertion_point(field_set_allocated:ConnectProto.pbRespDevices.deviceList)
    }

    // optional string separator = 2;
    inline void pbRespDevices::clear_separator() {
        separator_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline const ::std::string& pbRespDevices::separator() const {
        // @@protoc_insertion_point(field_get:ConnectProto.pbRespDevices.separator)
        return separator_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline void pbRespDevices::set_separator(const ::std::string& value) {
        separator_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
        // @@protoc_insertion_point(field_set:ConnectProto.pbRespDevices.separator)
    }
    inline void pbRespDevices::set_separator(const char* value) {
        separator_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
        // @@protoc_insertion_point(field_set_char:ConnectProto.pbRespDevices.separator)
    }
    inline void pbRespDevices::set_separator(const char* value, size_t size) {
        separator_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
            ::std::string(reinterpret_cast<const char*>(value), size));
        // @@protoc_insertion_point(field_set_pointer:ConnectProto.pbRespDevices.separator)
    }
    inline ::std::string* pbRespDevices::mutable_separator() {
        // @@protoc_insertion_point(field_mutable:ConnectProto.pbRespDevices.separator)
        return separator_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline ::std::string* pbRespDevices::release_separator() {
        // @@protoc_insertion_point(field_release:ConnectProto.pbRespDevices.separator)

        return separator_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    }
    inline void pbRespDevices::set_allocated_separator(::std::string* separator) {
        if (separator != NULL) {
        }
        else {
        }
        separator_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), separator);
        // @@protoc_insertion_point(field_set_allocated:ConnectProto.pbRespDevices.separator)
    }

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
    // -------------------------------------------------------------------

    // @@protoc_insertion_point(namespace_scope)
}  // namespace ConnectProto

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ConnectProto_2eproto__INCLUDED
