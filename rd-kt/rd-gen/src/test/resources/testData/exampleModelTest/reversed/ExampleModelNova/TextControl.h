//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a RdGen v1.10.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
#ifndef TEXTCONTROL_GENERATED_H
#define TEXTCONTROL_GENERATED_H


#include "protocol/Protocol.h"
#include "types/DateTime.h"
#include "impl/RdSignal.h"
#include "impl/RdProperty.h"
#include "impl/RdList.h"
#include "impl/RdSet.h"
#include "impl/RdMap.h"
#include "base/ISerializersOwner.h"
#include "base/IUnknownInstance.h"
#include "serialization/ISerializable.h"
#include "serialization/Polymorphic.h"
#include "serialization/NullableSerializer.h"
#include "serialization/ArraySerializer.h"
#include "serialization/InternedSerializer.h"
#include "serialization/SerializationCtx.h"
#include "serialization/Serializers.h"
#include "ext/RdExtBase.h"
#include "task/RdCall.h"
#include "task/RdEndpoint.h"
#include "task/RdSymmetricCall.h"
#include "std/to_string.h"
#include "std/hash.h"
#include "std/allocator.h"
#include "util/enum.h"
#include "util/gen_util.h"

#include <cstring>
#include <cstdint>
#include <vector>
#include <ctime>

#include "thirdparty.hpp"
#include "instantiations_ExampleRootNova.h"

#include "ExampleModelNova/Selection.Generated.h"



#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable:4250 )
#pragma warning( disable:4307 )
#pragma warning( disable:4267 )
#pragma warning( disable:4244 )
#pragma warning( disable:4100 )
#endif

/// <summary>
/// <p>Generated from: Example.kt:101</p>
/// </summary>
namespace org.example {

class TextControl : public rd::IPolymorphicSerializable, public rd::RdBindableBase {

private:
    // custom serializers

public:
    // constants

protected:
    // fields
    rd::RdProperty<Selection, rd::Polymorphic<Selection>> selection_;
    rd::RdSignal<rd::Void, rd::Polymorphic<rd::Void>> vsink_;
    rd::RdSignal<rd::Void, rd::Polymorphic<rd::Void>> vsource_;
    rd::RdEndpoint<int32_t, std::wstring, rd::Polymorphic<int32_t>, rd::Polymorphic<std::wstring>> there1_;
    

private:
    // initializer
    void initialize();

public:
    // primary ctor
    TextControl(rd::RdProperty<Selection, rd::Polymorphic<Selection>> selection_, rd::RdSignal<rd::Void, rd::Polymorphic<rd::Void>> vsink_, rd::RdSignal<rd::Void, rd::Polymorphic<rd::Void>> vsource_, rd::RdEndpoint<int32_t, std::wstring, rd::Polymorphic<int32_t>, rd::Polymorphic<std::wstring>> there1_);
    
    // default ctors and dtors
    
    TextControl();
    
    TextControl(TextControl &&) = default;
    
    TextControl& operator=(TextControl &&) = default;
    
    virtual ~TextControl() = default;
    
    // reader
    static TextControl read(rd::SerializationCtx& ctx, rd::Buffer & buffer);
    
    // writer
    void write(rd::SerializationCtx& ctx, rd::Buffer& buffer) const override;
    
    // virtual init
    void init(rd::Lifetime lifetime) const override;
    
    // identify
    void identify(const rd::Identities &identities, rd::RdId const &id) const override;
    
    // getters
    rd::IProperty<Selection> const & get_selection() const;
    rd::ISignal<rd::Void> const & get_vsink() const;
    rd::ISource<rd::Void> const & get_vsource() const;
    rd::RdEndpoint<int32_t, std::wstring, rd::Polymorphic<int32_t>, rd::Polymorphic<std::wstring>> const & get_there1() const;
    
    // intern

private:
    // equals trait
    bool equals(rd::ISerializable const& object) const override;

public:
    // equality operators
    friend bool operator==(const TextControl &lhs, const TextControl &rhs);
    friend bool operator!=(const TextControl &lhs, const TextControl &rhs);
    // hash code trait
    // type name trait
    std::string type_name() const override;
    // static type name trait
    static std::string static_type_name();

private:
    // polymorphic to string
    std::string toString() const override;

public:
    // external to string
    friend std::string to_string(const TextControl & value);
};

}

#ifdef _MSC_VER
#pragma warning( pop )
#endif



#endif // TEXTCONTROL_GENERATED_H
