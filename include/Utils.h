#pragma once
#define NOMINMAX
#include <string>
#include <windows.h>
#include <iostream>

namespace Utils {
    // ANSI Pastel Colors
    const std::string RESET = "\033[0m";
    const std::string PASTEL_CYAN = "\033[38;5;159m";
    const std::string PASTEL_GREEN = "\033[38;5;120m";
    const std::string PASTEL_RED = "\033[38;5;210m";
    const std::string PASTEL_YELLOW = "\033[38;5;229m";
    const std::string PASTEL_PINK = "\033[38;5;218m";
    const std::string PASTEL_PURPLE = "\033[38;5;141m";
    const std::string BOLD = "\033[1m";

    inline void EnableANSI() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    inline void PrintBanner() {
        std::cout << PASTEL_PINK << BOLD;
        std::cout << R"(
   _____           _       _   _____              
  / ____|         (_)     | | / ____|             
 | (___   ___ _ __ _ _ __ | || (___  _ __ ___     
  \___ \ / __| '__| | '_ \| __\___ \| '__/ __|    
  ____) | (__| |  | | |_) | |_____) | | | (__     
 |_____/ \___|_|  |_| .__/ \__|_____/|_|  \___|   
                    | |                           
                    |_|
              CLUMP EXTRACTOR                          
    )" << std::endl;
        std::cout << "Created by " << PASTEL_PINK << "Lil Poop" << PASTEL_PINK << " <3" << RESET << std::endl;
        std::cout << PASTEL_CYAN << "--------------------------------------------------" << RESET << std::endl;
        std::cout << std::endl;
    }
}
