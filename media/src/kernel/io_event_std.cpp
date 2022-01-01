#include "src/kernel/io_event_std.h"

#include "src/core/log.h"

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

}
