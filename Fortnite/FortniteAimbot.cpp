#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>

#define BASE 0x00400000
#define POS_OFFSET 0x01A7EC70
#define ENEMY_OFFSET 0x01A7ECC8
#define AIM_X_OFFSET 0x01444960
#define AIM_Y_OFFSET 0x01444964
#define SPEED 10.0f
#define RANGE 1000.0f

struct Vector3 {
    float x, y, z;
};

Vector3 getPlayerPos(HANDLE hProc, DWORD base, DWORD off) {
    Vector3 pos;
    if (ReadProcessMemory(hProc, (LPVOID)(base + off), &pos, sizeof(Vector3), NULL) == 0) {
        // handle error
    }
    return pos;
}

std::vector<Vector3> getEnemyPos(HANDLE hProc, DWORD base, DWORD off) {
    std::vector<Vector3> pos(10); // assuming there are 10 enemies
    if (ReadProcessMemory(hProc, (LPVOID)(base + off), pos.data(), pos.size() * sizeof(Vector3), NULL) == 0) {
        // handle error
    }
    return pos;
}

float dist(const Vector3& a, const Vector3& b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
}

void aim(HANDLE hProc, DWORD base, float x, float y) {
    float curX, curY;
    if (ReadProcessMemory(hProc, (LPVOID)(base + AIM_X_OFFSET), &curX, sizeof(float), NULL) == 0 ||
        ReadProcessMemory(hProc, (LPVOID)(base + AIM_Y_OFFSET), &curY, sizeof(float), NULL) == 0) {
        // handle error
    }
    float newX = curX + (x - curX) / SPEED;
    float newY = curY + (y - curY) / SPEED;
    if (WriteProcessMemory(hProc, (LPVOID)(base + AIM_X_OFFSET), &newX, sizeof(float), NULL) == 0 ||
        WriteProcessMemory(hProc, (LPVOID)(base + AIM_Y_OFFSET), &newY, sizeof(float), NULL) == 0) {
        // handle error
    }
}

int main() {
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    if (hProc == NULL) {
        // handle error
        return 1;
    }
    DWORD base = (DWORD)GetModuleHandleA(NULL);
    while (true) {
        Vector3 player = getPlayerPos(hProc, base, POS_OFFSET);
        std::vector<Vector3> enemies = getEnemyPos(hProc, base, ENEMY_OFFSET);
        if (!enemies.empty()) {
            auto closest = std::min_element(enemies.begin(), enemies.end(), [&player](const Vector3& a, const Vector3& b) {
                return dist(player, a) < dist(player, b);
            });
            if (closest != enemies.end() && dist(player, *closest) < RANGE) {
                aim(hProc, base, closest->x, closest->y);
            }
        }
        Sleep(1);
    }
    CloseHandle(hProc);
    return 0;
}
