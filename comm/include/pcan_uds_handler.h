#ifndef PCAN_UDS_HANDLER_H
#define PCAN_UDS_HANDLER_H

#include <string>
#include <windows.h> // For HMODULE, DWORD, WORD, BYTE etc.
#include "pcan/PCAN-UDS_2013.h" // Include UDS header

// Define function pointer types for PCAN-UDS DLL functions
// Core functions
typedef uds_status (__stdcall *UDS_Initialize_t)(cantp_handle Channel, cantp_baudrate Bitrate, cantp_hwtype HwType, DWORD IOPort, WORD Interrupt);
typedef uds_status (__stdcall *UDS_InitializeFD_t)(cantp_handle Channel, const cantp_bitrate BitrateFD);
typedef uds_status (__stdcall *UDS_Uninitialize_t)(cantp_handle Channel);
typedef uds_status (__stdcall *UDS_Reset_t)(cantp_handle Channel);
typedef uds_status (__stdcall *UDS_GetErrorText_t)(uds_status ErrorCode, WORD Language, char* Buffer);
typedef uds_status (__stdcall *UDS_Write_t)(cantp_handle Channel, uds_msg* MessageBuffer);
typedef uds_status (__stdcall *UDS_Read_t)(cantp_handle Channel, uds_msg* OutMessageBuffer, uds_msg* InMessageRequest, cantp_timestamp* OutTimestamp);
typedef uds_status (__stdcall *UDS_MsgAlloc_t)(uds_msg* MessageBuffer, uds_msgconfig MessageConfiguration, uint32_t MessageDataLength);
typedef uds_status (__stdcall *UDS_MsgFree_t)(uds_msg* MessageBuffer);
typedef uds_status (__stdcall *UDS_GetValue_t)(cantp_handle Channel, uds_parameter Parameter, void* Buffer, uint32_t BufferSize);
typedef uds_status (__stdcall *UDS_SetValue_t)(cantp_handle Channel, uds_parameter Parameter, void* Buffer, uint32_t BufferSize);
typedef uds_status (__stdcall *UDS_WaitForService_t)(cantp_handle Channel, uds_msg* MessageRequest, uds_msg* OutMessageResponse, uds_msg* OutMessageRequestConfirmation);

// Service functions (selected for bootloader)
typedef uds_status (__stdcall *UDS_SvcDiagnosticSessionControl_t)(cantp_handle Channel, uds_msgconfig RequestConfig, uds_msg* OutMessageRequest, BYTE SessionType);
typedef uds_status (__stdcall *UDS_SvcECUReset_t)(cantp_handle Channel, uds_msgconfig RequestConfig, uds_msg* OutMessageRequest, BYTE ResetType);
typedef uds_status (__stdcall *UDS_SvcSecurityAccess_t)(cantp_handle Channel, uds_msgconfig RequestConfig, uds_msg* OutMessageRequest, BYTE SecurityAccessType, BYTE* SecurityAccessDataRecord, DWORD SecurityAccessDataRecordSize);
typedef uds_status (__stdcall *UDS_SvcRequestDownload_t)(
    cantp_handle Channel, 
    uds_msgconfig RequestConfig, 
    uds_msg* OutMessageRequest, 
    BYTE compression_method,
    BYTE encrypting_method,
    BYTE* memory_address_buffer,
    BYTE memory_address_size,
    BYTE* memory_size_buffer,
    BYTE memory_size_size
);
typedef uds_status (__stdcall *UDS_SvcTransferData_t)(cantp_handle Channel, uds_msgconfig RequestConfig, uds_msg* OutMessageRequest, BYTE BlockSequenceCounter, BYTE* TransferRequestParameterRecord, DWORD TransferRequestParameterRecordSize);
typedef uds_status (__stdcall *UDS_SvcRequestTransferExit_t)(cantp_handle Channel, uds_msgconfig RequestConfig, uds_msg* OutMessageRequest, BYTE* TransferRequestParameterRecord, DWORD TransferRequestParameterRecordSize);


