#include "pcan_uds_handler.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Attempting to initialize PCAN-UDS..." << std::endl;

    try {
        PcanUdsHandler pcan_uds;

        if (!pcan_uds.isLoaded()) {
            std::cerr << "Failed to load PCAN-UDS library or resolve functions. Exiting." << std::endl;
            return 1;
        }

        // --- Test UDS Channel Initialization (Non-FD) ---
        cantp_handle uds_channel = PCANTP_HANDLE_USBBUS1; // Example: Use USB BUS 1 for UDS. Changed from PUDS_USBBUS1
        cantp_baudrate uds_bitrate = PCANTP_BAUDRATE_500K; // Example: 500 kbit/s for UDS. Changed from PUDS_BAUD_500K
        // For UDS_Initialize_2013, HwType, IOPort, Interrupt are often 0 unless specific hardware is used.
        cantp_hwtype uds_hw_type = (cantp_hwtype)0; 
        DWORD uds_io_port = 0;
        WORD uds_interrupt = 0;

        std::cout << "Initializing UDS channel " << std::hex << uds_channel 
                  << " at " << std::dec << 500 << " kbit/s..." << std::endl;
                  
        pcan_uds.InitializeChannel(uds_channel, uds_bitrate, uds_hw_type, uds_io_port, uds_interrupt);
        std::cout << "UDS channel initialized successfully." << std::endl;

        std::cout << "Uninitializing UDS channel..." << std::endl;
        pcan_uds.UninitializeChannel(uds_channel);
        std::cout << "UDS channel uninitialized." << std::endl;

        // --- Test UDS Channel FD Initialization (Example) ---
        // Note: Requires appropriate hardware and license for CAN FD
        
        cantp_handle uds_fd_channel = PCANTP_HANDLE_USBBUS2; // Example: Use USB BUS 2 for UDS FD. Changed from PUDS_USBBUS2
        // Example FD bitrate string for UDS (adjust according to PCAN-UDS documentation and your hardware)
        std::string uds_fd_bitrate = "f_clock_mhz=80, nom_brp=2, nom_tseg1=63, nom_tseg2=16, nom_sjw=16, data_brp=2, data_tseg1=15, data_tseg2=4, data_sjw=4";
        // 500k + 2M example: "f_clock_mhz=80, nom_brp=10, nom_tseg1=12, nom_tseg2=3, nom_sjw=3, data_brp=4, data_tseg1=7, data_tseg2=2, data_sjw=1"
        
        std::cout << "\nInitializing UDS FD channel " << std::hex << uds_fd_channel 
                  << " with bitrate: " << uds_fd_bitrate << "..." << std::endl;

        pcan_uds.InitializeChannelFD(uds_fd_channel, uds_fd_bitrate);
        std::cout << "UDS FD channel initialized successfully." << std::endl;

        std::cout << "Uninitializing UDS FD channel..." << std::endl;
        pcan_uds.UninitializeChannel(uds_fd_channel);
        std::cout << "UDS FD channel uninitialized." << std::endl;
        

    } catch (const std::runtime_error& e) {
        std::cerr << "\n*** PCAN-UDS Error: " << e.what() << " ***" << std::endl;
        return 1; // Indicate failure
    } catch (...) {
        std::cerr << "\n*** An unexpected error occurred. ***" << std::endl;
        return 1;
    }

    std::cout << "\nPCAN-UDS test finished successfully." << std::endl;
    return 0; // Indicate success
} 