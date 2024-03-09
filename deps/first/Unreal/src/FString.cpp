#define NOMINMAX

#include <Unreal/FString.hpp>
#include <Unreal/FMemory.hpp>

namespace RC::Unreal
{
    FString::FString(TCHAR* Str) : Data(TArray<wchar_t>(Str, 0, 0))
    {
        size_t StrLength = wcslen(Str);

        if (StrLength > std::numeric_limits<int32_t>::max())
        {
            throw std::runtime_error{"Tried to construct an FString with a size larger than int32"};
        }

        int32_t SafeStrLength = static_cast<int32_t>(StrLength);
        Data.AddUninitialized(SafeStrLength);
        std::memcpy(Data.GetData(), Str, StrLength * sizeof(TCHAR));

        Data.Add(STR('\0'));
    }

    FString::FString(const TCHAR* Str)
    {
        size_t StrLength = wcslen(Str);

        if (StrLength > std::numeric_limits<int32_t>::max())
        {
            throw std::runtime_error{"Tried to construct an FString with a size larger than int32"};
        }

        int32_t SafeStrLength = static_cast<int32_t>(StrLength);
        Data.AddUninitialized(SafeStrLength);
        std::memcpy(Data.GetData(), Str, StrLength * sizeof(TCHAR));

        Data.Add(STR('\0'));
    }

    FStringOut::FStringOut(const FStringOut& Other)
    {
        Data = Other.Data;
    }

    FStringOut::FStringOut(FStringOut&& Other) noexcept
    {
        Data = Other.Data;

        // Set the data pointer to nullptr so that the destructor of the old object won't free the memory
        // This is because the data pointer is still valid & in use
        // Set array_num to 0 to prevent accidentally accessing invalid memory
        Other.Data.SetNum(0);
        Other.Data.SetDataPtr(nullptr);
    }

    bool FString::operator==(FString& Other)
    {
        File::StringType A{};
        File::StringType B{};
        if (Data.GetData()) { A = Data.GetData(); }
        if (Other.Data.GetData()) { B = Other.Data.GetData(); }
        return A == B;
    }

    bool FString::operator==(const FString& Other) const
    {
        File::StringType A{};
        File::StringType B{};
        if (Data.GetData()) { A = Data.GetData(); }
        if (Other.Data.GetData()) { B = Other.Data.GetData(); }
        return A == B;
    }

    auto FString::GetCharArray() const -> const wchar_t*
    {
        return Data.Num() ? Data.GetData() : STR("");
    }

    auto FString::GetCharTArray() const -> const TArray<TCHAR>&
    {
        return Data;
    }

    auto FString::GetCharTArray() -> TArray<TCHAR>&
    {
        return Data;
    }

    auto FString::SetCharArray(TArray<TCHAR>& NewArray) -> void
    {
        if (Data.Num() > 0 && Data.GetData())
        {
            Data.Empty();
        }
        Data = NewArray;
        Data.Add(STR('\0'));
    }

    FString& FString::operator=(const FString& Other)
    {
        if (Data.Num() > 0 && Data.GetData())
        {
            Data.Empty();
        }
        Data = Other.Data;
        return *this;
    }

    auto FString::Clear() -> void
    {
        Data.SetNum(0);
        Data.SetMax(0);
        Data.SetDataPtr(nullptr);
    }

    FStringOut::~FStringOut() = default;
}