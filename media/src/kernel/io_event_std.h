#ifndef src_kernel_io_event_std_h_
#define src_kernel_io_event_std_h_

#include <unordered_map>
#include <vector>

#include "src/lj_defined.h"

#include "src/core/task_object.h"
#include "src/kernel/io_event.h"
#include "src/kernel/channel.h"

namespace LJMP {
	class IOEventStd : public TaskObject, public IIOEvent{
		disable_copy(IOEventStd)

	public:
		~IOEventStd() override;

		void setErrorCallback(ErrorCallback callback) override;
		void initialize() override;
		void uninitialize() override;
		void updateChannel(const IChannel::Ptr& channel, Event event) override;

	protected:
		IOEventStd(const TaskQueue::Ptr& task_queue, bool own_thread) noexcept;

		virtual void onInitialize() { }
		virtual void onUninitialize() { }

		void errorCallback(int code);

	protected:
		using ListChannels = std::vector<IChannel::Ptr>;
		const ListChannels& getReadChannels() const {
			return read_channels_;
		}
		ListChannels& getReadChannels() {
			return read_channels_;
		}
		
		const ListChannels& getWritehannels() const {
			return writer_channels_;
		}
		ListChannels& getWritehannels() {
			return writer_channels_;
		}

		size_t getCount() const {
			return list_channels_.size();
		}

		void process(unsigned long delay);

		virtual int onProcess() { return 0; }

	private:
		void doSetErrorCallback(ErrorCallback callback, ObjectPtr::WPtr wThis);
		void doInitialize(ObjectPtr::WPtr wThis);
		void doUninitialize(ObjectPtr::WPtr wThis);
		void doErrorCallback(int code, ObjectPtr::WPtr wThis);
		void doUpdateChannel(const IChannel::Ptr& channel, Event event, ObjectPtr::WPtr wThis);
		void doProcess(ObjectPtr::WPtr wThis);

		bool addChannel(const IChannel::Ptr& channel);
		void removeChannel(const IChannel::Ptr& channel);

	private:
		ErrorCallback error_callback_ = nullptr;

		using MapListChannels = std::unordered_map<FD, IChannel::Ptr>;

		MapListChannels list_channels_;
		ListChannels read_channels_;
		ListChannels writer_channels_;
	};
}

#endif // !src_kernel_io_event_std_h_
