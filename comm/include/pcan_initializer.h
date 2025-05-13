#ifndef PCAN_INITIALIZER_H
#define PCAN_INITIALIZER_H

#ifdef _WIN32
#include <windows.h> // For HMODULE, LoadLibrary, GetProcAddress, FreeLibrary, DWORD, WORD etc.
#endif
#include "pcan/PCANBasic.h" // Include the PCAN header
#include <string>
#include <stdexcept> // For runtime_error

// Define function pointer types for the functions we need
typedef TPCANStatus (__stdcall *PCAN_Initialize_t)(TPCANHandle Channel, TPCANBaudrate Btr0Btr1, TPCANType HwType, DWORD IOPort, WORD Interrupt);
typedef TPCANStatus (__stdcall *PCAN_InitializeFD_t)(TPCANHandle Channel, TPCANBitrateFD BitrateFD);
typedef TPCANStatus (__stdcall *PCAN_Uninitialize_t)(TPCANHandle Channel);
typedef TPCANStatus (__stdcall *PCAN_GetErrorText_t)(TPCANStatus Error, WORD Language, LPSTR Buffer);

class PcanInitializer {
public:
    PcanInitializer();
    ~PcanInitializer();

    // Delete copy constructor and assignment operator
    PcanInitializer(const PcanInitializer&) = delete;
    PcanInitializer& operator=(const PcanInitializer&) = delete;

    // Initialize CAN channel
    TPCANHandle InitializeChannel(TPCANHandle channel, TPCANBaudrate bitrate, TPCANType hwType = 0, DWORD ioPort = 0, WORD interrupt = 0);

    // Initialize CAN FD channel
    TPCANHandle InitializeChannelFD(TPCANHandle channel, const std::string& bitrateFD);

    // Uninitialize a specific channel
    void UninitializeChannel(TPCANHandle channel);

    // Get error text for a PCAN status code
    std::string GetErrorString(TPCANStatus status);

private:
    bool loaded_successfully_;
#ifdef _WIN32
    HMODULE pcan_dll_handle_;
#else
    void* pcan_dll_handle_; // Placeholder for non-Windows
#endif

    // Function pointers
    PCAN_Initialize_t   m_CAN_Initialize;
    PCAN_InitializeFD_t m_CAN_InitializeFD;
    PCAN_Uninitialize_t m_CAN_Uninitialize;
    PCAN_GetErrorText_t m_CAN_GetErrorText;

    bool loadPcanLibrary();
    void freePcanLibrary();
};

#endif // PCAN_INITIALIZER_H 