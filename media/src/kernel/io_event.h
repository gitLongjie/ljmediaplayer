#ifndef src_kernel_io_event_h_
#define src_kernel_io_event_h_

#include <memory>
#include <functional>

namespace LJMP {
	class IChannel;

	class IIOEvent {
	public:
		enum class Event {
			E_Unknown,
			E_ReadEable = 1 << 0,
			E_WriteEable = 1 << 2,
			E_ReadWriteEable = E_ReadEable | E_WriteEable,
			E_Remove = 1 << 3,
			E_Add = 1 << 4
		};

		using Ptr = std::shared_ptr<IIOEvent>;
		using WPtr = std::weak_ptr<IIOEvent>;

		using ErrorCallback = std::function<void(int error)>;

	public:
		virtual ~IIOEvent() = default;
		virtual void setErrorCallback(ErrorCallback callback) = 0;
		virtual void initialize() = 0;
		virtual void uninitialize() = 0;
		virtual void updateChannel(const std::shared_ptr<IChannel>& channel, unsigned int event) = 0;
	};
}

#endif // !src_kernel_io_event_h_
