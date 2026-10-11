#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <iostream>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <thread>
#include <unistd.h>
#endif

namespace Utils {

    constexpr double PI = 3.14159265358979323846;

    // Cross-platform thread launcher
    typedef void (*ThreadFunc)(void*);

    inline void sleepMs(int ms) {
#ifdef _WIN32
        Sleep(ms);
#else
        usleep(ms * 1000);
#endif
    }

    struct ThreadHandle {
#ifdef _WIN32
        HANDLE handle;
        DWORD id;
#else
        std::thread th;
#endif
    };

#ifdef _WIN32
    struct ThreadBridge {
        ThreadFunc func;
        void* param;
    };

    inline DWORD WINAPI ThreadProxy(LPVOID lpParam) {
        ThreadBridge* bridge = static_cast<ThreadBridge*>(lpParam);
        if (bridge && bridge->func) {
            bridge->func(bridge->param);
        }
        delete bridge;
        return 0;
    }
#endif

    inline void runInBackground(ThreadFunc func, void* param = nullptr) {
#ifdef _WIN32
        ThreadBridge* bridge = new ThreadBridge{func, param};
        HANDLE h = CreateThread(nullptr, 0, ThreadProxy, bridge, 0, nullptr);
        if (h) {
            CloseHandle(h); // Detached
        }
#else
        std::thread t([func, param]() {
            func(param);
        });
        t.detach();
#endif
    }

    inline std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* tm_ptr = std::localtime(&now_c);
        std::ostringstream oss;
        if (tm_ptr) {
            oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
        } else {
            oss << "2026-10-11 12:00:00";
        }
        return oss.str();
    }

    inline std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    inline std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(trim(token));
        }
        return tokens;
    }

    inline void printHeader(const std::string& title) {
        std::cout << "\n======================================================================\n";
        std::cout << "  " << title << "\n";
        std::cout << "======================================================================\n";
    }

    inline void printSubHeader(const std::string& title) {
        std::cout << "\n----------------------------------------------------------------------\n";
        std::cout << "  " << title << "\n";
        std::cout << "----------------------------------------------------------------------\n";
    }

    inline void pauseConsole() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(10000, '\n');
    }

} // namespace Utils

#endif // UTILS_H
