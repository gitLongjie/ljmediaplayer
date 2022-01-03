#ifndef src_kernel_io_select_event_h_
#define src_kernel_io_select_event_h_

#include "src/lj_defined.h"

#include "src/kernel/io_event_std.h"

namespace LJMP {
    class IOSelectEvent : public IOEventStd {
        disable_copy(IOSelectEvent);

    public:
        ~IOSelectEvent() override;

    protected:
        IOSelectEvent(const TaskQueue::Ptr& task_queue, bool own_thread) noexcept;

        void onInitialize() override;
        void onUninitialize() override;

        int onProcess() override;


    protected:
        void select();
        void doSelect();

    };
}

#endif // !src_kernel_io_select_event_h_
