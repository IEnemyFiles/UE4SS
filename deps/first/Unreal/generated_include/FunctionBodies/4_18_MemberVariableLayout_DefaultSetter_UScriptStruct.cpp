if (auto it = UScriptStruct::MemberOffsets.find(STR("CppStructOps")); it == UScriptStruct::MemberOffsets.end())
{
    UScriptStruct::MemberOffsets.emplace(STR("CppStructOps"), 0x90);
}

if (auto it = UScriptStruct::MemberOffsets.find(STR("StructFlags")); it == UScriptStruct::MemberOffsets.end())
{
    UScriptStruct::MemberOffsets.emplace(STR("StructFlags"), 0x88);
}

if (auto it = UScriptStruct::MemberOffsets.find(STR("bPrepareCppStructOpsCompleted")); it == UScriptStruct::MemberOffsets.end())
{
    UScriptStruct::MemberOffsets.emplace(STR("bPrepareCppStructOpsCompleted"), 0x8C);
}

