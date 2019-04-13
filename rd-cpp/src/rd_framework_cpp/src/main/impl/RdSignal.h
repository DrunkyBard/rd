#ifndef RD_CPP_RDSIGNAL_H
#define RD_CPP_RDSIGNAL_H

#include "../../../../rd_core_cpp/include/Lifetime.h"
#include "interfaces.h"
#include "../../../../rd_core_cpp/include/IScheduler.h"
#include "../../../../rd_core_cpp/include/SignalX.h"
#include "RdReactiveBase.h"
#include "Polymorphic.h"

#pragma warning( push )
#pragma warning( disable:4250 )
namespace rd {
	template<typename T, typename S = Polymorphic <T>>
	class RdSignal final : public RdReactiveBase, public ISignal<T>, public ISerializable {
	private:
		using WT = typename ISignal<T>::WT;

		std::string logmsg(T const &value) const {
			return "signal " + location.toString() + " " + rdid.toString() + ":: value = " + to_string(value);
		}

	protected:
		Signal <T> signal;
	public:
		//region ctor/dtor

		RdSignal(RdSignal const &) = delete;

		RdSignal &operator=(RdSignal const &) = delete;

		RdSignal() = default;

		RdSignal(RdSignal &&) = default;

		RdSignal &operator=(RdSignal &&) = default;

		virtual ~RdSignal() = default;
		//endregion

		static RdSignal<T, S> read(SerializationCtx const &ctx, Buffer const &buffer) {
			RdSignal<T, S> res;
			const RdId &id = RdId::read(buffer);
			withId(res, id);
			return res;
		}

		void write(SerializationCtx const &ctx, Buffer const &buffer) const override {
			rdid.write(buffer);
		}

		void init(Lifetime lifetime) const override {
			RdReactiveBase::init(lifetime);
			//        wire_scheduler = get_default_scheduler();
			get_wire()->advise(lifetime, this);
		}

		void on_wire_received(Buffer buffer) const override {
			auto value = S::read(this->get_serialization_context(), buffer);
			logReceived.trace(logmsg(wrapper::get<T>(value)));

			signal.fire(wrapper::get<T>(value));
		}

		using ISignal<T>::fire;

		void fire(T const &value) const override {
			assert_bound();
			if (!async) {
				assert_threading();
			}
			get_wire()->send(rdid, [this, &value](Buffer const &buffer) {
				logSend.trace(logmsg(value));
				S::write(get_serialization_context(), buffer, value);
			});
			signal.fire(value);
		}

		using ISource<T>::advise;

		void advise(Lifetime lifetime, std::function<void(const T &)> handler) const override {
			if (is_bound()) {
				assert_threading();
			}
			signal.advise(lifetime, handler);
		}
	};
}

#pragma warning( pop )

static_assert(std::is_move_constructible<rd::RdSignal<int>>::value, "Is not move constructible from RdSignal<int>");

#endif //RD_CPP_RDSIGNAL_H
