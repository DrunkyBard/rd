#ifndef Derived_H
#define Derived_H

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

#include "Base.h"


#pragma warning( push )
#pragma warning( disable:4250 )
#pragma warning( disable:4307 )
namespace demo {
    class Derived : public Base
    {
        
        //companion
        
        //custom serializers
        private:
        
        //fields
        protected:
        rd::Wrapper<std::wstring> string_;
        
        
        //initializer
        private:
        void initialize();
        
        //primary ctor
        public:
        Derived(rd::Wrapper<std::wstring> string_);
        
        //secondary constructor
        
        //default ctors and dtors
        
        Derived();
        
        Derived(Derived const &) = default;
        
        Derived& operator=(Derived const &) = default;
        
        Derived(Derived &&) = default;
        
        Derived& operator=(Derived &&) = default;
        
        virtual ~Derived() = default;
        
        //reader
        static Derived read(rd::SerializationCtx const& ctx, rd::Buffer const & buffer);
        
        //writer
        void write(rd::SerializationCtx const& ctx, rd::Buffer const& buffer) const override;
        
        //virtual init
        
        //identify
        
        //getters
        std::wstring const & get_string() const;
        
        //intern
        
        //equals trait
        private:
        bool equals(rd::ISerializable const& object) const override;
        
        //equality operators
        public:
        friend bool operator==(const Derived &lhs, const Derived &rhs);
        friend bool operator!=(const Derived &lhs, const Derived &rhs);
        
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
    template <> struct hash<demo::Derived> {
        size_t operator()(const demo::Derived & value) const {
            return value.hashCode();
        }
    };
}

#endif // Derived_H
