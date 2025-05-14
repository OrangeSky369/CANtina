#include "pcan_uds_handler.h" // Changed from pcan_initializer.h
#include <iostream> // For error messages
#include <vector>   // For error buffer
#include <stdexcept> // Required for std::runtime_error

// Define the DLL name for PCAN-UDS
const char* PCAN_DLL_NAME = "PCAN-UDS.dll"; 

PcanUdsHandler::PcanUdsHandler() // Changed class name
    : loaded_successfully_(false),
      pcan_dll_handle_(nullptr),
      // Initialize all PCAN-UDS function pointers to nullptr
      m_UDS_Initialize(nullptr),
      m_UDS_InitializeFD(nullptr),
      m_UDS_Uninitialize(nullptr),
      m_UDS_GetErrorText(nullptr),
      m_UDS_Write(nullptr),
      m_UDS_Read(nullptr),
      m_UDS_MsgAlloc(nullptr),
      m_UDS_MsgFree(nullptr),
      m_UDS_Reset(nullptr),
      m_UDS_SvcDiagnosticSessionControl(nullptr),
      m_UDS_SvcECUReset(nullptr),
      m_UDS_SvcSecurityAccess(nullptr),
      m_UDS_SvcRequestDownload(nullptr),
      m_UDS_SvcTransferData(nullptr),
      m_UDS_SvcRequestTransferExit(nullptr),
      m_UDS_WaitForService(nullptr)
{
    loaded_successfully_ = loadPcanLibrary();
    if (!loaded_successfully_) {
        std::cerr << "Critical Error: Failed to load " << PCAN_DLL_NAME << " or resolve required UDS functions." << std::endl;
    }
}

PcanUdsHandler::~PcanUdsHandler() { // Changed class name
    freePcanLibrary();
}

bool PcanUdsHandler::loadPcanLibrary() { // Changed class name
#ifdef _WIN32
    pcan_dll_handle_ = LoadLibraryA(PCAN_DLL_NAME);
    if (pcan_dll_handle_ == nullptr) {
        std::cerr << "Error loading " << PCAN_DLL_NAME << ". Error code: " << GetLastError() << std::endl;
        return false;
    }

    // Load UDS functions
    m_UDS_Initialize = (UDS_Initialize_t)GetProcAddress(pcan_dll_handle_, "UDS_Initialize_2013");
    m_UDS_InitializeFD = (UDS_InitializeFD_t)GetProcAddress(pcan_dll_handle_, "UDS_InitializeFD_2013");
    m_UDS_Uninitialize = (UDS_Uninitialize_t)GetProcAddress(pcan_dll_handle_, "UDS_Uninitialize_2013");
    m_UDS_GetErrorText = (UDS_GetErrorText_t)GetProcAddress(pcan_dll_handle_, "UDS_GetErrorText_2013");
    m_UDS_Write = (UDS_Write_t)GetProcAddress(pcan_dll_handle_, "UDS_Write_2013");
    m_UDS_Read = (UDS_Read_t)GetProcAddress(pcan_dll_handle_, "UDS_Read_2013");
    m_UDS_MsgAlloc = (UDS_MsgAlloc_t)GetProcAddress(pcan_dll_handle_, "UDS_MsgAlloc_2013");
    m_UDS_MsgFree = (UDS_MsgFree_t)GetProcAddress(pcan_dll_handle_, "UDS_MsgFree_2013");
    m_UDS_Reset = (UDS_Reset_t)GetProcAddress(pcan_dll_handle_, "UDS_Reset_2013");
    
    // Load UDS Service functions
    m_UDS_SvcDiagnosticSessionControl = (UDS_SvcDiagnosticSessionControl_t)GetProcAddress(pcan_dll_handle_, "UDS_SvcDiagnosticSessionControl_2013");
    m_UDS_SvcECUReset = (UDS_SvcECUReset_t)GetProcAddress(pcan_dll_handle_, "UDS_SvcECUReset_2013");
    m_UDS_SvcSecurityAccess = (UDS_SvcSecurityAccess_t)GetProcAddress(pcan_dll_handle_, "UDS_SvcSecurityAccess_2013");
    m_UDS_SvcRequestDownload = (UDS_SvcRequestDownload_t)GetProcAddress(pcan_dll_handle_, "UDS_SvcRequestDownload_2013");
    m_UDS_SvcTransferData = (UDS_SvcTransferData_t)GetProcAddress(pcan_dll_handle_, "UDS_SvcTransferData_2013");
    m_UDS_SvcRequestTransferExit = (UDS_SvcRequestTransferExit_t)GetProcAddress(pcan_dll_handle_, "UDS_SvcRequestTransferExit_2013");
    m_UDS_WaitForService = (UDS_WaitForService_t)GetProcAddress(pcan_dll_handle_, "UDS_WaitForService_2013");

    if (!m_UDS_Initialize || !m_UDS_InitializeFD || !m_UDS_Uninitialize || !m_UDS_GetErrorText ||
        !m_UDS_Write || !m_UDS_Read || !m_UDS_MsgAlloc || !m_UDS_MsgFree || !m_UDS_Reset ||
        !m_UDS_SvcDiagnosticSessionControl || !m_UDS_SvcECUReset || !m_UDS_SvcSecurityAccess ||
        !m_UDS_SvcRequestDownload || !m_UDS_SvcTransferData || !m_UDS_SvcRequestTransferExit || !m_UDS_WaitForService) {
        std::cerr << "Error resolving some function addresses in " << PCAN_DLL_NAME << "." << std::endl;
        FreeLibrary(pcan_dll_handle_);
        pcan_dll_handle_ = nullptr;
        m_UDS_Initialize = nullptr; m_UDS_InitializeFD = nullptr; m_UDS_Uninitialize = nullptr;
        m_UDS_GetErrorText = nullptr; m_UDS_Write = nullptr; m_UDS_Read = nullptr;
        m_UDS_MsgAlloc = nullptr; m_UDS_MsgFree = nullptr; m_UDS_Reset = nullptr;
        m_UDS_SvcDiagnosticSessionControl = nullptr; m_UDS_SvcECUReset = nullptr;
        m_UDS_SvcSecurityAccess = nullptr; m_UDS_SvcRequestDownload = nullptr;
        m_UDS_SvcTransferData = nullptr; m_UDS_SvcRequestTransferExit = nullptr;
        m_UDS_WaitForService = nullptr;
        return false;
    }
    return true;
#else
    std::cerr << "Dynamic library loading not implemented for this platform." << std::endl;
    return false;
#endif
}

