#include <Unreal/VersionedContainer/Derived427.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/FProperty.hpp>
#include <Helpers/Casting.hpp>

namespace RC::Unreal
{
    void Derived427::UObjectArray_AddUObjectCreateListener(FUObjectCreateListener* Listener)
    {
        if (m_guobjectarray_internal->UObjectCreateListeners.Contains(Listener))
        {
            throw std::runtime_error{"Cannot add a listener because it already exists in TArray"};
        }
        /* To workaround unreal's implementation flaws, ensuring that 
        TArray's length growth occurs after the Data pointer becomes available */
        auto new_ary = TArray<FUObjectCreateListener*>{Listener};
        m_guobjectarray_internal->UObjectCreateListeners.Append(new_ary);
    }

    void Derived427::UObjectArray_RemoveUObjectCreateListener(FUObjectCreateListener* Listener)
    {
        m_guobjectarray_internal->UObjectCreateListeners.RemoveSingleSwap(Listener);
    }

    void Unreal::Derived427::UObjectArray_AddUObjectDeleteListener(FUObjectDeleteListener* Listener)
    {
        if (m_guobjectarray_internal->UObjectDeleteListeners.Contains(Listener))
        {
            throw std::runtime_error{"Cannot add a listener because it already exists in TArray"};
        }
        auto new_ary = TArray<FUObjectDeleteListener*>{Listener};
        m_guobjectarray_internal->UObjectDeleteListeners.Append(new_ary);
    }

    void Unreal::Derived427::UObjectArray_RemoveUObjectDeleteListener(FUObjectDeleteListener* Listener)
    {
        m_guobjectarray_internal->UObjectDeleteListeners.RemoveSingleSwap(Listener);
    }

    auto Derived427::Field_get_type_fname(FField* p_this) -> FName
    {
        return p_this->GetClass().GetFName();
    }

    auto Derived427::Field_get_ffield_owner(FField* p_this) -> void*
    {
        return p_this->GetOwnerVariant().ToField();
    }

    auto Derived427::FFieldClass_get_fname(FFieldClass* p_this) -> FName
    {
        return Helper::Casting::offset_deref<FName>(p_this, 0);
    }

    void Derived427::UObjectArray_Lock()
    {
        Windows::EnterCriticalSection(std::bit_cast<Windows::CRITICAL_SECTION*>(&m_guobjectarray_internal->ObjObjectsCritical));
    }
    void Derived427::UObjectArray_Unlock()
    {
        Windows::LeaveCriticalSection(std::bit_cast<Windows::CRITICAL_SECTION*>(&m_guobjectarray_internal->ObjObjectsCritical));
    }
}
