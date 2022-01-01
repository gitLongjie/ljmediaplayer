#ifndef src_kernel_io_event_std_h_
#define src_kernel_io_event_std_h_

#include "src/lj_defined.h"

#include "src/core/task_object.h"
#include "src/kernel/io_event.h"

namespace LJMP {
	class IOEventStd : public TaskObject, public IIOEvent{
		disable_copy(IOEventStd)

	public:
		~IOEventStd() override;

		void setErrorCallback(ErrorCallback callback) override;
		void initialize() override;
		void uninitialize() override;

	protected:
		IOEventStd(const TaskQueue::Ptr& task_queue, bool own_thread) noexcept;

		virtual void onInitialize() { }
		virtual void onUninitialize() { }

		void errorCallback(int code);

	private:
		void doSetErrorCallback(ErrorCallback callback, ObjectPtr::WPtr wThis);
		void doInitialize(ObjectPtr::WPtr wThis);
		void doUninitialize(ObjectPtr::WPtr wThis);
		void doErrorCallback(int code, ObjectPtr::WPtr wThis);

	private:
		ErrorCallback error_callback_ = nullptr;
	};
}

#endif // !src_kernel_io_event_std_h_
