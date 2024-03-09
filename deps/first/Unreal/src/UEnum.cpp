#include <Unreal/UEnum.hpp>
#include <Unreal/UClass.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UEnum);
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(UUserDefinedEnum);

#include <MemberVariableLayout_SrcWrapper_UEnum.hpp>

    TArray<TPair<FName, uint8>>& UEnum::GetEnumNamesOldUnsafe()
    {
        static auto Offset = MemberOffsets.find(STR("Names"));
        if (Offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UEnum::Names' (for GetNameByValue) that doesn't exist in this engine version."}; }
        return *Helper::Casting::ptr_cast<TArray<TPair<FName, uint8>>*>(this, Offset->second);
    }

    TArray<TPair<FName, int64>>& UEnum::GetEnumNamesNewUnsafe()
    {
        static auto Offset = MemberOffsets.find(STR("Names"));
        if (Offset == MemberOffsets.end()) { throw std::runtime_error{"Tried getting member variable 'UEnum::Names' (for GetNameByValue) that doesn't exist in this engine version."}; }
        return *Helper::Casting::ptr_cast<TArray<TPair<FName, int64>>*>(this, Offset->second);
    }

    auto UEnum::GetEnumNames() -> TArray<TPair<FName, int64>>
    {
        if (Version::IsBelow(4, 15))
        {
            TArray<TPair<FName, int64>> ReturnArray{};
            for (TPair<FName, uint8>& Elem : GetEnumNamesOldUnsafe()) 
            {
                ReturnArray.Emplace(Elem.Key, static_cast<int64>(Elem.Value));
            }
            return ReturnArray;
        }
        else
        {
            return GetEnumNamesNewUnsafe();
        }
    }

    auto UEnum::GetEnumNameByIndex(int32 Index) -> TPair<FName, int64>
    {
        if (Version::IsBelow(4, 15))
        {
            auto Names = GetEnumNamesOldUnsafe();
            if (!Names.IsValidIndex(Index)) { Output::send<LogLevel::Error>(STR("Enum index out of bounds: {} from an array of size {} for Enum {}"), Index, Names.Num(), this->GetName()); }
            const auto& NameTuple = Names[Index];
            return {NameTuple.Key, static_cast<int64>(NameTuple.Value)};
        }
        else
        {
            auto Names = GetEnumNamesNewUnsafe();
            if (!Names.IsValidIndex(Index)) { Output::send<LogLevel::Error>(STR("Enum index out of bounds: {} from an array of size {} for Enum {}"), Index, Names.Num(), this->GetName()); }
            return Names[Index];
        }
    }

    void UEnum::GetEnumNamesAsVector(std::vector<std::pair<FName, int64>>& OutNames)
    {
        if (Version::IsBelow(4, 15))
        {
            for (TPair<FName, uint8>& Elem : GetEnumNamesOldUnsafe()) 
            {
                OutNames.emplace_back(std::pair{Elem.Key, Elem.Value});
            }
        }
        else
        {
            for (TPair<FName, int64>& Elem : GetEnumNamesNewUnsafe())
            {
                OutNames.emplace_back(std::pair{Elem.Key, Elem.Value});
            }
        }
    }

    auto UEnum::GetEnumFlags() -> EEnumFlags
    {
        if (Version::IsBelow(4, 26))
        {
            // EEnumFlags was added in 4.26
            // Let's assume the 'None' flag is valid for whatever code calls this function in <4.26
            return EEnumFlags::None;
        }
        else
        {
            // The offset of 'Names' + the size of 'Names' + the size of 'CppForm' = offset of 'EnumFlags'
            return GetEnumFlags_Internal();
        }
    }

    FName UEnum::GetNameByValue(int64 Value)
    {
        if (Version::IsBelow(4, 15))
        {
            if (Value < std::numeric_limits<uint8_t>::min() || Value > std::numeric_limits<uint8_t>::max())
            {
                Output::send<LogLevel::Error>(STR("Invalid value used for enumerator value: {}.\n"), this->GetName());
                return {};
            }
            
            uint8 ValueAsU8 = static_cast<uint8>(Value);
            
            for (TPair<FName, uint8>& Elem : GetEnumNamesOldUnsafe()) 
            {
                if (Elem.Value == ValueAsU8)
                {
                    return Elem.Key;
                }
            }
        }
        else
        {
            for (TPair<FName, int64>& Elem : GetEnumNamesNewUnsafe()) 
            {
                if (Elem.Value == Value)
                {
                    return Elem.Key;
                }
            }
        }
        return {};
    }

    int32 UEnum::NumEnums()
    {
        if (Version::IsBelow(4, 15))
        {
            return GetEnumNamesOldUnsafe().Num();
        }
        else
        {
            return GetEnumNamesNewUnsafe().Num();
        }
    }

    StringType UEnum::GenerateEnumPrefix()
    {
        StringType enum_prefix;

        if (NumEnums() > 0)
        {
            enum_prefix = GetEnumNameByIndex(0).Key.ToString();
            
            // For each item in the enumeration, trim the prefix as much as necessary to keep it a prefix.
            // This ensures that once all items have been processed, a common prefix will have been constructed.
            // This will be the longest common prefix since as little as possible is trimmed at each step.
            for (auto [Name, Value] : ForEachName())
            {
                StringType EnumItemName = Name.ToString();

                // Find the length of the longest common prefix of Prefix and EnumItemName.
                int32 PrefixIdx = 0;
                while (PrefixIdx < enum_prefix.length() && PrefixIdx < EnumItemName.length() && enum_prefix[PrefixIdx] == EnumItemName[PrefixIdx])
                {
                    PrefixIdx++;
                }

                // Trim the prefix to the length of the common prefix.
                enum_prefix.resize(PrefixIdx);
            }

            // Find the index of the rightmost underscore in the prefix.
            const int64 UnderscoreIdx = enum_prefix.find_last_of(STR("_"));

            // If an underscore was found, trim the prefix so only the part before the rightmost underscore is included.
            if (UnderscoreIdx > 0)
            {
                enum_prefix.resize(UnderscoreIdx);
            }
            else
            {
                // no underscores in the common prefix - this probably indicates that the names
                // for this enum are not using Epic's notation, so just empty the prefix so that
                // the max item will use the full name of the enum
                enum_prefix.clear();
            }
        }
        
        // If no common prefix was found, or if the enum does not contain any entries,
        // use the name of the enumeration instead.
        if (enum_prefix.length() == 0)
        {
            enum_prefix = GetName();
        }
        return enum_prefix;
    }

    void UEnum::EditNameAt(int32 Index, FName NewName)
    {
        if (Version::IsBelow(4, 15))
        {
            GetEnumNamesOldUnsafe()[Index].Key = NewName;
        }
        else
        {
            GetEnumNamesNewUnsafe()[Index].Key = NewName;
        }
    }

    void UEnum::EditValueAt(int32 Index, int64 NewValue)
    {
        if (Version::IsBelow(4, 15))
        {
            if (NewValue < std::numeric_limits<uint8_t>::min() || NewValue > std::numeric_limits<uint8_t>::max())
            {
                Output::send<LogLevel::Error>(STR("Invalid value used for enumerator value: {}.\n"), this->GetName());
                return;
            }
            uint8 ValueAsU8 = static_cast<uint8>(NewValue);
            GetEnumNamesOldUnsafe()[Index].Value = ValueAsU8;
        }
        else
        {
            GetEnumNamesNewUnsafe()[Index].Value = NewValue;
        }
    }

    void UEnum::RemoveFromNamesAt(int32 Index, int32 Count, bool bAllowShrinking)
    {
        if (Version::IsBelow(4, 15))
        {
            auto& Names = GetEnumNamesOldUnsafe();
            Names.RemoveAt(Index, Count, (Names.Num() > 1 && bAllowShrinking));
        }
        else
        {
            auto& Names = GetEnumNamesNewUnsafe();
            Names.RemoveAt(Index, Count, (Names.Num() > 1 && bAllowShrinking));
        }
    }

    int32 UEnum::InsertIntoNames(const TPair<FName, int64>& Item, int32 Index, bool bShiftValues)
    {
        auto InsertIntoNamesArray = [](auto& NamesArray, const auto NewItem, int32 Index, bool bShiftValues) {
            if (bShiftValues)
            {
                for (auto& CheckName : NamesArray)
                {
                    if (NewItem.Value <= CheckName.Value)
                    {
                        CheckName.Value = CheckName.Value + 1;
                    }
                }
            }
            return NamesArray.Insert(NewItem, Index);
        };

        if (Version::IsBelow(4, 15))
        {
            if (Item.Value < std::numeric_limits<uint8_t>::min() || Item.Value > std::numeric_limits<uint8_t>::max())
            {
                Output::send<LogLevel::Error>(STR("Value < or > uint8 maximum used for enumerator value in <4.15: {}.\n"), this->GetName());
                return -1;
            }
            const TPair<FName, uint8> NewItem{Item.Key, static_cast<uint8>(Item.Value)};
            return InsertIntoNamesArray(GetEnumNamesOldUnsafe(), NewItem, Index, bShiftValues);
        }
        else
        {
            return InsertIntoNamesArray(GetEnumNamesNewUnsafe(), Item, Index, bShiftValues);
        }
    }
    
}
