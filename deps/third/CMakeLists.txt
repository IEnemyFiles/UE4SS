include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

# Third Party Dependencies -> START
add_subdirectory("GLFW")

# glaze JSON library start
FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/IEnemyFiles/glaze.git
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze)
# glaze JSON library end

# ImGui start
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/IEnemyFiles/imgui.git
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
    CONFIGURE_COMMAND "")
    
FetchContent_Declare(ImGuiTextEdit
    GIT_REPOSITORY https://github.com/IEnemyFiles/ImGuiColorTextEdit.git
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
    CONFIGURE_COMMAND "")
    
FetchContent_Declare(IconFontCppHeaders
    GIT_REPOSITORY https://github.com/IEnemyFiles/IconFontCppHeaders.git
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
    CONFIGURE_COMMAND "")

add_subdirectory("imgui")
# ImGui end

# Zydis start
FetchContent_Declare(zydis
    GIT_REPOSITORY https://github.com/IEnemyFiles/zydis.git
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON)
    
add_subdirectory("zydis")
# Zydis end

# PolyHook start
FetchContent_Declare(PolyHook2
    GIT_REPOSITORY https://github.com/IEnemyFiles/PolyHook_2_0.git
    GIT_PROGRESS ON)

add_subdirectory("PolyHook_2_0")
# PolyHook end

# raw_pdb start
FetchContent_Declare(raw_pdb
    GIT_REPOSITORY https://github.com/IEnemyFiles/raw_pdb.git
    GIT_PROGRESS ON)
    
add_subdirectory("raw_pdb")
# raw_pdb end

# corrosion start
FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG 123be1e3d8170c86e121392e8bffa4def7dc3447)

add_subdirectory("corrosion")
# corrosion end

# Third Party Depenedencies
