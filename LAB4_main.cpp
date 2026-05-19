#define UNICODE
#define _UNICODE
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <iomanip>
#include <numeric>

// ── IDs ──────────────────────────────────────
#define IDM_FILE_OPEN       101
#define IDM_FILE_SAVE       102
#define IDM_FILE_EXIT       103
#define IDM_ABOUT           104
#define IDC_TEXTBOX         201
#define IDC_BTN_CALC_AVG    202
#define IDC_BTN_CALC_MED    203
#define IDC_BTN_SPLIT       204
#define IDC_LABEL_STATUS    205

// ── Globals ───────────────────────────────────
HWND hTextBox, hStatus;
std::wstring gLoadedFile = L"";

// ── Person (self-contained) ───────────────────
struct Person {
    std::string firstName, surname;
    std::vector<double> hw;
    double exam = 0, finalGrade = 0;

    double average() const {
        if (hw.empty()) return 0;
        double s = 0; for (double x : hw) s += x;
        return s / hw.size();
    }
    double median() const {
        if (hw.empty()) return 0;
        std::vector<double> v = hw;
        std::sort(v.begin(), v.end());
        size_t n = v.size();
        return (n % 2 == 0) ? (v[n/2-1]+v[n/2])/2.0 : v[n/2];
    }
    void calcFinal(bool useMedian) {
        finalGrade = 0.4 * (useMedian ? median() : average()) + 0.6 * exam;
    }
};

bool sortByName(const Person& a, const Person& b) {
    return a.firstName != b.firstName ? a.firstName < b.firstName
                                      : a.surname < b.surname;
}

// ── Read students from file ───────────────────
std::vector<Person> readStudents(const std::wstring& wpath) {
    std::string path(wpath.begin(), wpath.end());
    std::ifstream f(path);
    std::vector<Person> students;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        Person p;
        std::vector<double> all;
        double v;
        ss >> p.firstName >> p.surname;
        while (ss >> v) all.push_back(v);
        if (all.empty()) continue;
        p.exam = all.back(); all.pop_back();
        p.hw = all;
        students.push_back(p);
    }
    return students;
}

// ── Format results to wstring ─────────────────
std::wstring formatResults(std::vector<Person>& students, bool useMedian) {
    for (auto& p : students) p.calcFinal(useMedian);
    std::sort(students.begin(), students.end(), sortByName);

    std::wostringstream out;
    out << std::left
        << std::setw(14) << L"Name"
        << std::setw(14) << L"Surname"
        << std::setw(14) << L"Final (Avg)"
        << std::setw(14) << L"Final (Med)"
        << L"\r\n"
        << std::wstring(56, L'-') << L"\r\n";

    for (auto& p : students) {
        double avg = 0.4 * p.average() + 0.6 * p.exam;
        double med = 0.4 * p.median()  + 0.6 * p.exam;
        std::wstring fn(p.firstName.begin(), p.firstName.end());
        std::wstring sn(p.surname.begin(),   p.surname.end());
        out << std::left
            << std::setw(14) << fn
            << std::setw(14) << sn
            << std::fixed << std::setprecision(2)
            << std::setw(14) << avg
            << std::setw(14) << med
            << L"\r\n";
    }
    return out.str();
}

// ── Open file dialog ──────────────────────────
std::wstring openFileDialog(HWND hwnd) {
    wchar_t buf[MAX_PATH] = {0};
    OPENFILENAME ofn = {};
    ofn.lStructSize  = sizeof(ofn);
    ofn.hwndOwner    = hwnd;
    ofn.lpstrFilter  = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile    = buf;
    ofn.nMaxFile     = MAX_PATH;
    ofn.Flags        = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileName(&ofn)) return std::wstring(buf);
    return L"";
}

