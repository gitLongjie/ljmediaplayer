#include "src/kernel/io_event_factory.h"

#include "src/kernel/io_select_event.h"

namespace LJMP {
	
	IIOEvent::Ptr IOEventFactory::createSelectEvent(const TaskQueue::Ptr& task_queue, bool own_thread)
	{
		return createPtr<IOSelectEvent>(task_queue, own_thread);
	}
}
