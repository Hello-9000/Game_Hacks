#include <windows.h>
#include <vector>
#include <algorithm>

// Define the base address of the game executable
DWORD baseAddress = 0x00400000;

// Define the offset to the enemy player rendering
DWORD enemyRenderingOffset = 0x01A7ECE0;

// Function to retrieve the enemy player rendering
int getEnemyRendering(HANDLE hProcess, DWORD baseAddress, DWORD enemyRenderingOffset) {
    int enemyRendering;
    ReadProcessMemory(hProcess, (LPVOID)(baseAddress + enemyRenderingOffset), &enemyRendering, sizeof(int), NULL);
    return enemyRendering;
}

// Function to set the enemy player rendering
void setEnemyRendering(HANDLE hProcess, DWORD baseAddress, DWORD enemyRenderingOffset, int newRendering) {
    WriteProcessMemory(hProcess, (LPVOID)(baseAddress + enemyRenderingOffset), &newRendering, sizeof(int), NULL);
}

int main() {
    // Get the handle to the game process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

    // Get the base address of the game executable
    baseAddress = (DWORD)GetModuleHandleA(NULL);

    // Set the enemy player rendering to 1 (visible)
    setEnemyRendering(hProcess, baseAddress, enemyRenderingOffset, 1);

    while (true) {
        // Keep the enemy player rendering at 1
        setEnemyRendering(hProcess, baseAddress, enemyRenderingOffset, 1);

        // Sleep for 1ms
        Sleep(1);
    }

    // Close the handle to the game process
    CloseHandle(hProcess);

    return 0;
}