void PcanUdsHandler::freePcanLibrary() { // Changed class name
#ifdef _WIN32
    if (pcan_dll_handle_ != nullptr) {
        FreeLibrary(pcan_dll_handle_);
        pcan_dll_handle_ = nullptr;
        loaded_successfully_ = false;
        m_UDS_Initialize = nullptr;
        m_UDS_InitializeFD = nullptr;
        m_UDS_Uninitialize = nullptr;
        m_UDS_GetErrorText = nullptr;
        m_UDS_Write = nullptr;
        m_UDS_Read = nullptr;
        m_UDS_MsgAlloc = nullptr;
        m_UDS_MsgFree = nullptr;
        m_UDS_Reset = nullptr;
        m_UDS_SvcDiagnosticSessionControl = nullptr;
        m_UDS_SvcECUReset = nullptr;
        m_UDS_SvcSecurityAccess = nullptr;
        m_UDS_SvcRequestDownload = nullptr;
        m_UDS_SvcTransferData = nullptr;
        m_UDS_SvcRequestTransferExit = nullptr;
        m_UDS_WaitForService = nullptr;
    }
#else
#endif
}

cantp_handle PcanUdsHandler::InitializeChannel(cantp_handle channel, cantp_baudrate bitrate, cantp_hwtype hwType, DWORD ioPort, WORD interrupt) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_Initialize) {
        throw std::runtime_error("PCAN-UDS library not loaded or UDS_Initialize_2013 function not found.");
    }
    uds_status status = m_UDS_Initialize(channel, bitrate, hwType, ioPort, interrupt);
    if (status != PUDS_STATUS_OK) {
        throw std::runtime_error("UDS_Initialize_2013 failed: " + GetErrorString(status));
    }
    return channel; 
}

cantp_handle PcanUdsHandler::InitializeChannelFD(cantp_handle channel, const std::string& bitrateFD) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_InitializeFD) {
        throw std::runtime_error("PCAN-UDS library not loaded or UDS_InitializeFD_2013 function not found.");
    }
    uds_status status = m_UDS_InitializeFD(channel, const_cast<char*>(bitrateFD.c_str()));
    if (status != PUDS_STATUS_OK) {
        throw std::runtime_error("UDS_InitializeFD_2013 failed: " + GetErrorString(status));
    }
    return channel;
}

void PcanUdsHandler::UninitializeChannel(cantp_handle channel) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_Uninitialize) {
        std::cerr << "Warning: PCAN-UDS library not loaded or UDS_Uninitialize_2013 function not found during uninitialization attempt." << std::endl;
        return; 
    }
    m_UDS_Uninitialize(channel); 
}

void PcanUdsHandler::ResetChannel(cantp_handle channel) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_Reset) {
        throw std::runtime_error("PCAN-UDS library not loaded or UDS_Reset_2013 function not found.");
    }
    uds_status status = m_UDS_Reset(channel);
    if (status != PUDS_STATUS_OK) {
        throw std::runtime_error("UDS_Reset_2013 failed: " + GetErrorString(status));
    }
}

