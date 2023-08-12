#include <stdio.h>
#include <windows.h>
#include <tlHelp32.h>
#define MAX_PATH_LEN 260

char* k = "[+]";
char* i = "[*]";
char* e = "[-]";

int horizontal;
int vertical;
HBITMAP hbm;

int main (int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s Need more than that mate.\n", e);
        return 1;
    }

    DWORD PID = atoi(argv[1]);

    //getSystemDirectory(PID);
    //systemSnap(PID);
    //getNameOfComp();
    //getNativeSysInfo();
    //fileMappingA();
    //getClipboard();
    /*
    HDC dc = deviceContext();
    deviceCaps(dc);
    bitBLT(dc);
    bmpCreation(dc);*/
    keyName();
    return 0;
}

// We're gonna return the system directory of the Windows Computer
int getSystemDirectory(PID) {
    HANDLE hprocess = NULL;
    void* store = NULL;
    UINT result = NULL;

    char msg[MAX_PATH_LEN];

    /* GetSystemDirectory writes the location of the system directory
    inside it's first arguement, 'result' only stores the length of the 
    message written into 'msg'*/
    result = GetSystemDirectoryA(msg, MAX_PATH_LEN);

    if (result == NULL) {
        printf("%s Failed to get System Directory.\nError: %ld", e, GetLastError());
        CloseHandle(hprocess);
        return 1;
    }

    printf("%s Result: %s", k, msg);



    return 0;

}

// Take a snapshot of all processes running on a system
int systemSnap(PID) {

    // Structure that saves all the data of a process from a snapshot
    PROCESSENTRY32 pe32;
    // Empties the Process Entry
    ZeroMemory(&pe32, sizeof(pe32));
    // Sets the size of the entry to the size of the entry structure (since we erased it in the line above)
    pe32.dwSize = sizeof(pe32);
    // Result will be used later
    HANDLE result = NULL;

    // Use 'result' to store the data from the snapshot
    // TH32CS_SNAPPROCESS tells the system to screenshot all processes running on the system
    // The second parameter is 0 because it is not needed for the first parameter to run
    result = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // Quick check to see if the snapshot was successful
    if (result == INVALID_HANDLE_VALUE) {
        printf("%s Failed to snap.\nError: %ld", e, GetLastError());
        return 1;
    }

    printf("%s Successfully snapped!\n", k);

    // PROCESS32NEXT fills the 'pe32' with info about the next process in the snapshot
    // '&pe32' is a pointer to a structure, where the info will be stored
    while (Process32Next(result, &pe32)) {
        printf("%s Successfully obtained First Process!\nProcess ID: %s", k, pe32.szExeFile);
    }

    // End of program
    CloseHandle(result);
    return 0;
}

// Get name of computer
int getNameOfComp() {
    // Get the maximum length a computer name can have ('+ 1' includes the terminating NULL character)
    char name[MAX_COMPUTERNAME_LENGTH + 1];
    // Get the size of the name variable
    // We use DWORD because the 'GetComputer...' function expects it and its safer than int
    DWORD size = sizeof(name);

    // GetComputerNameA retrieves the name of the computer and stores it in the buffer 'name'
    HANDLE result = GetComputerNameA(
        name,
        &size
    );

    // Checks to see if the function failed
    if (result == 0) {
        printf("%s Failed to retrieve name.\nError: %ld", e, GetLastError());
        CloseHandle(result);
        return 1;
    }

    // Prints out the name of the computer
    printf("%s Successfully received name!\nName: %s", k, name);

    return 0;
}

// Get info about the system
int getNativeSysInfo() {
    // Creates a variable with the structure we need to retreive the info
    SYSTEM_INFO x;

    // Retreives the info about the system
    // MUST use a pointer so that x actually receives the info
    GetNativeSystemInfo(&x);

    // Prints out info (there's way more than just the amount of processors)
    printf("%s Received Info: %lu", k, x.dwNumberOfProcessors);

    return 0;
}

