#include "icb_gui.h"

int multiLineEdit, accountEdit, tcEdit, nameEdit, balanceEdit, photoFrame;

struct BankData
{
    ICDEVICE indexFile, dataFile;
    ICBYTES balanceData, notes, customerImage;
    ICBYTES indexMap, accountNum, tcNum, fullName;
};

void ICGUI_Create()
{
    ICG_MWTitle("Ciftlik Bank Database Management System");
    ICG_MWSize(625, 700);
}

unsigned FindIndex(ICBYTES& index, unsigned long long* key)
{
    for (unsigned i = 1; i <= index.Y(); i++)
        if (index.O(1, i) == key[0] && index.O(2, i) == key[1]) return i;
    return 0xFFFFFFFF;
}

void SaveData(void* db)
{
    BankData& data = *((BankData*)db);
    GetText(accountEdit, data.accountNum);
    GetText(tcEdit, data.tcNum);
    GetText(nameEdit, data.fullName);
    GetText(balanceEdit, data.balanceData);
    GetText(multiLineEdit, data.notes);

    unsigned long long* accountKey = KeyMapTR(data.accountNum, 2);

    unsigned exists = FindIndex(data.indexMap, accountKey);
    if (exists != 0xFFFFFFFF) {
        ICG_printf(multiLineEdit, "\nError: Account exists!\n");
        return;
    }

    long long newIdx = data.indexMap.Y();
    if (data.indexMap.O(1, newIdx) != 0) {
        newIdx++;
        ResizeMatrix(data.indexMap, 3, newIdx);
    }

    data.indexMap.O(1, newIdx) = accountKey[0];
    data.indexMap.O(2, newIdx) = accountKey[1];
    data.indexMap.O(3, newIdx) = GetFileLength(data.dataFile);

    long long addr = WriteICBYTES(data.dataFile, data.accountNum, data.indexMap.O(3, newIdx));
    addr = WriteICBYTES(data.dataFile, data.tcNum, addr);
    addr = WriteICBYTES(data.dataFile, data.fullName, addr);
    addr = WriteICBYTES(data.dataFile, data.balanceData, addr);
    addr = WriteICBYTES(data.dataFile, data.customerImage, addr);
    WriteICBYTES(data.dataFile, data.notes, addr);

    ICG_printf(multiLineEdit, "\nData saved!\n");
}

void AddPhoto(void* db)
{
    BankData& data = *((BankData*)db);
    ICBYTES path;
    ICBYTES img;

    const char* file = OpenFileMenu(path, "JPEG\0*.JPG\0BMP\0*.BMP\0");
    if (file && ReadImage(file, img)) {
        data.customerImage = img;
        DisplayImage(photoFrame, data.customerImage);
    }
}

void FindAccount(void* db)
{
    BankData& data = *((BankData*)db);
    GetText(accountEdit, data.accountNum);
    unsigned long long* accountKey = KeyMapTR(data.accountNum, 2);

    unsigned pos = FindIndex(data.indexMap, accountKey);
    if (pos != 0xFFFFFFFF) {
        long long addr = ReadICBYTES(data.dataFile, data.accountNum, data.indexMap.O(3, pos));
        addr = ReadICBYTES(data.dataFile, data.tcNum, addr);
        addr = ReadICBYTES(data.dataFile, data.fullName, addr);
        addr = ReadICBYTES(data.dataFile, data.balanceData, addr);
        addr = ReadICBYTES(data.dataFile, data.customerImage, addr);
        ReadICBYTES(data.dataFile, data.notes, addr);

        ICG_SetWindowText(accountEdit, &data.accountNum.C(1));
        ICG_SetWindowText(tcEdit, &data.tcNum.C(1));
        ICG_SetWindowText(nameEdit, &data.fullName.C(1));
        ICG_SetWindowText(balanceEdit, &data.balanceData.C(1));
        ICG_SetWindowText(multiLineEdit, &data.notes.C(1));

        DisplayImage(photoFrame, data.customerImage);
    }
    else ICG_printf(multiLineEdit, "\nAccount missing!\n");
}

