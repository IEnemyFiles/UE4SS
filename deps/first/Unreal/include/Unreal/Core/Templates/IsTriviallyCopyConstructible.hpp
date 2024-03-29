// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Unreal/Core/CoreTypes.hpp"
#include <type_traits>

namespace RC::Unreal
{
    /**
     * Traits class which tests if a type has a trivial copy constructor.
     */
    template <typename T>
    struct TIsTriviallyCopyConstructible
    {
      enum { Value = std::is_trivially_copy_constructible_v<T> };
    };
}