class PcanUdsHandler { // Renamed from PcanInitializer
public:
    PcanUdsHandler();
    ~PcanUdsHandler();

    bool isLoaded() const { return loaded_successfully_; }

    // Channel initialization and uninitialization using UDS types
    cantp_handle InitializeChannel(cantp_handle channel, cantp_baudrate bitrate, cantp_hwtype hwType = (cantp_hwtype)0, DWORD ioPort = 0, WORD interrupt = 0);
    cantp_handle InitializeChannelFD(cantp_handle channel, const std::string& bitrateFD);
    void UninitializeChannel(cantp_handle channel);
    void ResetChannel(cantp_handle channel);

    // Error handling using UDS types
    std::string GetErrorString(uds_status status);

    // UDS Message Handling
    uds_status AllocateMessage(uds_msg* msg_buffer, uds_msgconfig msg_configuration, uint32_t msg_data_length);
    uds_status FreeMessage(uds_msg* msg_buffer);
    uds_status WriteMessage(cantp_handle channel, uds_msg* msg_buffer);
    uds_status ReadMessage(cantp_handle channel, uds_msg* out_msg_buffer, uds_msg* in_msg_request = nullptr, cantp_timestamp* out_timestamp = nullptr);
    uds_status WaitForService(cantp_handle channel, uds_msg* msg_request, uds_msg* out_msg_response, uds_msg* out_msg_request_confirmation = nullptr);


    // UDS Services (selected for bootloader)
    uds_status SvcDiagnosticSessionControl(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE session_type);
    uds_status SvcECUReset(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE reset_type);
    uds_status SvcSecurityAccess(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE security_access_type, BYTE* security_access_data, DWORD security_access_data_size);
    uds_status SvcRequestDownload(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, 
                                  BYTE compression_method, BYTE encrypting_method, 
                                  BYTE* memory_address_buffer, BYTE memory_address_size, 
                                  BYTE* memory_size_buffer, BYTE memory_size_size);
    uds_status SvcTransferData(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE block_sequence_counter, BYTE* transfer_request_parameter_record, DWORD transfer_request_parameter_record_size);
    uds_status SvcRequestTransferExit(cantp_handle channel, uds_msgconfig request_config, uds_msg* out_msg_request, BYTE* transfer_request_parameter_record, DWORD transfer_request_parameter_record_size);

private:
    bool loadPcanLibrary();
    void freePcanLibrary();

    HMODULE pcan_dll_handle_;
    bool loaded_successfully_;

    // PCAN-UDS function pointers
    UDS_Initialize_t m_UDS_Initialize;
    UDS_InitializeFD_t m_UDS_InitializeFD;
    UDS_Uninitialize_t m_UDS_Uninitialize;
    UDS_GetErrorText_t m_UDS_GetErrorText;
    UDS_Write_t m_UDS_Write;
    UDS_Read_t m_UDS_Read;
    UDS_MsgAlloc_t m_UDS_MsgAlloc;
    UDS_MsgFree_t m_UDS_MsgFree;
    UDS_Reset_t m_UDS_Reset;
    UDS_WaitForService_t m_UDS_WaitForService;
    // UDS_GetValue_t m_UDS_GetValue; // If needed
    // UDS_SetValue_t m_UDS_SetValue; // If needed

    // Service function pointers
    UDS_SvcDiagnosticSessionControl_t m_UDS_SvcDiagnosticSessionControl;
    UDS_SvcECUReset_t m_UDS_SvcECUReset;
    UDS_SvcSecurityAccess_t m_UDS_SvcSecurityAccess;
    UDS_SvcRequestDownload_t m_UDS_SvcRequestDownload;
    UDS_SvcTransferData_t m_UDS_SvcTransferData;
    UDS_SvcRequestTransferExit_t m_UDS_SvcRequestTransferExit;
};

#endif // PCAN_UDS_HANDLER_H 