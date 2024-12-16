#include <windows.h>
#include <stdio.h>

#define CMD_BUFFER_SIZE 256

// Forward declarations
void StartShell();
void ExecuteCommand(const char *cmd);
void TaskManager();
void FileExplorer();
void Notepad();
void Calculator();
void Settings();
void AboutMicroNT();
void DisplayWinver(HWND hwnd);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinverProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create a simple window
    const char *CLASS_NAME = "MicroNT";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "MicroNT - Portable Windows NT",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Failed to create window.", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Start shell interaction in a separate thread
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartShell, NULL, 0, NULL);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void StartShell() {
    char command[CMD_BUFFER_SIZE];

    printf("Welcome to MicroNT Shell\n");
    printf("Type 'help' for a list of commands\n\n");

    while (1) {
        printf("MicroNT> ");
        fgets(command, CMD_BUFFER_SIZE, stdin);

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            printf("Exiting shell...\n");
            ExitProcess(0);
        }

        ExecuteCommand(command);
    }
}

void ExecuteCommand(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        printf("Available commands:\n");
        printf("  help         - Show available commands\n");
        printf("  taskmgr      - Open Task Manager\n");
        printf("  explorer     - Open File Explorer\n");
        printf("  notepad      - Open Notepad\n");
        printf("  calc         - Open Calculator\n");
        printf("  settings     - Open Settings\n");
        printf("  about        - Open About MicroNT (winver-like)\n");
        printf("  exit         - Exit the shell\n");
    } else if (strcmp(cmd, "taskmgr") == 0) {
        TaskManager();
    } else if (strcmp(cmd, "explorer") == 0) {
        FileExplorer();
    } else if (strcmp(cmd, "notepad") == 0) {
        Notepad();
    } else if (strcmp(cmd, "calc") == 0) {
        Calculator();
    } else if (strcmp(cmd, "settings") == 0) {
        Settings();
    } else if (strcmp(cmd, "about") == 0) {
        AboutMicroNT();
    } else {
        printf("Unknown command: %s\n", cmd);
    }
}

// About MicroNT (winver-like program)
void AboutMicroNT() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WinverProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "Winver";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, "Winver", "About MicroNT", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WinverProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Draw the logo
        HFONT hFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                 ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                 DEFAULT_PITCH | FF_SWISS, "Arial");
        SelectObject(hdc, hFont);

        TextOut(hdc, 50, 50, "MicroNT", 7);

        HFONT hSubFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                    ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                    DEFAULT_PITCH | FF_SWISS, "Arial");
        SelectObject(hdc, hSubFont);
        TextOut(hdc, 50, 100, "Portable Windows NT 4.0 Clone", 29);
        TextOut(hdc, 50, 130, "Version: 1.0.0", 14);
        TextOut(hdc, 50, 160, "(Not affiliated with Microsoft)", 31);

        DeleteObject(hFont);
        DeleteObject(hSubFont);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