// Create a map of a file
int fileMappingA(){
    // Define a HANDLE when we open the file
    HANDLE hFile = NULL;
    // Tell the computer where the file is
    char* filePath = "D:\\Mal\\MalAPI Experiments\\FileMappingA.txt";

    // Open the file
    hFile = CreateFileA(
        filePath,
        (GENERIC_READ | GENERIC_WRITE),
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    // Get the size of the file for when we map it
    DWORD fileSize = GetFileSize(hFile, NULL);

    // Check if it errored/didn't work
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("%s Failed to open file.\nError: %ld",e, GetLastError());
        CloseHandle(hFile);
        return 1;
    }

    printf("%s Successfully opened file!\n", k);

    // Create the mapping
    HANDLE x = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READWRITE,
        0,
        fileSize,
        NULL
    );

    // Check if it worked
    if (x == NULL) {
        printf("%s Failed to create mapping\nError: %ld",e, GetLastError());
        CloseHandle(hFile);
        CloseHandle(x);
        return 1;
    }

    printf("%s Successfully created Mapping!\nResult: %p",k, x);

    // Close handles on success
    CloseHandle(hFile);
    CloseHandle(x);

    return 0;
}

// Get text from Clipboard
int getClipboard() {
    // Handle for the later clipboard data
    HANDLE clipboard = NULL;

    // Opens clipboard whilst checking if it can't open
    if(!OpenClipboard(NULL)) {
        printf("%s Failed to open clipboard.\nError: %ld", e, GetLastError());
        return 1;
    }

    // Retreives text from the clipboard
    clipboard = GetClipboardData(CF_TEXT);

    // Checked if it worked
    if (clipboard == NULL) {
        printf("%s Failed to receive clipboard text\nError: %ld", e, GetLastError());
        CloseHandle(clipboard);
        return 1;
    }

    printf("%s Successfully received Clipboard text!\nResult: %s", k, clipboard);

    // Closes clipboard and Handle
    CloseClipboard();
    CloseHandle(clipboard);

    return 0;
}

// Grab info about the drawing attributes of a device (display/printer)
int deviceContext() {
    HDC context;

    // Retreives specific info about a device
    context = GetDC(
        NULL
    );

    if (!context) {
        printf("%s Failed to retreive Device Context\nError: %ld", e, GetLastError());
        return 1;
    }

    printf("%s Successfully retreived Device Context!\nResult: %p",k, context);

    return context;
}

/*
 -------------------------------IMPORTANT-----------------------------------
                Shelving this for when I have learned more
*/
// Retreive device specific information about the specified device
int deviceCaps(HDC hdc) {
    // Using the context from the previous context, we can grab the resolution of the device
    int horz = GetDeviceCaps(
        hdc,
        HORZRES
    );
    int vert = GetDeviceCaps(
        hdc,
        VERTRES
    );
    // NOTE: Since my laptop has scaling, the results will be a bit wacky
    printf("\n%s Results: %i x %i\n", k, horz, vert);

    horizontal = horz;
    vertical = vert;

    return 0;
    
}

//
int bitBLT (HDC hdc) {
    // Handle for device context that will be returned when BitBLT is run
    HDC retHDC;
    // Device context source (from previous functions)
    HDC hdcSrc = hdc;

    // Create a bitmap
    HBITMAP hBitmap = CreateCompatibleBitmap(
        hdcSrc,
        horizontal,
        vertical
    );

    // 'SelectObject(hdcSrc, hBitmap)' replaces bitmap with a bitmap that has been imbued with the device context
    // '(HBITMAP)' makes sure that that the data returned to 'hBitmapOld' is a HBITMAP structure
    HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcSrc, hBitmap);

    // Transfers color data of a rectangle from a device context into another device context
    BitBlt(
        retHDC,
        0,
        0,
        horizontal,
        vertical,
        hdcSrc,
        0,
        0,
        SRCCOPY
    );

    // Checks if the above function worked
    if (retHDC == 0) {
        printf("%s Failed!\nError: %ld", e, GetLastError());
        return 1;
    }

    printf("%s Successfully transfered bit-lock data!\nResult: %p\n", k, retHDC);

    // Makes a new bitmap after the colour data transfer
    hBitmap = (HBITMAP)SelectObject(retHDC, hBitmapOld);
    hbm = hBitmap;

    // Deletes device context
    DeleteDC(retHDC);
    DeleteDC(hdcSrc);

    return 0;
}

