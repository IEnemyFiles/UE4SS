#include <filesystem>

#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/Signatures.hpp>
#include <Unreal/UnrealVersion.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/FString.hpp>
#include <Unreal/FMemory.hpp>
#include <Unreal/FText.hpp>
#include <SigScanner/SinglePassSigScanner.hpp>
#include <File/File.hpp>
#include <Helpers/ASM.hpp>
#include <Helpers/Casting.hpp>
#include <ASMHelper/ASMHelper.hpp>

namespace RC::Unreal::Signatures
{
    auto ScanForGameFunctionsAndData(const UnrealInitializer::Config& config) -> ScanResult
    {
        // TODO: Add some code here to deal with nothing being found due to code being injected too early
        return ScanForGameFunctionsAndDataImpl(config);
    }
    auto ScanForGUObjectArray(const UnrealInitializer::Config& config) -> ScanResult
    {
        // TODO: Add some code here to deal with nothing being found due to code being injected too early
        return ScanForGUObjectArrayImpl(config);
    }

    auto ScanForGameFunctionsAndDataImpl(const UnrealInitializer::Config& config) -> ScanResult
    {
        ScanResult scan_result;

        SinglePassScanner::SignatureContainerMap signature_container_map;
        std::vector<SignatureContainer> signature_containers_coreuobject;
        std::vector<SignatureContainer> signature_containers_core;
        std::vector<SignatureContainer> signature_containers_engine;
        std::vector<SignatureContainer> signature_containers_umg;
        std::vector<SignatureContainer> signature_containers_mainexe;

        uint8_t* FNameToStringAddress{};
        uint8_t FNameToStringNumMatches{};


        VersionStatus version_status{};

        if (config.ScanOverrides.version_finder)
        {
            config.ScanOverrides.version_finder(signature_containers_core, scan_result);
        }

        if (config.ScanOverrides.fname_to_string)
        {
            // If we have an override look in the 'Core' module because that's where FName::ToString is
            config.ScanOverrides.fname_to_string(signature_containers_core, scan_result);
        }

        if (config.ScanOverrides.static_construct_object)
        {
            config.ScanOverrides.static_construct_object(signature_containers_umg, scan_result);
        }

        signature_container_map.emplace(ScanTarget::CoreUObject, signature_containers_coreuobject);
        signature_container_map.emplace(ScanTarget::Core, signature_containers_core);
        signature_container_map.emplace(ScanTarget::Engine, signature_containers_engine);
        signature_container_map.emplace(ScanTarget::UMG, signature_containers_umg);
        signature_container_map.emplace(ScanTarget::MainExe, signature_containers_mainexe);

        SinglePassScanner::start_scan(signature_container_map);

        // Special error handler for FName::ToString
        // Needed because it checks in more than one module and the scanner isn't setup for that
        if (FNameToStringNumMatches > 1)
        {
            scan_result.InfoMessages.emplace(STR("Found more than one FName::ToString, using the first match\n"));
        }

        if (scan_result.Errors.empty())
        {
            scan_result.Status = ScanStatus::Succeeded;
        }
        return scan_result;
    }
    auto ScanForGUObjectArrayImpl(const UnrealInitializer::Config& config) -> ScanResult
    {
        ScanResult scan_result;

        SinglePassScanner::SignatureContainerMap signature_container_map;
        std::vector<SignatureContainer> signature_containers_coreuobject;
        std::vector<SignatureContainer> signature_containers_core;

        // FName:FName has to be in the second pass because we need access to FName::ToString which is found in the first pass
        if (config.ScanOverrides.fname_constructor)
        {
            config.ScanOverrides.fname_constructor(signature_containers_core, scan_result);
        }
        if (config.ScanOverrides.fmemory_free)
        {
            config.ScanOverrides.fmemory_free(signature_containers_core, scan_result);
        }
        if (config.ScanOverrides.guobjectarray)
        {
            config.ScanOverrides.guobjectarray(signature_containers_coreuobject, scan_result);
        }
        if (config.ScanOverrides.ftext_constructor)
        {
            config.ScanOverrides.ftext_constructor(signature_containers_core, scan_result);
        }

        signature_container_map.emplace(ScanTarget::Core, signature_containers_core);
        signature_container_map.emplace(ScanTarget::CoreUObject, signature_containers_coreuobject);
        SinglePassScanner::start_scan(signature_container_map);

        if (scan_result.Errors.empty())
        {
            scan_result.Status = Signatures::ScanStatus::Succeeded;
        }
        return scan_result;
    }
}
