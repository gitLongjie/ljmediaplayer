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

	void IOEventStd::updateChannel(const IChannel::Ptr& channel, unsigned int event) {
		if (enum_cast(IIOEvent::Event::E_Unknown) == event || !channel) {
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

	void IOEventStd::process(unsigned long delay) {
		ObjectPtr::WPtr wThis(shared_from_this());

		auto task = createTaskNoCallback(&IOEventStd::doProcess, this, wThis);
		invoke(task, delay);
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

	void IOEventStd::doUpdateChannel(const IChannel::Ptr& channel, unsigned int event, ObjectPtr::WPtr wThis) {
		LOGI("event =", event);

		ObjectPtr::Ptr self(wThis.lock());
		if (!self) {
			LOG_DESTRUCT;
			return;
		}

		if ((event & enum_cast(IIOEvent::Event::E_Remove)) != 0) {
			LOGD("remove event={}", event);
			removeChannel(channel);
			return;
		}
		else if ((event & enum_cast(IIOEvent::Event::E_Add)) != 0) {
			addChannel(channel);
		}

		if ((event & enum_cast(IIOEvent::Event::E_ReadEable)) != 0) {
			read_channels_.emplace_back(channel);
		}

		if ((event & enum_cast(IIOEvent::Event::E_WriteEable)) != 0) {
			writer_channels_.emplace_back(channel);
		}
	}

	void IOEventStd::doProcess(ObjectPtr::WPtr wThis) {
		ObjectPtr::Ptr self(wThis.lock());
		if (!self) {
			LOG_DESTRUCT;
			return;
		}

		int ret = onProcess();
		if (ret == -2) {
			LOGE("on prcess error");
			return;
		}
		else if (ret == -1) {
			process(1);
		}
		else {
			process(0);
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
