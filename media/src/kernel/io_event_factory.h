#ifndef src_kernel_io_event_factory_h_

#include "src/lj_defined.h"

#include "src/kernel/io_event.h"

#include "src/core/task_queue.h"

namespace LJMP {
	class IOEventFactory {
		disable_copy(IOEventFactory);

	public:
		IOEventFactory() noexcept = default;
		~IOEventFactory() = default;

		IIOEvent::Ptr createSelectEvent(const TaskQueue::Ptr& task_queue, bool own_thread);

	};

}

#endif // !src_kernel_io_event_factory_h_
