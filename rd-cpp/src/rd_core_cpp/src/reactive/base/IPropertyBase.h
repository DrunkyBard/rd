#ifndef RD_CPP_IPROPERTYBASE_H
#define RD_CPP_IPROPERTYBASE_H


#include "wrapper.h"
#include "interfaces.h"
#include "SignalX.h"
#include "SequentialLifetimes.h"

#include "thirdparty.hpp"

namespace rd {
	template<typename T>
	class IPropertyBase : public ISource<T>, public IViewable<T> {
	protected:
		mutable property_storage<T> value;

		Signal<T> change, before_change;

	public:

		bool has_value() const {
			return (bool) (value);
		}

		//region ctor/dtor

		IPropertyBase() = default;

		IPropertyBase(IPropertyBase &&other) = default;

		IPropertyBase &operator=(IPropertyBase &&other) = default;

		explicit IPropertyBase(T const &value) : value(value) {}

		explicit IPropertyBase(T &&value) : value(std::move(value)) {}

		virtual ~IPropertyBase() = default;
		//endregion

		virtual void advise_before(Lifetime lifetime, std::function<void(T const &)> handler) const = 0;

		void view(Lifetime lifetime, std::function<void(Lifetime, T const &)> handler) const override {
			if (lifetime->is_terminated()) return;

			Lifetime lf = lifetime.create_nested();
			std::shared_ptr<SequentialLifetimes> seq = std::make_shared<SequentialLifetimes>(lf);

			this->advise_before(lf, [this, lf, seq](T const &v) {
				if (!lf->is_terminated()) {
					seq->terminate_current();
				}
			});

			this->advise(lf, [this, lf, seq, handler](T const &v) {
				if (!lf->is_terminated()) {
					handler(seq->next(), v);
				}
			});
		}
	};
}


#endif //RD_CPP_IPROPERTYBASE_H
