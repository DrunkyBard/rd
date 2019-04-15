#ifndef RD_CPP_ABSTRACTPOLYMORPHIC_H
#define RD_CPP_ABSTRACTPOLYMORPHIC_H

#include "wrapper.h"
#include "Polymorphic.h"
#include "SerializationCtx.h"

namespace rd {
	template<typename T>
	class AbstractPolymorphic {
	public:
		static value_or_wrapper <T> read(SerializationCtx const &ctx, Buffer const &buffer) {
			return ctx.get_serializers().readPolymorphicNullable<T>(ctx, buffer);
		}

		static void write(SerializationCtx const &ctx, Buffer const &buffer, T const &value) {
			ctx.get_serializers().writePolymorphicNullable(ctx, buffer, value);
		}

		static void write(SerializationCtx const &ctx, Buffer const &buffer, Wrapper <T> const &value) {
			ctx.get_serializers().writePolymorphicNullable(ctx, buffer, *value);
		}
	};
}

#endif //RD_CPP_ABSTRACTPOLYMORPHIC_H
