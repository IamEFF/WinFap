#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<tchar.h>
#include"Resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static int scrlh, scrlv, lIndex, cIndex;
int* val[] = { &scrlh, &scrlv, &lIndex, &cIndex };
TCHAR number[150];
LPCWSTR list[];
HWND hwnd;

const int HNUM = 10, VNUM = 100;
const int List_size = 20, Combo_size = 20;
const int INTERVAL = 20;
INT_PTR CALLBACK Dialog1(HWND, UINT, WPARAM, LPARAM);
TCHAR szAppName[] = _T("BigDialog");
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hWnd;
    WNDCLASSEX wc;
    MSG lpMsg;
    hInst = hInstance;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szAppName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    if (!RegisterClassEx(&wc)) return 0;
    hWnd = CreateWindow(szAppName, _T("Демонстрация списков"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, (HWND)NULL, (HMENU)NULL,
        hInstance, (LPSTR)NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&lpMsg, NULL, 0, 0)) {
        TranslateMessage(&lpMsg);
        DispatchMessageW(&lpMsg);
    }
    return (int)lpMsg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR str[256];

    HMENU hMenu, hPopupMenu;

    TCHAR str0[] = _T("Диалог");
    TCHAR str1[] = _T("Открыть");
    switch (msg)
    {
    case WM_CREATE:
        hMenu = CreateMenu();
        hPopupMenu = CreatePopupMenu();
        AppendMenu(hPopupMenu, MF_STRING, ID_STDDIALOG, str1);
        AppendMenu(hMenu, MF_POPUP, (UINT)hPopupMenu, str0);
        SetMenu(hwnd, hMenu);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_STDDIALOG:
            DialogBox(hInst, MAKEINTRESOURCE(ID_STDDIALOG), hwnd, Dialog1);
            break;
        case IDM_EXIT:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK Dialog1(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int scrlv;
    static HWND vScroll, hWndList;
    HDC hdc;


    HBRUSH hBrushStatic = CreateSolidBrush(RGB(0, 255, 0));
    HBRUSH hBrushList = CreateSolidBrush(RGB(0, 255, 0));


    switch (msg)
    {
    case WM_CTLCOLORDLG:

        return (LONG)hBrushList;
    case WM_CTLCOLORSTATIC:
        hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(0, 0, 255));
        return (LONG)hBrushStatic;
    case WM_CTLCOLORLISTBOX:
        SetTextColor((HDC)wParam, RGB(255, 0, 0));
        return (LRESULT)CreateSolidBrush(RGB(0, 255, 0));

    case WM_INITDIALOG:
        scrlv = ::scrlv;
        SetDlgItemInt(hDlg, IDC_VSCR, scrlv, 0);
        vScroll = GetDlgItem(hDlg, IDC_SCROLLBAR2);
        SetScrollRange(vScroll, SB_CTL, 0, VNUM, FALSE);
        SetScrollPos(vScroll, SB_CTL, scrlv, TRUE);
        hWndList = GetDlgItem(hDlg, IDC_LIST1);

        for (int i = 1; i <= 99; i++)
        {
            _itot(i, number, 10);
            SendMessage(hWndList, LB_ADDSTRING, 1, (LPARAM)number);
        }
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            case IDOK:
            lIndex = SendMessage(hWndList, LB_GETCURSEL, 0, 0);
            ::scrlv = scrlv;
            InvalidateRect(GetParent(hDlg), NULL, TRUE);
        case IDCANCEL:
            return EndDialog(hDlg, 0);
        }
        return TRUE;

    case WM_VSCROLL:
        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            scrlv--;
            break;
        case SB_LINEDOWN:
            scrlv++;
            break;
        case SB_PAGEUP:
            scrlv -= VNUM / 10;
            break;
        case SB_PAGEDOWN:
            scrlv += VNUM / 10;
            break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
            scrlv =
                HIWORD(wParam);
            break;
        }
        scrlv = max(0, min(scrlv, VNUM));
        if (scrlv != GetScrollPos(vScroll, SB_CTL)) {
            SetScrollPos(vScroll, SB_CTL, scrlv, TRUE);
            SetDlgItemInt(hDlg, IDC_VSCR, scrlv, 0);
        }
        return TRUE;
    default:
        return FALSE;
    }
    return FALSE;
}