#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>

namespace rtti
{

    struct Object
    {
        virtual const char* GetType () = 0;
        virtual const char* GetParentType (const ushort& i) = 0;
        virtual ushort GetParentCount () = 0;
        virtual size_t GetTypeID () = 0;
        virtual size_t GetParentTypeID (const ushort& i) = 0;
    };

}

#endif // OBJECT_H
