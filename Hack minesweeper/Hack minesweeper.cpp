// Hack minesweeper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
using namespace std;
int main()
{
	//ngoài lề
	//0x010056A4: số bom trong trò chơi, đặt nó bằng 1 xong bắt đầu game mới thì game sẽ bắt đầu với 1 bom, chơi thế chả vui lắm nên thui k dùng
	//0x0100579C: thời gian của trò chơi
	DWORD widthAddr = 0x010056AC; // chiều rộng map
	DWORD heightAddr = 0x010056A8; //chiều cao map
	DWORD firstAddr = 0x01005361; // ô mìn đầu tiên map[0][0]
	DWORD opendAddr = 0x010057A4; // số ô đã mở
	HWND hwnd = FindWindowA(0, "Minesweeper");
	int mywidth, myheight, myopend;
	byte mycurr;
	if (!hwnd)
	{
		cout << "not found";
	}
	else
	{
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid); 
		HANDLE phandle = OpenProcess(PROCESS_VM_READ, 0, pid); //PROCESS_ALL_ACCESS để có thể thao tác đọc ghi và tất cả cái khác, đây mình chỉ đọc thôi
		while (true) // update giá trị liên tục nên đặt trong vòng lặp
		{
			system("CLS");
			ReadProcessMemory(phandle, (LPVOID)widthAddr, &mywidth, sizeof(mywidth), 0);
			ReadProcessMemory(phandle, (LPVOID)heightAddr, &myheight, sizeof(myheight), 0);
			ReadProcessMemory(phandle, (LPVOID)opendAddr, &myopend, sizeof(myopend), 0);
			//cout << mywidth << "-" << myheight << endl;
			if (myopend == 0) { // sau khi mở 1 ô rồi thì game mới tạo bom, nên chưa mở thì vẽ tạm theo kích thước của map thui
				for (int i = 0; i < myheight; i++)
				{
					for (int j = 0; j < mywidth; j++) {
						cout << "-";
					}
					cout << endl;
				}
			}
			else {
				DWORD current = firstAddr;
				for (int i = 0; i < myheight; i++)
				{
					for (int j = 0; j < mywidth; j++) {
						ReadProcessMemory(phandle, (LPVOID)current, &mycurr, sizeof(mycurr), 0);
						//cout << mycurr;
						if (mycurr == 0x8F || mycurr == 0x8E || mycurr == 0x8A) // 0x8F là bom, 0x8F là đặt cờ, 0x8A là bom mở ra sau khi kết thúc game
						{
							cout << "x";
						}
						else if (mycurr == 0xCC) // 0xCC là quả bom lúc ấn vào nó nổ
						{
							cout << "0";
						}
						else if (mycurr == 0x0F) { //0x0F là ô mặc định chưa mở hoặc chưa đặt cờ
							cout << "-";
						}
						else {
							cout << " "; 
						}
						current++; //  đi đến ô tiếp theo
					}
					current = firstAddr + 0x20 * (i + 1); //0x20 là giá trị mỗi hàng cách nhau, trừ đi là ra nó bằng 0x20
					cout << endl;
				}
			}
			Sleep(1000); // mỗi giây update 1 lần
		}

	}
}
