// Copyright (C) 2015, for Authors.
// Author: An Qin (anqin.qin@gmail.com)
//
// Created: 05/07/12

#ifndef TWISTER_IO_TEST_DATA_H
#define TWISTER_IO_TEST_DATA_H

#include <string>
#include <vector>

#include "common/base/scoped_ptr.h"
#include "common/crypto/random/pseudo_random.h"
#include "thirdparty/protobuf/descriptor.h"
#include "thirdparty/protobuf/message.h"

namespace twister {
namespace io {

class TestData {
public:
    typedef ::std::string string;
    typedef ::protobuf::Message Message;
    typedef ::protobuf::Reflection Reflection;
    typedef ::protobuf::Descriptor Descriptor;
    typedef ::protobuf::FieldDescriptor FieldDescriptor;
    typedef ::protobuf::EnumDescriptor EnumDescriptor;
    typedef ::protobuf::EnumValueDescriptor EnumValueDescriptor;

    TestData();
    ~TestData();

    void Reset(const Message& message);
    void Reset(const Message* message);
    void GenerateMessages(uint32_t message_number);
    const Message& operator[](uint32_t index) const;
    int size() const;

    void SetSeed(int64_t seed);
    void SetReadable(bool readable);
    void SetUniqueValueCount(int unique_value_count);
    void SetMaxRepeatedSize(int max_repeated_size);
    void SetMaxStringLength(int max_string_length);
    void SetFloatRange(int float_range);

private:
    void CreateMessage(Message* message);
    void AddValue(Message* message, const FieldDescriptor* field);

    bool RandomOptional();
    bool RandomRepeated();
    double RandomProb();

    int32_t RandomInt32();
    int64_t RandomInt64();
    uint32_t RandomUInt32();
    uint64_t RandomUInt64();
    double RandomDouble();
    float RandomFloat();
    bool RandomBool();
    const EnumValueDescriptor* RandomEnum(const FieldDescriptor* field);
    string RandomString();

private:
    bool m_readable;
    int m_unique_value_count;
    int m_max_repeated_size;
    int m_max_string_length;
    int m_float_range;
    scoped_ptr<PseudoRandom> m_random;
    scoped_ptr<Message> m_proto_type;
    std::vector<Message*> m_data;
};

} // namespace io
} // namespace twister

#endif  // TWISTER_IO_TEST_DATA_H