std::string PcanUdsHandler::GetErrorString(uds_status status) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_GetErrorText) {
        char fallback_msg[100];
        sprintf_s(fallback_msg, sizeof(fallback_msg), "PCAN-UDS GetErrorText not loaded. UDS_Status: 0x%X", status);
        return std::string(fallback_msg);
    }
    char error_buffer[256];
    uds_status result = m_UDS_GetErrorText(status, 0, error_buffer);
    if (result != PUDS_STATUS_OK) {
        char fallback_msg[100];
        sprintf_s(fallback_msg, sizeof(fallback_msg), "UDS_GetErrorText_2013 failed. Original UDS_Status: 0x%X", status);
        return std::string(fallback_msg);
    }
    return std::string(error_buffer);
} 

uds_status PcanUdsHandler::AllocateMessage(uds_msg* msg_buffer, uds_msgconfig msg_configuration, uint32_t msg_data_length) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_MsgAlloc) {
        throw std::runtime_error("PCAN-UDS library not loaded or UDS_MsgAlloc_2013 function not found.");
    }
    return m_UDS_MsgAlloc(msg_buffer, msg_configuration, msg_data_length);
}

uds_status PcanUdsHandler::FreeMessage(uds_msg* msg_buffer) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_MsgFree) {
        throw std::runtime_error("PCAN-UDS library not loaded or UDS_MsgFree_2013 function not found.");
    }
    return m_UDS_MsgFree(msg_buffer);
}

uds_status PcanUdsHandler::WriteMessage(cantp_handle channel, uds_msg* msg_buffer) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_Write) {
        throw std::runtime_error("PCAN-UDS library not loaded or UDS_Write_2013 function not found.");
    }
    return m_UDS_Write(channel, msg_buffer);
}

uds_status PcanUdsHandler::ReadMessage(cantp_handle channel, uds_msg* out_msg_buffer, uds_msg* in_msg_request, cantp_timestamp* out_timestamp) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_Read) {
        throw std::runtime_error("PCAN-UDS library not loaded or UDS_Read_2013 function not found.");
    }
    return m_UDS_Read(channel, out_msg_buffer, in_msg_request, out_timestamp);
}

uds_status PcanUdsHandler::WaitForService(cantp_handle channel, uds_msg* msg_request, uds_msg* out_msg_response, uds_msg* out_msg_request_confirmation) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_WaitForService) {
        throw std::runtime_error("PCAN-UDS library not loaded or UDS_WaitForService_2013 function not found.");
    }
    return m_UDS_WaitForService(channel, msg_request, out_msg_response, out_msg_request_confirmation);
}

uds_status PcanUdsHandler::SvcDiagnosticSessionControl(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE session_type) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_SvcDiagnosticSessionControl) {
        throw std::runtime_error("UDS_SvcDiagnosticSessionControl_2013 function not found.");
    }
    return m_UDS_SvcDiagnosticSessionControl(channel, request_config, out_msg_request, session_type);
}

uds_status PcanUdsHandler::SvcECUReset(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE reset_type) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_SvcECUReset) {
        throw std::runtime_error("UDS_SvcECUReset_2013 function not found.");
    }
    return m_UDS_SvcECUReset(channel, request_config, out_msg_request, reset_type);
}

uds_status PcanUdsHandler::SvcSecurityAccess(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE security_access_type, BYTE* security_access_data, DWORD security_access_data_size) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_SvcSecurityAccess) {
        throw std::runtime_error("UDS_SvcSecurityAccess_2013 function not found.");
    }
    return m_UDS_SvcSecurityAccess(channel, request_config, out_msg_request, security_access_type, security_access_data, security_access_data_size);
}

uds_status PcanUdsHandler::SvcRequestDownload(
    cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, 
    BYTE compression_method, BYTE encrypting_method, 
    BYTE* memory_address_buffer, BYTE memory_address_size, 
    BYTE* memory_size_buffer, BYTE memory_size_size) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_SvcRequestDownload) {
        throw std::runtime_error("UDS_SvcRequestDownload_2013 function not found.");
    }
    return m_UDS_SvcRequestDownload(channel, request_config, out_msg_request, compression_method, encrypting_method, memory_address_buffer, memory_address_size, memory_size_buffer, memory_size_size);
}

uds_status PcanUdsHandler::SvcTransferData(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE block_sequence_counter, BYTE* transfer_request_parameter_record, DWORD transfer_request_parameter_record_size) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_SvcTransferData) {
        throw std::runtime_error("UDS_SvcTransferData_2013 function not found.");
    }
    return m_UDS_SvcTransferData(channel, request_config, out_msg_request, block_sequence_counter, transfer_request_parameter_record, transfer_request_parameter_record_size);
}

uds_status PcanUdsHandler::SvcRequestTransferExit(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE* transfer_request_parameter_record, DWORD transfer_request_parameter_record_size) { // Changed class name
    if (!loaded_successfully_ || !m_UDS_SvcRequestTransferExit) {
        throw std::runtime_error("UDS_SvcRequestTransferExit_2013 function not found.");
    }
    return m_UDS_SvcRequestTransferExit(channel, request_config, out_msg_request, transfer_request_parameter_record, transfer_request_parameter_record_size);
} 