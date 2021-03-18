// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: mk/gen/protoc/test.proto

#ifndef PROTOBUF_INCLUDED_mk_2fgen_2fprotoc_2ftest_2eproto
#define PROTOBUF_INCLUDED_mk_2fgen_2fprotoc_2ftest_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_mk_2fgen_2fprotoc_2ftest_2eproto 

namespace protobuf_mk_2fgen_2fprotoc_2ftest_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_mk_2fgen_2fprotoc_2ftest_2eproto
class Foo;
class FooDefaultTypeInternal;
extern FooDefaultTypeInternal _Foo_default_instance_;
namespace google {
namespace protobuf {
template<> ::Foo* Arena::CreateMaybeMessage<::Foo>(Arena*);
}  // namespace protobuf
}  // namespace google

enum Foo_Bar {
  Foo_Bar_Baz = 1
};
bool Foo_Bar_IsValid(int value);
const Foo_Bar Foo_Bar_Bar_MIN = Foo_Bar_Baz;
const Foo_Bar Foo_Bar_Bar_MAX = Foo_Bar_Baz;
const int Foo_Bar_Bar_ARRAYSIZE = Foo_Bar_Bar_MAX + 1;

const ::google::protobuf::EnumDescriptor* Foo_Bar_descriptor();
inline const ::std::string& Foo_Bar_Name(Foo_Bar value) {
  return ::google::protobuf::internal::NameOfEnum(
    Foo_Bar_descriptor(), value);
}
inline bool Foo_Bar_Parse(
    const ::std::string& name, Foo_Bar* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Foo_Bar>(
    Foo_Bar_descriptor(), name, value);
}
// ===================================================================

class Foo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Foo) */ {
 public:
  Foo();
  virtual ~Foo();

  Foo(const Foo& from);

  inline Foo& operator=(const Foo& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Foo(Foo&& from) noexcept
    : Foo() {
    *this = ::std::move(from);
  }

  inline Foo& operator=(Foo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Foo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Foo* internal_default_instance() {
    return reinterpret_cast<const Foo*>(
               &_Foo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Foo* other);
  friend void swap(Foo& a, Foo& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Foo* New() const final {
    return CreateMaybeMessage<Foo>(NULL);
  }

  Foo* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Foo>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Foo& from);
  void MergeFrom(const Foo& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Foo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef Foo_Bar Bar;
  static const Bar Baz =
    Foo_Bar_Baz;
  static inline bool Bar_IsValid(int value) {
    return Foo_Bar_IsValid(value);
  }
  static const Bar Bar_MIN =
    Foo_Bar_Bar_MIN;
  static const Bar Bar_MAX =
    Foo_Bar_Bar_MAX;
  static const int Bar_ARRAYSIZE =
    Foo_Bar_Bar_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Bar_descriptor() {
    return Foo_Bar_descriptor();
  }
  static inline const ::std::string& Bar_Name(Bar value) {
    return Foo_Bar_Name(value);
  }
  static inline bool Bar_Parse(const ::std::string& name,
      Bar* value) {
    return Foo_Bar_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:Foo)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_mk_2fgen_2fprotoc_2ftest_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Foo

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::Foo_Bar> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Foo_Bar>() {
  return ::Foo_Bar_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_mk_2fgen_2fprotoc_2ftest_2eproto
