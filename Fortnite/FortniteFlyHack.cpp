#include <windows.h>
#include <vector>
#include <algorithm>

// Define the base address of the game executable
DWORD baseAddress = 0x00400000;

// Define the offset to the player gravity
DWORD playerGravityOffset = 0x01A7EC80;

// Function to retrieve the player gravity
float getPlayerGravity(HANDLE hProcess, DWORD baseAddress, DWORD playerGravityOffset) {
    float playerGravity;
    ReadProcessMemory(hProcess, (LPVOID)(baseAddress + playerGravityOffset), &playerGravity, sizeof(float), NULL);
    return playerGravity;
}

// Function to set the player gravity
void setPlayerGravity(HANDLE hProcess, DWORD baseAddress, DWORD playerGravityOffset, float newGravity) {
    WriteProcessMemory(hProcess, (LPVOID)(baseAddress + playerGravityOffset), &newGravity, sizeof(float), NULL);
}

int main() {
    // Get the handle to the game process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

    // Get the base address of the game executable
    baseAddress = (DWORD)GetModuleHandleA(NULL);

    // Set the player gravity to 0 (fly mode)
    setPlayerGravity(hProcess, baseAddress, playerGravityOffset, 0.0f);

    while (true) {
        // Keep the player gravity at 0
        setPlayerGravity(hProcess, baseAddress, playerGravityOffset, 0.0f);

        // Sleep for 1ms
        Sleep(1);
    }

    // Close the handle to the game process
    CloseHandle(hProcess);

    return 0;
}
