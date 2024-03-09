#include <bit>

#include <Unreal/ClassListener.hpp>
#include <Unreal/UnrealInitializer.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/Searcher/ObjectSearcher.hpp>

namespace RC::Unreal
{
    FClassCreateListener FClassCreateListener::ClassCreateListener{};
    FClassDeleteListener FClassDeleteListener::ClassDeleteListener{};

    std::unordered_map<StringType, UObject*> FClassCreateListener::GlobalObjectCache{};
    std::unordered_map<UObject*, StringType> FClassCreateListener::GlobalObjectValueCache{};

    void CacheGlobalObject(UObject* Object)
    {
        auto [ValueIt, bWasEmplaced] = FClassCreateListener::GlobalObjectValueCache.emplace(Object, StringType{});
        if (bWasEmplaced)
        {
            auto FullName = Object->GetFullName();
            auto SpaceLocation = FullName.find(STR(" "));
            auto FullNameWithoutType = StringType{FullName.begin() + SpaceLocation + 1, FullName.end()};
            ValueIt->second = FullNameWithoutType;
            FClassCreateListener::GlobalObjectCache.emplace(FullNameWithoutType, Object);
        }
    }

    void InvalidateGlobalObject(UObject* Object)
    {
        if (auto ValueIt = FClassCreateListener::GlobalObjectValueCache.find(Object); ValueIt != FClassCreateListener::GlobalObjectValueCache.end())
        {
            FClassCreateListener::GlobalObjectCache.erase(ValueIt->second);
            FClassCreateListener::GlobalObjectValueCache.erase(ValueIt);
        }
    }

    UObject* GetGlobalObject(const StringType& FullNameWithoutType)
    {
        if (auto It = FClassCreateListener::GlobalObjectCache.find(FullNameWithoutType); It != FClassCreateListener::GlobalObjectCache.end())
        {
            return It->second;
        }
        else
        {
            return nullptr;
        }
    }

    void FClassCreateListener::NotifyUObjectCreated(const UObjectBase* Object, int32 Index)
    {
    }

    void FClassCreateListener::OnUObjectArrayShutdown()
    {
        UnrealInitializer::StaticStorage::bIsInitialized = false;
        UObjectArray::RemoveUObjectCreateListener(this);
    }

    template<typename T>
    static void DeleteAllFromPool(T &Pool, const FUObjectItem *ObjectItem)
    {
        Pool.erase(std::remove_if(Pool.begin(), Pool.end(), [&](const auto& Item) {
            return Item == ObjectItem;
        }), Pool.end());
    }

    void FClassDeleteListener::NotifyUObjectDeleted(const UObjectBase* Object, int32 Index)
    {
        InvalidateGlobalObject(std::bit_cast<UObject*>(Object));

        auto* ObjectItem = Object->GetObjectItem();

        // All classes.
        DeleteAllFromPool(ObjectSearcherPool<UClass, AnySuperStruct>::Pool, ObjectItem);

        // All actor classes.
        DeleteAllFromPool(ObjectSearcherPool<UClass, AActor>::Pool, ObjectItem);

        // All actor instances.
        DeleteAllFromPool(ObjectSearcherPool<AActor, AnySuperStruct>::Pool, ObjectItem);
    }

    void FClassDeleteListener::OnUObjectArrayShutdown()
    {
        UnrealInitializer::StaticStorage::bIsInitialized = false;
        UObjectArray::RemoveUObjectDeleteListener(this);
    }

}
