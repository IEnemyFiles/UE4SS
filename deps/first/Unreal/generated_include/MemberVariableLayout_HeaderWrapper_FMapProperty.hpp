static std::unordered_map<File::StringType, int32_t> MemberOffsets;

public:
    FProperty*& GetKeyProp();
    const FProperty*& GetKeyProp() const;

public:
    EMapPropertyFlags& GetMapFlags();
    const EMapPropertyFlags& GetMapFlags() const;

public:
    FProperty*& GetValueProp();
    const FProperty*& GetValueProp() const;

