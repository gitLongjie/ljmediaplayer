#ifndef src_kernel_io_event_h_
#define src_kernel_io_event_h_

#include <memory>

namespace LJMP {
	class IIOEvent {
	public:
		using Ptr = std::shared_ptr<IIOEvent>;
		using WPtr = std::weak_ptr<IIOEvent>;

		using ErrorCallback = std::function<void(int error)>;

	public:
		virtual ~IIOEvent() = default;
		virtual void setErrorCallback(ErrorCallback callback) = 0;
		virtual void initialize() = 0;
		virtual void uninitialize() = 0;
	};
}

#endif // !src_kernel_io_event_h_
