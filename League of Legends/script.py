import ctypes
import time

# LOL Game Handle
HANDLE = ctypes.windll.kernel32.OpenProcess(0x1F0FFF, False, 1234)  # replace 1234 with LOL process ID

# LOL Game Module Base Address
MODULE_BASE = 0x00400000

# LOL Game Player Base Address
PLAYER_BASE = MODULE_BASE + 0x01A7EC70

# LOL Game Enemy Base Address
ENEMY_BASE = MODULE_BASE + 0x01A7ECC8

while True:
    # Get Player Position
    player_x = ctypes.c_float()
    player_y = ctypes.c_float()
    player_z = ctypes.c_float()
    ctypes.windll.kernel32.ReadProcessMemory(HANDLE, PLAYER_BASE + 0x00, ctypes.byref(player_x), 4, None)
    ctypes.windll.kernel32.ReadProcessMemory(HANDLE, PLAYER_BASE + 0x04, ctypes.byref(player_y), 4, None)
    ctypes.windll.kernel32.ReadProcessMemory(HANDLE, PLAYER_BASE + 0x08, ctypes.byref(player_z), 4, None)

    # Get Enemy Position
    enemy_x = ctypes.c_float()
    enemy_y = ctypes.c_float()
    enemy_z = ctypes.c_float()
    ctypes.windll.kernel32.ReadProcessMemory(HANDLE, ENEMY_BASE + 0x00, ctypes.byref(enemy_x), 4, None)
    ctypes.windll.kernel32.ReadProcessMemory(HANDLE, ENEMY_BASE + 0x04, ctypes.byref(enemy_y), 4, None)
    ctypes.windll.kernel32.ReadProcessMemory(HANDLE, ENEMY_BASE + 0x08, ctypes.byref(enemy_z), 4, None)

    # Calculate Distance between Player and Enemy
    distance = ((enemy_x.value - player_x.value) ** 2 + (enemy_y.value - player_y.value) ** 2 + (enemy_z.value - player_z.value) ** 2) ** 0.5

    # If Distance is less than 1000 units, aim at enemy
    if distance < 1000:
        # Calculate Aim Angle
        angle_x = (enemy_x.value - player_x.value) / distance
        angle_y = (enemy_y.value - player_y.value) / distance
        angle_z = (enemy_z.value - player_z.value) / distance

        # Set Aim Angle
        ctypes.windll.kernel32.WriteProcessMemory(HANDLE, PLAYER_BASE + 0x10, ctypes.byref(ctypes.c_float(angle_x)), 4, None)
        ctypes.windll.kernel32.WriteProcessMemory(HANDLE, PLAYER_BASE + 0x14, ctypes.byref(ctypes.c_float(angle_y)), 4, None)
        ctypes.windll.kernel32.WriteProcessMemory(HANDLE, PLAYER_BASE + 0x18, ctypes.byref(ctypes.c_float(angle_z)), 4, None)

    time.sleep(0.01)