// ── Save file dialog ──────────────────────────
std::wstring saveFileDialog(HWND hwnd) {
    wchar_t buf[MAX_PATH] = {0};
    OPENFILENAME ofn = {};
    ofn.lStructSize  = sizeof(ofn);
    ofn.hwndOwner    = hwnd;
    ofn.lpstrFilter  = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile    = buf;
    ofn.nMaxFile     = MAX_PATH;
    ofn.lpstrDefExt  = L"txt";
    ofn.Flags        = OFN_OVERWRITEPROMPT;
    if (GetSaveFileName(&ofn)) return std::wstring(buf);
    return L"";
}

// ── Set status bar text ───────────────────────
void setStatus(const std::wstring& msg) {
    SetWindowText(hStatus, msg.c_str());
}

// ── Window Procedure ──────────────────────────
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {

    case WM_CREATE: {
        // ── Menu bar ──
        HMENU hMenu     = CreateMenu();
        HMENU hFile     = CreatePopupMenu();
        HMENU hHelp     = CreatePopupMenu();

        AppendMenu(hFile, MF_STRING, IDM_FILE_OPEN, L"&Open Student File...");
        AppendMenu(hFile, MF_STRING, IDM_FILE_SAVE, L"&Save Results...");
        AppendMenu(hFile, MF_SEPARATOR, 0, NULL);
        AppendMenu(hFile, MF_STRING, IDM_FILE_EXIT, L"E&xit");

        AppendMenu(hHelp, MF_STRING, IDM_ABOUT, L"&About");

        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFile, L"&File");
        AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelp, L"&Help");
        SetMenu(hwnd, hMenu);

        // ── Buttons ──
        CreateWindow(L"BUTTON", L"Calculate (Average)",
            WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
            10, 10, 180, 30, hwnd, (HMENU)IDC_BTN_CALC_AVG, NULL, NULL);

        CreateWindow(L"BUTTON", L"Calculate (Median)",
            WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
            200, 10, 180, 30, hwnd, (HMENU)IDC_BTN_CALC_MED, NULL, NULL);

        CreateWindow(L"BUTTON", L"Split Passed / Failed",
            WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
            390, 10, 180, 30, hwnd, (HMENU)IDC_BTN_SPLIT, NULL, NULL);

        // ── TextBox (multiline, scrollable) ──
        hTextBox = CreateWindow(L"EDIT", L"",
            WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|
            ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_READONLY,
            10, 50, 760, 460, hwnd, (HMENU)IDC_TEXTBOX, NULL, NULL);

        // Use monospace font for alignment
        HFONT hFont = CreateFont(16,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,
            DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,FIXED_PITCH|FF_MODERN, L"Courier New");
        SendMessage(hTextBox, WM_SETFONT, (WPARAM)hFont, TRUE);

        // ── Status bar ──
        hStatus = CreateWindow(L"STATIC", L"Ready. Use File > Open to load a student file.",
            WS_CHILD|WS_VISIBLE|SS_LEFT,
            10, 520, 760, 20, hwnd, (HMENU)IDC_LABEL_STATUS, NULL, NULL);

        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wp)) {

        case IDM_FILE_OPEN: {
            std::wstring path = openFileDialog(hwnd);
            if (!path.empty()) {
                gLoadedFile = path;
                auto students = readStudents(path);
                std::wostringstream info;
                info << L"Loaded " << students.size() << L" students from:\r\n" << path
                     << L"\r\n\r\nClick 'Calculate' to see results.";
                SetWindowText(hTextBox, info.str().c_str());
                setStatus(L"File loaded: " + path);
            }
            break;
        }

        case IDM_FILE_SAVE: {
            if (gLoadedFile.empty()) {
                MessageBox(hwnd, L"Please open a file first.", L"No Data", MB_ICONWARNING);
                break;
            }
            std::wstring savePath = saveFileDialog(hwnd);
            if (!savePath.empty()) {
                wchar_t buf[1<<20] = {0};
                GetWindowText(hTextBox, buf, sizeof(buf)/sizeof(wchar_t));
                std::wstring content(buf);
                std::string path(savePath.begin(), savePath.end());
                std::ofstream f(path);
                for (wchar_t c : content)
                    f << (char)c;
                setStatus(L"Results saved to: " + savePath);
                MessageBox(hwnd, L"Results saved successfully!", L"Saved", MB_ICONINFORMATION);
            }
            break;
        }

        case IDM_FILE_EXIT:
            DestroyWindow(hwnd);
            break;

        case IDM_ABOUT:
            MessageBox(hwnd,
                L"Student Grade Calculator v1.0\r\n\r\n"
                L"Calculates student final grades using:\r\n"
                L"  Final = 0.4 * (Avg|Med of HW) + 0.6 * Exam\r\n\r\n"
                L"Organization: VVK\r\n"
                L"Course: Systemic Programming (PITs-25-NL-EN)\r\n\r\n"
                L"Usage:\r\n"
                L"  1. File > Open to load a student .txt file\r\n"
                L"  2. Click Calculate (Average or Median)\r\n"
                L"  3. Click Split to divide passed/failed\r\n"
                L"  4. File > Save to save results",
                L"About", MB_ICONINFORMATION);
            break;

        case IDC_BTN_CALC_AVG: {
            if (gLoadedFile.empty()) {
                MessageBox(hwnd, L"Please open a file first.", L"No Data", MB_ICONWARNING);
                break;
            }
            auto students = readStudents(gLoadedFile);
            std::wstring result = formatResults(students, false);
            SetWindowText(hTextBox, result.c_str());
            setStatus(L"Results calculated using Average.");
            break;
        }

        case IDC_BTN_CALC_MED: {
            if (gLoadedFile.empty()) {
                MessageBox(hwnd, L"Please open a file first.", L"No Data", MB_ICONWARNING);
                break;
            }
            auto students = readStudents(gLoadedFile);
            std::wstring result = formatResults(students, true);
            SetWindowText(hTextBox, result.c_str());
            setStatus(L"Results calculated using Median.");
            break;
        }

        case IDC_BTN_SPLIT: {
            if (gLoadedFile.empty()) {
                MessageBox(hwnd, L"Please open a file first.", L"No Data", MB_ICONWARNING);
                break;
            }
            auto students = readStudents(gLoadedFile);
            for (auto& p : students) p.calcFinal(false);
            std::sort(students.begin(), students.end(), sortByName);

            // Ask where to save passed
            MessageBox(hwnd, L"Choose file for PASSED students.", L"Save Passed", MB_ICONINFORMATION);
            std::wstring passedPath = saveFileDialog(hwnd);
            if (passedPath.empty()) break;

            MessageBox(hwnd, L"Choose file for FAILED students.", L"Save Failed", MB_ICONINFORMATION);
            std::wstring failedPath = saveFileDialog(hwnd);
            if (failedPath.empty()) break;

            std::string pp(passedPath.begin(), passedPath.end());
            std::string fp(failedPath.begin(),  failedPath.end());
            std::ofstream pf(pp), ff(fp);

            int passedCount = 0, failedCount = 0;
            for (const auto& p : students) {
                std::ostringstream row;
                row << std::left << std::setw(14) << p.firstName
                    << std::setw(14) << p.surname
                    << std::fixed << std::setprecision(2)
                    << p.finalGrade << "\n";
                if (p.finalGrade >= 5.0) { pf << row.str(); passedCount++; }
                else                     { ff << row.str(); failedCount++; }
            }

            std::wostringstream info;
            info << L"Split complete!\r\n\r\n"
                 << L"Passed: " << passedCount << L" students -> " << passedPath << L"\r\n"
                 << L"Failed: " << failedCount << L" students -> " << failedPath;
            MessageBox(hwnd, info.str().c_str(), L"Split Done", MB_ICONINFORMATION);
            setStatus(L"Split complete. Passed: " + std::to_wstring(passedCount) +
                      L"  Failed: " + std::to_wstring(failedCount));
            break;
        }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// ── WinMain ───────────────────────────────────
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = L"GradeCalcWnd";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        L"GradeCalcWnd",
        L"Student Grade Calculator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 580,
        NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
