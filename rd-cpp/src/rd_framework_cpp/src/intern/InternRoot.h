#ifndef RD_CPP_INTERNROOT_H
#define RD_CPP_INTERNROOT_H


#include "RdReactiveBase.h"
#include "InternScheduler.h"
#include "Lifetime.h"
#include "wrapper.h"
#include "RdAny.h"
#include "core_traits.h"

#include "tsl/ordered_map.h"

#include <vector>
#include <string>
#include <mutex>


#pragma warning( push )
#pragma warning( disable:4250 )

namespace rd {
	//region predeclared

	class Identities;

	//endregion

	class InternRoot final : public RdReactiveBase {
	private:
		// template<typename T>
		mutable std::vector<InternedAny> myItemsList;

		// template<typename T>
		mutable ordered_map<int32_t, InternedAny> otherItemsList;
		// template<typename T>
		mutable ordered_map<InternedAny, int32_t, any::TransparentHash, any::TransparentKeyEqual> inverseMap;

		mutable InternScheduler intern_scheduler;

		mutable std::recursive_mutex lock;


		void set_interned_correspondence(int32_t id, InternedAny &&value) const;

		static constexpr bool is_index_owned(int32_t id);
	public:
		//region ctor/dtor

		InternRoot();
		//endregion

		template<typename T>
		int32_t intern_value(Wrapper <T> value) const;

		template<typename T>
		Wrapper<T> un_intern_value(int32_t id) const;

		IScheduler *get_wire_scheduler() const override;

		void bind(Lifetime lf, IRdDynamic const *parent, string_view name) const override;

		void identify(const Identities &identities, RdId const &id) const override;

		void on_wire_received(Buffer buffer) const override;
	};	
}

#pragma warning( pop )

#include "InternedAnySerializer.h"

namespace rd {
	/*template<typename T>
	std::vector<value_or_wrapper<T>> InternRoot::myItemsList = {};

	template<typename T>
	ordered_map<int32_t, value_or_wrapper<T>> InternRoot::otherItemsList = {};

	template<typename T>
	ordered_map<value_or_wrapper<T>, int32_t> InternRoot::inverseMap = {};*/

	constexpr bool InternRoot::is_index_owned(int32_t id) {
		return !static_cast<bool>(id & 1);
	}

	template<typename T>
	Wrapper<T> InternRoot::un_intern_value(int32_t id) const {
		//don't need lock because value's already exists and never removes
		return any::get<T>(is_index_owned(id) ? myItemsList[id / 2] : otherItemsList[id / 2]);
	}

	template<typename T>
	int32_t InternRoot::intern_value(Wrapper <T> value) const {
		InternedAny any = any::make_interned_any<T>(value);

		std::lock_guard<decltype(lock)> guard(lock);
		
		auto it = inverseMap.find(any);
		int32_t index = 0;
		if (it == inverseMap.end()) {
			get_protocol()->get_wire()->send(this->rdid, [this, &index, value, any](Buffer const &buffer) {
				InternedAnySerializer::write<T>(get_serialization_context(), buffer, wrapper::get<T>(value));
				{
					std::lock_guard<decltype(lock)> guard(lock);
					index = static_cast<int32_t>(myItemsList.size()) * 2;
					myItemsList.emplace_back(any);
				}
				buffer.write_integral<int32_t>(index);
			});
		} else {
			index = it->second;
		}
		if (inverseMap.count(any) == 0) {
			inverseMap[any] = index;
		}
		return index;
	}
}


#endif //RD_CPP_INTERNROOT_H
