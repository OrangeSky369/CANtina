#include "pcan_initializer.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Attempting to initialize PCAN..." << std::endl;

    try {
        PcanInitializer pcan;

        // --- Test CAN Initialization ---
        TPCANHandle can_channel = PCAN_USBBUS1; // Example: Use USB BUS 1
        TPCANBaudrate can_bitrate = PCAN_BAUD_500K; // Example: 500 kbit/s

        std::cout << "Initializing CAN channel " << std::hex << can_channel 
                  << " at " << std::dec << 500 << " kbit/s..." << std::endl;
                  
        pcan.InitializeChannel(can_channel, can_bitrate);
        std::cout << "CAN channel initialized successfully." << std::endl;

        std::cout << "Uninitializing CAN channel..." << std::endl;
        pcan.UninitializeChannel(can_channel);
        std::cout << "CAN channel uninitialized." << std::endl;

        // --- Test CAN FD Initialization (Example) ---
        // Note: Requires appropriate hardware and license for CAN FD
        /*
        TPCANHandle fd_channel = PCAN_USBBUS1; // Can use the same handle if supported
        // Example FD bitrate string (adjust according to PCANBasic documentation and your hardware)
        std::string fd_bitrate = "f_clock_mhz=80, nom_brp=2, nom_tseg1=63, nom_tseg2=16, nom_sjw=16, data_brp=2, data_tseg1=15, data_tseg2=4, data_sjw=4";
        500 + 2M: "f_clock_mhz=80, nom_brp=10, nom_tseg1=12, nom_tseg2=3, nom_sjw=3, data_brp=4, data_tseg1=7, data_tseg2=2, data_sjw=1"
        
        std::cout << "\nInitializing CAN FD channel " << std::hex << fd_channel 
                  << " with bitrate: " << fd_bitrate << "..." << std::endl;

        pcan.InitializeChannelFD(fd_channel, fd_bitrate);
        std::cout << "CAN FD channel initialized successfully." << std::endl;

        std::cout << "Uninitializing CAN FD channel..." << std::endl;
        pcan.UninitializeChannel(fd_channel);
        std::cout << "CAN FD channel uninitialized." << std::endl;
        */

    } catch (const std::runtime_error& e) {
        std::cerr << "\n*** PCAN Error: " << e.what() << " ***" << std::endl;
        return 1; // Indicate failure
    } catch (...) {
        std::cerr << "\n*** An unexpected error occurred. ***" << std::endl;
        return 1;
    }

    std::cout << "\nPCAN test finished successfully." << std::endl;
    return 0; // Indicate success
} 