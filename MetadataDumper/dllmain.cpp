#include "Includes.h"

namespace mem {
    __forceinline MODULEINFO get_module_info(HMODULE module) {
        MODULEINFO mInfo;
        GetModuleInformation(GetCurrentProcess(), module, &mInfo, sizeof(mInfo));
        return mInfo;
    }

    __forceinline uintptr_t find_pattern(HMODULE module, const char* signature) {
        static auto pattern_to_byte = [](const char* pattern) {
            auto bytes = std::vector<char>{};
            auto start = const_cast<char*>(pattern);
            auto end = const_cast<char*>(pattern) + strlen(pattern);

            for (auto current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back('\?');
                }
                else {
                    bytes.push_back(static_cast<char>(strtoul(current, &current, 16)));
                }
            }
            return bytes;
        };

        MODULEINFO mInfo = get_module_info(module);
        uintptr_t base = (uintptr_t)mInfo.lpBaseOfDll;
        uintptr_t sizeOfImage = (uintptr_t)mInfo.SizeOfImage;
        auto patternBytes = pattern_to_byte(signature);

        uintptr_t patternLength = patternBytes.size();
        auto data = patternBytes.data();

        for (uintptr_t i = 0ul; i < sizeOfImage - patternLength; i++) {
            bool found = true;
            for (uintptr_t j = 0ul; j < patternLength; j++) {
                char a = '\?';
                char b = *(char*)(base + i + j);
                found &= data[j] == a || data[j] == b;
            }
            if (found) {
                return base + i;
            }
        }
        return NULL;
    }
} // namespace mem

DWORD  Patch() {

    const char* _MetadataDat = "\x67\x6C\x6F\x62\x61\x6C\x2D\x6D\x65\x74\x61\x64\x61\x74\x61\x2E\x64\x61\x74";
    const uintptr_t _VMLoadMetadataFile = mem::find_pattern(GetModuleHandle(L"GameAssembly.dll"), "48 89 5C 24 08 48 89 74 24 18 55 57 41 56 48 8D 6C 24 B9 48 81 EC B0");

    if (!_VMLoadMetadataFile) {
        std::cout << "Invalid Function Pattern" << std::endl;
        return NULL;
    }

    char* _GlobalMetadata = static_cast<char*>(
        reinterpret_cast<void* (__fastcall*)(const char*)> (_VMLoadMetadataFile)(_MetadataDat)
        );

    std::filesystem::path _MetadataPath = std::filesystem::current_path();
    _MetadataPath /= "Blockade_Data/il2cpp_data/Metadata";
    _MetadataPath /= _MetadataDat;

    uintmax_t _MetadataSize = std::filesystem::file_size(_MetadataPath);
    if (_MetadataSize) {
        char* _MetadataBuffer = (char*)malloc(_MetadataSize);
        std::memcpy(_MetadataBuffer, _GlobalMetadata, _MetadataSize);

        std::ofstream ofs("global-metadata.dump.dat", std::ios::binary);

        if (ofs.is_open()) {
            if (ofs.write(_MetadataBuffer, _MetadataSize))
                std::cout << "Successfully dumped global-metadata.dat" << std::endl;
            else
                std::cout << "Failed to dump global-metadata.dat" << std::endl;
            ofs.close();
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{

    FILE* fp;
    AllocConsole();
    freopen("CON", "w", stdout);
    SetConsoleTitle(L"Internal Metadata Dumper");

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Patch, 0, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