int bmpCreation(HDC hdc) {
    BITMAPFILEHEADER bfHeader;
    BITMAPINFOHEADER biHeader;
    BITMAPINFO bInfo;
    DWORD dwSizeOfDIB = 0;
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER info;
    char *bitmapData;

    char* filePath = "D:\\Mal\\MalAPI Experiments\\Screenshot.bmp"; 
    HANDLE hFile = NULL;
    DWORD dwBytesWritten = 0;

    hFile = CreateFileA(
        filePath,
        (GENERIC_READ | GENERIC_WRITE),
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("%s Failed to create file.\nError: %ld\n", e, GetLastError());
        return 1;
    }

    bfHeader.bfType = 0x4D42; // "BM"
    bfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfHeader.bfReserved1 = 0;
    bfHeader.bfReserved2 = 0;
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    if (!WriteFile(
        hFile,
        &bfHeader,
        sizeof(bfHeader),
        &dwBytesWritten,
        NULL
    )) {
        printf("%s Failed to write Header info to the file\nError: %ld\n", e, GetLastError());
        return 1;
    }

    printf("%s Successfully wrote Bitmap Header Info into the file!\n", k); 

    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    
    GetDIBits(
        hdc,
        hbm,
        0,
        0,
        NULL,
        (BITMAPINFO*)&biHeader,
        DIB_RGB_COLORS
    );

    if(!WriteFile(
        hFile,
        &biHeader,
        sizeof(biHeader),
        &dwBytesWritten,
        NULL
    )){
        printf("%s Failed to write Bitmap Info Header to the file\nError: %ld\n",e, GetLastError());
        return 1;
    }

    DWORD dwBitmapSize = biHeader.biSizeImage;

    BYTE* pBitmapData = (BYTE*)malloc(dwBitmapSize);
    if (pBitmapData == NULL) {
        printf("Failed to allocate memory for bitmap data.\n");
        CloseHandle(hFile);
        return;
    }

    GetDIBits(
        NULL,          // Device context (NULL for memory bitmap)
        hbm,       // Bitmap handle
        0,             // Start scan line
        biHeader.biHeight, // Number of scan lines
        pBitmapData,   // Buffer to hold the bitmap data
        (BITMAPINFO*)&biHeader,
        DIB_RGB_COLORS
    );

    // Write the bitmap data to the file
    WriteFile(hFile, pBitmapData, dwBitmapSize, &dwBytesWritten, NULL);

    // Free the allocated memory
    free(pBitmapData);

    // Close the file handle
    CloseHandle(hFile);

    printf("%s Successfully wrote Bitmap Info into the file!\n", k);



    return 0;

}

//--------------------------------------------------------------------------

int captainHook () {
    HINSTANCE hmod = 
    HHOOK SetWindowsHookExA(
        [in] int       WH_KEYBOARD,
        [in] HOOKPROC  lpfn,
        [in] HINSTANCE hmod,
        [in] DWORD     0
    );
}

/*int keyName() {
    LPSTR lpString;
    if(GetKeyNameTextA(
        WM_KEYDOWN,
        lpString,
        sizeof(lpString)
    ) == 0) {
        printf("%s Failed to get Keyname.\nError: %ld", e, GetLastError());
    }
    printf("%s", lpString);
}
*/










