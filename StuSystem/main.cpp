#include "Action.h"

int main() {
	//初始化控制台窗口
	Console cons;
	cons.init();
	
	//初始化主界面
	MUI mui(&cons);

	//获取用户输入
	UserInput input(&cons);
	
	system("cls");
	//加载数据
	loadingData(cons);
	

	while (true) {
		mui.showMainInit();
		control(input, mui, cons);
		system("pause");
	}

	return 0;
}