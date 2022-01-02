#include "src/kernel/io_event_std.h"

#include "src/core/log.h"
#include "src/core/enum_operator.h"

namespace LJMP {
	
	IOEventStd::IOEventStd(const TaskQueue::Ptr& task_queue, bool own_thread) noexcept
		: TaskObject(task_queue, own_thread) {
		LOG_CREATER;
	}

	IOEventStd::~IOEventStd() {
		LOG_DESTRUCT;
	}

	void IOEventStd::setErrorCallback(ErrorCallback callback) {
		
	}

	void IOEventStd::initialize() {
		LOG_ENTER;

		ObjectPtr::Ptr wThis(shared_from_this());
		Task::Ptr task = createTask(std::bind(&IOEventStd::doInitialize, this, wThis));
		invoke(task);
	}

	void IOEventStd::uninitialize() {
		LOG_ENTER;

		ObjectPtr::WPtr wThis(shared_from_this());
		Task::Ptr task = createTask(std::bind(&IOEventStd::doUninitialize, this, wThis));
		invoke(task);
	}

	void IOEventStd::updateChannel(const IChannel::Ptr& channel, Event event) {
		if (IIOEvent::Event::E_Unknown == event || !channel) {
			return;
		}

		ObjectPtr::WPtr wThis(shared_from_this());
		Task::Ptr task = createTaskNoCallback(&IOEventStd::doUpdateChannel, this, channel, event, wThis);
		invoke(task);
	}

	void IOEventStd::errorCallback(int code) {
		LOGI("error code={}", code);

		ObjectPtr::WPtr wThis(shared_from_this());
		Task::Ptr task = createTaskNoCallback(&IOEventStd::doErrorCallback, this, code, wThis);
		invoke(task);
	}

	void IOEventStd::doSetErrorCallback(ErrorCallback callback, ObjectPtr::WPtr wThis) {
		LOG_ENTER;

		ObjectPtr::Ptr self(wThis.lock());
		if (!self) {
			LOG_DESTRUCT;
			return;
		}

		error_callback_ = callback;
	}

	void IOEventStd::doInitialize(ObjectPtr::WPtr wThis) {
		LOG_ENTER;

		ObjectPtr::Ptr self(wThis.lock());
		if (!self) {
			LOG_DESTRUCT;
			return;
		}

		onInitialize();
	}

	void IOEventStd::doUninitialize(ObjectPtr::WPtr wThis) {
		LOG_ENTER;

		ObjectPtr::Ptr self(wThis.lock());
		if (!self) {
			LOG_DESTRUCT;
			return;
		}

		onUninitialize();
	}

	void IOEventStd::doErrorCallback(int code, ObjectPtr::WPtr wThis){
		LOG_ENTER;

		ObjectPtr::Ptr self(wThis.lock());
		if (!self) {
			LOG_DESTRUCT;
			return;
		}

		if (!error_callback_) {
			error_callback_(code);
		}
	}

	void IOEventStd::doUpdateChannel(const IChannel::Ptr& channel, Event event, ObjectPtr::WPtr wThis) {
		LOGI("event =", event);

		ObjectPtr::Ptr self(wThis.lock());
		if (!self) {
			LOG_DESTRUCT;
			return;
		}

		if (enum_cast(event & IIOEvent::Event::E_Remove) != 0) {
			LOGD("remove event={}", event);
			removeChannel(channel);
			return;
		}
		else if (enum_cast(event & IIOEvent::Event::E_Add) != 0) {
			addChannel(channel);
		}

		if (enum_cast(event & IIOEvent::Event::E_ReadEable) != 0) {
			read_channels_.emplace_back(channel);
		}

		if (enum_cast(event & IIOEvent::Event::E_WriteEable) != 0) {
			writer_channels_.emplace_back(channel);
		}
	}

	bool IOEventStd::addChannel(const IChannel::Ptr& channel) {
		LOGI("add channel");

		if (!channel) {
			LOGD("channel is nullptr");
			return false;
		}

		FD fd = channel->getFD();
		list_channels_[fd] = channel;
		return true;
	}

	void IOEventStd::removeChannel(const IChannel::Ptr& channel) {
		LOG_ENTER;

		if (!channel) {
			LOGD("channel is nullptr");
			return;
		}

		list_channels_.erase(channel->getFD());
	}

}
