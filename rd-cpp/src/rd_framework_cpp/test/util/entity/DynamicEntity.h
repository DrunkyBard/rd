#ifndef RD_CPP_DYNAMICENTITY_H
#define RD_CPP_DYNAMICENTITY_H


#include "RdBindableBase.h"
#include "ISerializable.h"
#include "RdProperty.h"

namespace rd {
	namespace test {
		namespace util {
			class DynamicEntity : public RdBindableBase, public IPolymorphicSerializable {
			public:
				RdProperty <int32_t> foo;

				//region ctor/dtor

				//    DynamicEntity() = default;


				//region ctor/dtor

				DynamicEntity() = default;

				DynamicEntity(DynamicEntity const &other) = delete;

				DynamicEntity(DynamicEntity &&other) = default;

				DynamicEntity &operator=(DynamicEntity &&other) = default;

				virtual ~DynamicEntity() = default;
				//endregion

				explicit DynamicEntity(RdProperty <int32_t> &&foo) : foo(std::move(foo)) {}

				explicit DynamicEntity(int32_t value) : DynamicEntity(RdProperty<int32_t>(value)) {};
				//endregion

				static DynamicEntity read(SerializationCtx const &, Buffer const &buffer);

				void write(SerializationCtx const &ctx, Buffer const &buffer) const override;

				static void create(IProtocol *protocol);

				void init(Lifetime lifetime) const override;

				void identify(Identities const &identities, RdId const &id) const override;

				friend bool operator==(const DynamicEntity &lhs, const DynamicEntity &rhs);

				friend bool operator!=(const DynamicEntity &lhs, const DynamicEntity &rhs);

				size_t hashCode() const override;

				bool equals(const ISerializable &object) const override;

				std::string type_name() const override;

				static std::string static_type_name();
			};
		}
	}
}


#endif //RD_CPP_DYNAMICENTITY_H
