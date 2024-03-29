#pragma once

#include <Unreal/Property/FObjectProperty.hpp>
#include <Unreal/FWeakObjectPtr.hpp>

namespace RC::Unreal
{
    class RC_UE_API FWeakObjectProperty : public TFObjectPropertyBase<FWeakObjectPtr>
    {
        DECLARE_FIELD_CLASS(FWeakObjectProperty);
        DECLARE_VIRTUAL_TYPE(FWeakObjectProperty);
    };
}



