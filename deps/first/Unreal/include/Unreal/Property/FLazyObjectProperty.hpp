#pragma once

#include <Unreal/Property/FObjectProperty.hpp>
#include <Unreal/FWeakObjectPtr.hpp>
#include <Unreal/PersistentObjectPtr.hpp>

namespace RC::Unreal
{
    struct FLazyObjectPtr : public TPersistentObjectPtr<FUniqueObjectGuid>
    {
    };

    class RC_UE_API FLazyObjectProperty : public TFObjectPropertyBase<FLazyObjectPtr>
    {
        DECLARE_FIELD_CLASS(FLazyObjectProperty);
        DECLARE_VIRTUAL_TYPE(FLazyObjectProperty);

    public:
    };
}



