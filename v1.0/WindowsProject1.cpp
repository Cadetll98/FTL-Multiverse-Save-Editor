// WindowsProject1.cpp : Defines the entry point for the application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#include "WindowsProject1.h"
#include "Editor.h"

// Global Variables:
int nHull = 0, nFuel = 0, nMissle = 0, nDrone = 0, nScrap = 0;
FILE* infile = NULL;
FILE* outfile = NULL;
HWND fld_Amt[5];
bool modified = false;
Save sav;

HWND hWnd;  // root window handle
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING] = L"FTL Save Editor";                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING] = L"TestWindow";            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void openSav(Save& sav);
void writeSav(Save& sav);
void createAmtField();
void createControl();
void fileClosure();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Register Windows Class
    MyRegisterClass(hInstance);
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // Load savefile and create static windows/controls
    openSav(sav); 

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));
    MSG msg;
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 400, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        MessageBox(nullptr, L"Windows Creation Failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case ADDHULL:
            nHull += HULL_INCRE;
            nHull = nHull > MAX_HULL ? MAX_HULL : nHull;
            SetWindowTextW(fld_Amt[0], std::to_wstring(nHull).c_str());
            break;
        case ADDFUEL:
            nFuel += FMD_INCRE;
            nFuel = nFuel > MAX_FMD ? MAX_FMD : nFuel;
            SetWindowTextW(fld_Amt[1], std::to_wstring(nFuel).c_str());
            break;
        case ADDMISSLE:
            nMissle += FMD_INCRE;
            nMissle = nMissle > MAX_FMD ? MAX_FMD : nMissle;
            SetWindowTextW(fld_Amt[2], std::to_wstring(nMissle).c_str());
            break;
        case ADDDRONE:
            nDrone += FMD_INCRE;
            nDrone = nDrone > MAX_FMD ? MAX_FMD : nDrone;
            SetWindowTextW(fld_Amt[3], std::to_wstring(nDrone).c_str());
            break;
        case ADDSCRAP:
            nScrap += SCRAP_INCRE;
            nScrap = nScrap > MAX_SCRAP ? MAX_SCRAP : nScrap;
            SetWindowTextW(fld_Amt[4], std::to_wstring(nScrap).c_str());
            break;
        case SAVEFILE:  // save file
            writeSav(sav);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        fileClosure();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


// Open Save file and create controls
void openSav(Save& sav) {
    int selec = IDRETRY;
    unsigned char* buffer;

    while (selec == IDRETRY && infile == NULL) {
        infile = fopen("hs_mv_continue.sav", "rb");
        if (infile == NULL) {
            selec = MessageBox(hWnd, L"Unable to open infile!", L"Error", MB_RETRYCANCEL);
        }
    }

    if (infile != NULL) {        
        readSavefile(infile, sav);

        nHull = stoi(sav.mData.at("Hull"));
        nFuel = stoi(sav.mData.at("Fuel"));
        nMissle = stoi(sav.mData.at("Missle"));
        nDrone = stoi(sav.mData.at("Drone"));
        nScrap = stoi(sav.mData.at("Scrap"));

        createControl();
        createAmtField();
    }
    else {
        DestroyWindow(hWnd);
    }
}

// Create Amt Field
void createAmtField() {
    fld_Amt[0] = CreateWindowW(L"STATIC", to_wstring(nHull).c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 30, 50, 20, hWnd, nullptr, hInst, nullptr);
    fld_Amt[1] = CreateWindowW(L"STATIC", to_wstring(nFuel).c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 60, 50, 20, hWnd, nullptr, hInst, nullptr);
    fld_Amt[2] = CreateWindowW(L"STATIC", to_wstring(nMissle).c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 90, 50, 20, hWnd, nullptr, hInst, nullptr);
    fld_Amt[3] = CreateWindowW(L"STATIC", to_wstring(nDrone).c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 120, 50, 20, hWnd, nullptr, hInst, nullptr);
    fld_Amt[4] = CreateWindowW(L"STATIC", to_wstring(nScrap).c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 150, 50, 20, hWnd, nullptr, hInst, nullptr);
}

// Create Control
void createControl() {
    HWND fld_Save;
    HWND fld_Add_Hull, fld_Add_Fuel, fld_Add_Missle, fld_Add_Drone, fld_Add_Scrap;

    // Hull
    CreateWindowW(L"STATIC", L"Hull", WS_VISIBLE | WS_CHILD, 30, 30, 50, 20, hWnd, nullptr, hInst, nullptr);
    fld_Add_Hull = CreateWindowW(L"BUTTON", L"Add 1", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 150, 30, 60, 20, hWnd, (HMENU)ADDHULL, hInst, nullptr);
    // Fuel
    CreateWindowW(L"STATIC", L"Fuel", WS_VISIBLE | WS_CHILD, 30, 60, 50, 20, hWnd, nullptr, hInst, nullptr);
    fld_Add_Fuel = CreateWindowW(L"BUTTON", L"Add 20", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 150, 60, 60, 20, hWnd, (HMENU)ADDFUEL, hInst, nullptr);
    // Missle
    CreateWindowW(L"STATIC", L"Missle", WS_VISIBLE | WS_CHILD, 30, 90, 50, 20, hWnd, nullptr, hInst, nullptr);
    fld_Add_Missle = CreateWindowW(L"BUTTON", L"Add 20", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 150, 90, 60, 20, hWnd, (HMENU)ADDMISSLE, hInst, nullptr);
    // Drone
    CreateWindowW(L"STATIC", L"Drone", WS_VISIBLE | WS_CHILD, 30, 120, 50, 20, hWnd, nullptr, hInst, nullptr);
    fld_Add_Drone = CreateWindowW(L"BUTTON", L"Add 20", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 150, 120, 60, 20, hWnd, (HMENU)ADDDRONE, hInst, nullptr);
    // Scrap
    CreateWindowW(L"STATIC", L"Scrap", WS_VISIBLE | WS_CHILD, 30, 150, 50, 20, hWnd, nullptr, hInst, nullptr);
    fld_Add_Scrap = CreateWindowW(L"BUTTON", L"Add 50", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 150, 150, 60, 20, hWnd, (HMENU)ADDSCRAP, hInst, nullptr);

    fld_Save = CreateWindowW(L"BUTTON", L"Save", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 300, 400, 50, 20, hWnd, (HMENU)SAVEFILE, hInst, nullptr);
}

// Open backup.sav and write updated data to it
void writeSav(Save& sav) {
    bool bWriteFailed;

    outfile = fopen("hs_mv_continue_out.sav", "wb");
    if (outfile == NULL) {
        if (MessageBox(hWnd, L"Unable to create outfile ... Exit Now?", L"Error", MB_YESNO) == IDYES) {
            DestroyWindow(hWnd);
        }
    }
    else {
        sav.mData.at("Hull") = to_string(nHull);
        sav.mData.at("Fuel") = to_string(nFuel);
        sav.mData.at("Missle") = to_string(nMissle);
        sav.mData.at("Drone") = to_string(nDrone);
        sav.mData.at("Scrap") = to_string(nScrap);

        // save to file
        bWriteFailed = writeSavefile(outfile,sav);
        if (bWriteFailed) {
            modified = false;
            if (MessageBox(hWnd, L"Unable to write to outfile ... Exit Now?", L"Error", MB_YESNO) == IDYES) {
                DestroyWindow(hWnd);
            }
        }
        else {
            // exited after pressing save
            modified = true;
            MessageBox(hWnd, L"Write Successful! ... Exit Now?", L"Error", MB_OK);
            DestroyWindow(hWnd);
        }
    }
}

// Manage Remove/Rename of savfiles and close file stream when closing window
void fileClosure() {
    if (infile) {
        fclose(infile);
    }
    else {
        return;
    }
    if (outfile) {
        fclose(outfile);
    }
    else {
        return;
    }
    if (modified) {
        FILE* fCheck;
        if (fCheck = fopen("hs_mv_continue_backup.sav", "r")) {
            fclose(fCheck);
            remove("hs_mv_continue_backup.sav");
        }

        if (rename("hs_mv_continue.sav", "hs_mv_continue_backup.sav") != 0) {
            MessageBox(hWnd, L"Failed to rename continue.sav to backup.sav!", L"Error", MB_OK);

        }
        if (rename("hs_mv_continue_out.sav", "hs_mv_continue.sav") != 0) {
            MessageBox(hWnd, L"Failed to rename continue_out.sav to continue.sav!", L"Error", MB_OK);
        }
    }
    else {
        // delete unused output file
        if (remove("hs_mv_continue_out.sav") != 0) {
            MessageBox(hWnd, L"Failed to delete output file!", L"Error", MB_OK);
        }
    }
}