#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>

namespace rtti
{

    struct Object
    {
        Object () { static size_t uid_counter = 1; uid = uid_counter++; }

        inline size_t GetUID () const { return uid; }
        virtual const char* GetType () = 0;
        virtual const char* GetParentType (const ushort& i) = 0;
        virtual ushort GetParentCount () = 0;
        virtual size_t GetTypeID () = 0;
        virtual size_t GetParentTypeID (const ushort& i) = 0;

    private:
        size_t uid;
    };

}

#endif // OBJECT_H
