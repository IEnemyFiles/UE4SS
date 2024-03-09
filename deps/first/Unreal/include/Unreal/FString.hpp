#pragma once

#include <Function/Function.hpp>

#include <Unreal/Common.hpp>
#include <Unreal/Core/Containers/Array.hpp>

namespace RC::Unreal
{
    namespace UnrealInitializer
    {
        struct CacheInfo;
        RC_UE_API auto CreateCache(UnrealInitializer::CacheInfo& Target) -> void;
        RC_UE_API auto LoadCache(UnrealInitializer::CacheInfo& Target) -> void;
    }

    class RC_UE_API FString
    {
    protected:
        friend RC_UE_API auto UnrealInitializer::CreateCache(UnrealInitializer::CacheInfo& Target) -> void;
        friend RC_UE_API auto UnrealInitializer::LoadCache(UnrealInitializer::CacheInfo& Target) -> void;

    protected:
        TArray<TCHAR> Data{nullptr, 0, 0};

    public:
        FString() = default;
        explicit FString(TCHAR* Str);
        explicit FString(const TCHAR* Str);

        bool operator==(FString& Other);
        bool operator==(const FString& Other) const;

        FString& operator=(const FString& Other);

        [[nodiscard]] auto GetCharArray() const -> const wchar_t*;
        [[nodiscard]] auto GetCharTArray() const -> const TArray<TCHAR>&;
        [[nodiscard]] auto GetCharTArray() -> TArray<TCHAR>&;
        auto SetCharArray(TArray<TCHAR>& NewArray) -> void;
        auto Clear() -> void;
    };

    class FStringOut : public FString
    {
    public:
        FStringOut() = default;
        FStringOut(const FStringOut&); // Copy constructor
        FStringOut(FStringOut&&) noexcept;  // Move constructor
        ~FStringOut();
    };

}


