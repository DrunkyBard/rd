#ifndef MyScalar_H
#define MyScalar_H

#include "Protocol.h"
#include "RdList.h"
#include "RdMap.h"
#include "RdProperty.h"
#include "RdSet.h"
#include "RdSignal.h"
#include "RdExtBase.h"
#include "RdCall.h"
#include "RdEndpoint.h"
#include "gen_util.h"

#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <type_traits>
#include <utility>

#include "optional.hpp"


#pragma warning( push )
#pragma warning( disable:4250 )
#pragma warning( disable:4307 )
namespace demo {
    
    //data
    class MyScalar : public rd::IPolymorphicSerializable
    {
        
        //companion
        
        //custom serializers
        private:
        
        //fields
        protected:
        bool sign_;
        signed char byte_;
        short short_;
        int32_t int_;
        int64_t long_;
        float float__;
        double double__;
        
        
        //initializer
        private:
        void initialize();
        
        //primary ctor
        public:
        MyScalar(bool sign_, signed char byte_, short short_, int32_t int_, int64_t long_, float float__, double double__);
        
        //secondary constructor
        
        //default ctors and dtors
        
        MyScalar();
        
        MyScalar(MyScalar const &) = default;
        
        MyScalar& operator=(MyScalar const &) = default;
        
        MyScalar(MyScalar &&) = default;
        
        MyScalar& operator=(MyScalar &&) = default;
        
        virtual ~MyScalar() = default;
        
        //reader
        static MyScalar read(rd::SerializationCtx const& ctx, rd::Buffer const & buffer);
        
        //writer
        void write(rd::SerializationCtx const& ctx, rd::Buffer const& buffer) const override;
        
        //virtual init
        
        //identify
        
        //getters
        bool const & get_sign() const;
        signed char const & get_byte() const;
        short const & get_short() const;
        int32_t const & get_int() const;
        int64_t const & get_long() const;
        float const & get_float_() const;
        double const & get_double_() const;
        
        //intern
        
        //equals trait
        private:
        bool equals(rd::ISerializable const& object) const override;
        
        //equality operators
        public:
        friend bool operator==(const MyScalar &lhs, const MyScalar &rhs);
        friend bool operator!=(const MyScalar &lhs, const MyScalar &rhs);
        
        //hash code trait
        size_t hashCode() const override;
        
        //type name trait
        std::string type_name() const override;
        
        //static type name trait
        static std::string static_type_name();
    };
};

#pragma warning( pop )


//hash code trait
namespace std {
    template <> struct hash<demo::MyScalar> {
        size_t operator()(const demo::MyScalar & value) const {
            return value.hashCode();
        }
    };
}

#endif // MyScalar_H
