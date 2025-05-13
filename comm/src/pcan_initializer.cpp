#include "pcan_initializer.h"
#include <iostream> // For error messages
#include <vector>   // For error buffer

// Define the DLL name
const char* PCAN_DLL_NAME = "PCANBasic.dll";

PcanInitializer::PcanInitializer()
    : loaded_successfully_(false),
      pcan_dll_handle_(nullptr),
      m_CAN_Initialize(nullptr),
      m_CAN_InitializeFD(nullptr),
      m_CAN_Uninitialize(nullptr),
      m_CAN_GetErrorText(nullptr)
{
    loaded_successfully_ = loadPcanLibrary();
    if (!loaded_successfully_) {
        std::cerr << "Critical Error: Failed to load " << PCAN_DLL_NAME << " or resolve required functions." << std::endl;
        // Consider throwing an exception here if loading is absolutely critical for the application start
        // throw std::runtime_error("Failed to load PCANBasic library");
    }
}

PcanInitializer::~PcanInitializer() {
    freePcanLibrary();
}

bool PcanInitializer::loadPcanLibrary() {
#ifdef _WIN32
    pcan_dll_handle_ = LoadLibraryA(PCAN_DLL_NAME);
    if (pcan_dll_handle_ == nullptr) {
        std::cerr << "Error loading " << PCAN_DLL_NAME << ". Error code: " << GetLastError() << std::endl;
        return false;
    }

    // Load required functions
    m_CAN_Initialize = (PCAN_Initialize_t)GetProcAddress(pcan_dll_handle_, "CAN_Initialize");
    m_CAN_InitializeFD = (PCAN_InitializeFD_t)GetProcAddress(pcan_dll_handle_, "CAN_InitializeFD");
    m_CAN_Uninitialize = (PCAN_Uninitialize_t)GetProcAddress(pcan_dll_handle_, "CAN_Uninitialize");
    m_CAN_GetErrorText = (PCAN_GetErrorText_t)GetProcAddress(pcan_dll_handle_, "CAN_GetErrorText");

    // Check if all essential functions were loaded
    if (!m_CAN_Initialize || !m_CAN_InitializeFD || !m_CAN_Uninitialize || !m_CAN_GetErrorText) {
        std::cerr << "Error resolving function addresses in " << PCAN_DLL_NAME << "." << std::endl;
        FreeLibrary(pcan_dll_handle_);
        pcan_dll_handle_ = nullptr;
        return false;
    }
    return true;
#else
    // Placeholder for non-Windows platforms (e.g., using dlopen)
    std::cerr << "Dynamic library loading not implemented for this platform." << std::endl;
    return false;
#endif
}

void PcanInitializer::freePcanLibrary() {
#ifdef _WIN32
    if (pcan_dll_handle_ != nullptr) {
        FreeLibrary(pcan_dll_handle_);
        pcan_dll_handle_ = nullptr;
        loaded_successfully_ = false;
        // Reset function pointers
        m_CAN_Initialize = nullptr;
        m_CAN_InitializeFD = nullptr;
        m_CAN_Uninitialize = nullptr;
        m_CAN_GetErrorText = nullptr;
    }
#else
    // Placeholder
#endif
}

TPCANHandle PcanInitializer::InitializeChannel(TPCANHandle channel, TPCANBaudrate bitrate, TPCANType hwType, DWORD ioPort, WORD interrupt) {
    if (!loaded_successfully_ || !m_CAN_Initialize) {
        throw std::runtime_error("PCAN library not loaded or CAN_Initialize function not found.");
    }

    TPCANStatus status = m_CAN_Initialize(channel, bitrate, hwType, ioPort, interrupt);
    if (status != PCAN_ERROR_OK) {
        throw std::runtime_error("CAN_Initialize failed: " + GetErrorString(status));
    }
    return channel; // Return the handle if successful
}

// TPCANHandle PcanInitializer::InitializeChannelFD(TPCANHandle channel, const std::string& bitrateFD) {
TPCANHandle PcanInitializer::InitializeChannelFD(TPCANHandle channel, const std::string& bitrateFD) {
    if (!loaded_successfully_ || !m_CAN_InitializeFD) {
        throw std::runtime_error("PCAN library not loaded or CAN_InitializeFD function not found.");
    }
    
    // Need to convert std::string to TPCANBitrateFD (char*)
    // Ensure the string is null-terminated
    TPCANStatus status = m_CAN_InitializeFD(channel, const_cast<char*>(bitrateFD.c_str()));
    if (status != PCAN_ERROR_OK) {
        throw std::runtime_error("CAN_InitializeFD failed: " + GetErrorString(status));
    }
    return channel;
}

void PcanInitializer::UninitializeChannel(TPCANHandle channel) {
    if (!loaded_successfully_ || !m_CAN_Uninitialize) {
        // Maybe just log a warning instead of throwing, as uninitialization might be less critical
        std::cerr << "Warning: PCAN library not loaded or CAN_Uninitialize function not found during uninitialization attempt." << std::endl;
        return; 
    }
    m_CAN_Uninitialize(channel); // Ignore return status for simplicity here, but could check it
}

std::string PcanInitializer::GetErrorString(TPCANStatus status) {
    if (!loaded_successfully_ || !m_CAN_GetErrorText) {
        return "PCAN library not loaded or CAN_GetErrorText function not found.";
    }

    char error_buffer[256];
    TPCANStatus result = m_CAN_GetErrorText(status, 0, error_buffer); // Language 0 = Neutral
    if (result != PCAN_ERROR_OK) {
        return "Error getting error text (CAN_GetErrorText failed).";
    }
    return std::string(error_buffer);
} 