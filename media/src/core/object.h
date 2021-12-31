#ifndef src_core_object_h_
#define src_core_object_h_

namespace LJMP {
    class Object {
    public:
        virtual ~Object() = default;

        virtual void destory() = 0;

    protected:
        Object() = default;

    };

}

#endif // !src_core_object_h_

