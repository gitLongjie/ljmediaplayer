#ifndef src_object_sptr_h_
#define src_object_sptr_h_

#include <memory>

#include "src/lj_defined.h"

#include "src/core/object.h"

namespace LJMP {

    class ObjectPtr : public Object, public std::enable_shared_from_this<ObjectPtr> {
        disable_copy(ObjectPtr)

    public:
        using Ptr = std::shared_ptr<ObjectPtr>;
        using WPtr = std::weak_ptr<ObjectPtr>;

    public:
        ~ObjectPtr() override;

    protected:
        ObjectPtr() noexcept = default;
    };

}
#endif // !src_object_sptr_h_
