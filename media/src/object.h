#ifndef src_object_h_
#define src_object_h_

namespace LJMP {
    class Object {
    public:
        virtual ~Object() = default;

    protected:
        Object() = default;

    };

}

#endif // !src_object_h_