void RemoveAccount(void* db)
{
    BankData& data = *((BankData*)db);
    GetText(accountEdit, data.accountNum);
    unsigned long long* accountKey = KeyMapTR(data.accountNum, 2);

    unsigned delPos = FindIndex(data.indexMap, accountKey);
    if (delPos == 0xFFFFFFFF) {
        ICG_printf(multiLineEdit, "\nDelete failed!\n");
        return;
    }

    for (unsigned i = delPos; i < data.indexMap.Y(); i++) {
        data.indexMap.O(1, i) = data.indexMap.O(1, i + 1);
        data.indexMap.O(2, i) = data.indexMap.O(2, i + 1);
        data.indexMap.O(3, i) = data.indexMap.O(3, i + 1);
    }
    ResizeMatrix(data.indexMap, 3, data.indexMap.Y() - 1);

    ICG_SetWindowText(accountEdit, "");
    ICG_SetWindowText(tcEdit, "");
    ICG_SetWindowText(nameEdit, "");
    ICG_SetWindowText(balanceEdit, "");
    ICG_SetWindowText(multiLineEdit, "");
    ICG_printf(multiLineEdit, "\nAccount deleted!\n");
}

void SetupDB(BankData& db)
{
    CreateMatrix(db.indexMap, 3, 1, ICB_ULONGLONG);
    CreateFileDevice(db.indexFile, "bank_index.dat");
    CreateFileDevice(db.dataFile, "bank_data.dat");
    ReadICBYTES(db.indexFile, db.indexMap, 0);
}

void Shutdown(void* db)
{
    BankData& data = *((BankData*)db);
    if (data.indexMap.O(1, 1) != 0)
        WriteICBYTES(data.indexFile, data.indexMap, 0);
    CloseDevice(data.indexFile);
    CloseDevice(data.dataFile);
}

void ResetFields(void* db)
{
    BankData& data = *((BankData*)db);
    ICG_SetWindowText(accountEdit, "");
    ICG_SetWindowText(tcEdit, "");
    ICG_SetWindowText(nameEdit, "");
    ICG_SetWindowText(balanceEdit, "");
    ICG_SetWindowText(multiLineEdit, "");

    ICBYTES blank;
    data.customerImage = blank;
    DisplayImage(photoFrame, blank);
}

void ICGUI_main()
{
    static BankData db;

    int winWidth = 900;
    int winHeight = 600;

    ICG_Static((winWidth - 500) / 2, 20, 400, 50, "Ciftlik Bank Database Management System");

    int baseX = (winWidth - 800) / 2;
    int baseY = 100;

    ICG_Button(baseX, baseY, 120, 30, "SAVE", SaveData, &db);
    ICG_Button(baseX + 130, baseY, 120, 30, "SEARCH", FindAccount, &db);
    ICG_Button(baseX + 260, baseY, 120, 30, "DELETE", RemoveAccount, &db);
    ICG_Button(baseX + 390, baseY, 120, 30, "RESET", ResetFields, &db);
    ICG_Button(baseX, baseY + 480, 120, 30, "CHOOSE PHOTO", AddPhoto, &db);

    accountEdit = ICG_SLEditSunken(baseX, baseY + 70, 250, 25, "");
    tcEdit = ICG_SLEditSunken(baseX, baseY + 130, 250, 25, "");
    nameEdit = ICG_SLEditSunken(baseX, baseY + 190, 250, 25, "");
    balanceEdit = ICG_SLEditSunken(baseX, baseY + 250, 250, 25, "");
    multiLineEdit = ICG_MLEditSunken(baseX, baseY + 310, 500, 150, "", SCROLLBAR_V);
    int imgX = winWidth - 250;
    photoFrame = ICG_FramePanel(imgX-300, baseY + 70, 200, 200);

    ICG_Static(baseX, baseY + 50, 250, 20, "Account ID:");
    ICG_Static(baseX, baseY + 110, 250, 20, "TC Number:");
    ICG_Static(baseX, baseY + 170, 250, 20, "Client Name:");
    ICG_Static(baseX, baseY + 230, 250, 20, "Balance:");
    ICG_Static(baseX, baseY + 290, 250, 20, "Details:");
    ICG_Static(imgX-300, baseY + 50, 200, 20, "Client Photo:");
    
    ICG_SetOnExit(Shutdown, &db);
    SetupDB(db);
}
