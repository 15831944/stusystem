#include "Action.h"
bool isExit = false;
MTree<Mdata> tree;
MTree<Mdata> treeName;

//文件和树数据添加
void add(MString& data, int id) {
	Mdata tmp;
	{ //加作用域提前释放打开的文件
		//打开文件写入文件
		MFile mfile("nameEN.bin", ios::out | ios::binary | ios::app);
		tmp = mfile.write(data);
	}
	tmp.mId = id;
	//更新树节点
	tree.insert(tmp);//更新id树节点

	MString tmpId;
	MString tmpName;
	MString str = read(tmp);//读取文件数据
	idAndName(str, tmpId, tmpName);//获取文件数据id和name
	tmp.mId = mHash(tmpName.getStr(), tmpName.length());
	treeName.insert(tmp);
}

//数据删除
bool del(int id) {
	Mdata data;
	data.mId = id;
	try {
		data = tree.find(data)->data; //查找节点
	}
	catch (MException& e) {
		cout << e.what() << endl;
		return false;
	}
	{//作用域提前关文件
		//打开文件删除数据
		MFile mfile("nameEN.bin", ios::out | ios::binary | ios::in);
		mfile.write((char)0xFB, data.mOffset);
	}
	//更新树节点
	tree.remove(data);

	MString tmpId;
	MString tmpName;
	MString str = read(data);//读取文件数据
	idAndName(str, tmpId, tmpName);//获取文件数据id和name
	data.mId = mHash(tmpName.getStr(), tmpName.length());
	MTree<Mdata>::TreeNode* node = treeName.find(data);
	if (node->next == nullptr) {
		treeName.remove(data);
		return true;
	}
	
	str = read(node->data);//读取文件数据
	idAndName(str, tmpId, tmpName);//获取文件数据id和name
	int curId = strtol(tmpId.getStr(), nullptr, 10);//转换int
	if (curId == id) {
		if (node->parent != nullptr) {
			if (node->parent->lChild == node) {
				node->parent->lChild = node->next;
			}
			else {
				node->parent->rChild = node->next;
			}
		}
		node->next->parent = node->parent;
		if (node->lChild != nullptr) {
			node->lChild->parent = node->next;
		}
		if (node->rChild != nullptr) {
			node->rChild->parent = node->next;
		}
		node->next->lChild = node->lChild;
		node->next->rChild = node->rChild;
		node->next->height = node->height;
		delete node;
		return true;
	}

	//node = node->next;
	MTree<Mdata>::TreeNode* next = node->next;
	while (next != nullptr) {
		str = read(next->data);//读取文件数据
		idAndName(str, tmpId, tmpName);//获取文件数据id和name
		curId = strtol(tmpId.getStr(), nullptr, 10);//转换int
		if (curId == id) {
			node->next = next->next;
			delete next;
			return true;
		}
		node = node->next;
		next = next->next;
	}
	return true;
}

void clearUp() {
	{
		MFile mfile("nameEN.bin", ios::binary | ios::in);
		MFile mNew("nameEN.tmp", ios::binary | ios::out);
		istream& mf = mfile.getFileP();
		ostream& mn = mNew.getFileP();

		char sign = '\0'; //保存标志位
		short len = 0;
		char szD[62] = { 0 };
		mf.read((char*)& sign, sizeof(char));
		mf.read((char*)& len, sizeof(short));
		while (!mf.eof()) {
			if (sign == (char)0xFB) {
				mf.seekg(len, ios::cur);
			}
			else if (sign == (char)0xFA) {
				mn.write((char*)& sign, sizeof(char));
				mn.write((char*)& len, sizeof(short));
				mf.read(szD, len);
				mn.write(szD, len);
			}
			mf.read((char*)& sign, sizeof(char));
			mf.read((char*)& len, sizeof(short));
		}
	}
	DeleteFile("nameEN.bin");
	rename("nameEN.tmp", "nameEN.bin");
	tree.release();
	treeName.release();
	isExit = true;
}

//分割字符串中的学号和姓名
void idAndName(MString& des, MString& id, MString& name) {
	char szId[16] = { 0 };
	char szName[26] = { 0 };
	sscanf(des.getStr(), "%s \"%[^\"]\"", szId, szName);
	id = szId;
	name = szName;
}

//读取字符串
MString read(Mdata& data){
	MFile mfile("nameEN.bin", ios::in | ios::binary );
	return mfile.read(data);
}

//按姓名hash查找节点
MTree<Mdata>::TreeNode* findName(MString& name) {
	int hash = mHash(name.getStr(), name.length());
	Mdata tmp;
	tmp.mId = hash;
	MTree<Mdata>::TreeNode* node = nullptr;
	try {
		node = treeName.find(tmp); //查找ID
	}
	catch (MException& e) {
		cout << e.what() << endl;
		return nullptr;
	}
	return node;
}

void loadingData(Console& cons){
	thread t1(loading);
	thread t3(loadingName);
	auto loadingUiT = std::bind(loadingUi, cons); //函数绑定
	thread t2(loadingUiT, cons);
	t1.detach();
	t3.detach();
	t2.join();
}



//获取用户输入的姓名出生日期和性别
MString inputAllData(UserInput& input) {
	MString name;
	while (true) {
		cout << "姓名(姓名长度不能超过25字节，否则将截取前25字节存入): ";
		name = input.inputStr(26);
		if (name.length() <= 0 || name[0] == ' ') {
			cout << "error: 姓名不能为空或姓名不能以空格开头! " << endl;
		}
		else {
			break;
		}
	}

	MString date; //储存用户输入的数据
	struct DATE { //日期结构体
		int nYear; //表示年
		int nMonth;//表示月
		int nDay;  //表示日
		int nMonthOfDays[12];
	} dataStruct = { -1,-1,-1,31,28,31,30,31,30,31,31,30,31,30,31 };
	//判断是否为闰年并把二月改成29天
	while (true) {
		cout << "出生日期(YYYY-MM-DD) : ";
		date = input.inputStr(16);
		sscanf(date.getStr(), "%d-%d-%d", &dataStruct.nYear, &dataStruct.nMonth, &dataStruct.nDay);//格式输入字符串
		//判断是否为闰年并把二月改成29天
		if (dataStruct.nYear % 4 == 0 && dataStruct.nYear % 100 != 0) {
			dataStruct.nMonthOfDays[1] = 29;
		}

		if (dataStruct.nYear == -1 || dataStruct.nMonth == -1 || dataStruct.nDay == -1) {//判断输入格式
			cout << "error: 出生日期格式（YYYY-MM-DD）!" << endl;
			continue;
		}
		else {
			if (dataStruct.nYear > 2019) {
				cout << "error: 年份错误!" << endl;
				continue;
			}
			if (dataStruct.nMonth < 1 || dataStruct.nMonth >12) {
				cout << "error: 一年只有12个月（1~12）!" << endl;
				continue;
			}

			//判断本月是否存在某天
			if (dataStruct.nDay > dataStruct.nMonthOfDays[dataStruct.nMonth - 1] || dataStruct.nDay <= 0) {
				if (dataStruct.nMonth == 2) {
					if (dataStruct.nMonthOfDays[1] = 29) {
						cout << "error: " <<"闰年2月份只有29天! " << endl;
					}
					else {
						cout << "error: " << "平年2月份只有28天! " << endl;
					}
				}
				else {
					cout << "error: " << dataStruct.nMonth << "月中没有 " << dataStruct.nDay << "号! " << endl;
				}
				continue;
			}
			break;
		}
	}
	
	//格式日期格式，不足2位填0
	date = dataStruct.nYear;
	date.append("-");
	if (dataStruct.nMonth < 10) {
		date.append("0").append(dataStruct.nMonth).append("-");
	}
	else {
		date.append(dataStruct.nMonth).append("-");
	}
	if (dataStruct.nDay < 10) {
		date.append("0").append(dataStruct.nDay);
	}
	else {
		date.append(dataStruct.nDay);
	}


	char sex = '\0';
	//判断性别输入
	while (true) {
		cout << "性别( 男 M, 女 F ): ";
		sex = input.inputCh();
		if (sex == 'M' || sex == 'm' || sex == 'f' || sex == 'F') {
			if (sex == 'm') {
				sex = 'M';
			}
			else if (sex == 'f') {
				sex = 'F';
			}
			break;
		}
		cout << endl;
		cout << "error: 性别是（男 M/m, 女 F/f）!" << endl;
	}
	cout << endl;
	MString data;
	data.append(" \"").append(name).append("\" ").append(date).append(" ").append(sex);
	return std::move(data);
}

//计算hash算法
unsigned int mHash(char* str, unsigned int len)
{
	unsigned int hash = 0;
	unsigned int x = 0;
	unsigned int i = 0;
	for (i = 0; i < len; str++, i++)
	{
		hash = (hash << 4) + (*str);
		if ((x = hash & 0xF0000000L) != 0)
		{
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}
	return hash;
}

//加载数据
void loading() {
	MFile file("nameEN.bin", ios::in | ios::binary);
	fstream& ifs = file.getFileP();
	
	long long curOffset = 0; //记录当前文件游标位置
	short curSize = 0; //记录当前数据长度
	char sign = '\0'; //记录标志位
	char szId[16] = { 0 };//记录学号id
	char szName[26] = { 0 };
	MString s;
	while (true) {
		curOffset = ifs.tellg();//获取当前游标位置
		ifs.read((char*)& sign, sizeof(char));//读取标志位
		ifs.read((char*)& curSize, sizeof(short));//读取数据长度

		//跳过已经删除数据
		if (sign == (char)0xFB) {//判断标志位
			ifs.seekg(curSize, ios::cur);//跳过当前数据
			sign = '\0'; //初始化标志位
			continue;//结束当前循环
		}
		//判断文件结束
		if (ifs.eof()) {
			break;
		}

		//读取用户数据
		Mdata tmp;
		
		tmp.mOffset = curOffset;
		tmp.mSize = curSize;

		s = file.read(tmp);
		sscanf(s.getStr(), "%s \"%[^\"]\"", szId, szName);
		tmp.mId = strtol(szId, nullptr, 10);
		//写入树
		tree.insert(tmp);
	}
	isExit = true; //发送结束消息
}

//加载数据
void loadingName() {
	MFile file("nameEN.bin", ios::in | ios::binary);
	fstream& ifs = file.getFileP();

	long long curOffset = 0; //记录当前文件游标位置
	short curSize = 0; //记录当前数据长度
	char sign = '\0'; //记录标志位
	char szId[16] = { 0 };//记录学号id
	char szName[26] = { 0 };
	MString s;
	while (true) {
		curOffset = ifs.tellg();//获取当前游标位置
		ifs.read((char*)& sign, sizeof(char));//读取标志位
		ifs.read((char*)& curSize, sizeof(short));//读取数据长度

		//跳过已经删除数据
		if (sign == (char)0xFB) {//判断标志位
			ifs.seekg(curSize, ios::cur);//跳过当前数据
			sign = '\0'; //初始化标志位
			continue;//结束当前循环
		}
		//判断文件结束
		if (ifs.eof()) {
			break;
		}

		//读取用户数据
		Mdata tmp ;

		tmp.mOffset = curOffset;
		tmp.mSize = curSize;

		s = file.read(tmp);
		sscanf(s.getStr(), "%s \"%[^\"]\"", szId, szName);
		tmp.mId = mHash(szName,strlen(szName));
		//写入树
		treeName.insert(tmp);
	}
	//isExit = false; //发送结束消息
}

//加载数据等待ui
void loadingUi(Console& cons) {
	int x = 38;
	int y = 4;
	int width = 20;
	int height = 3;

	for (int i = 0;i < height; i++) {
		cons.fillConsole(x, y + i, 0, width);
	}
	int color = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY | BACKGROUND_RED;
	for (int i = 0;i < height; i++) {
		cons.fillConsole(x - 1, y - 1 + i, color, width);
	}

	const char* point = "loading";
	char tmp[20] = "loading";

	cons.setColor(0, 15);
	for (int i = 1;;i++) {
		cons.setCursorPos(x + 4, y);
		cout << tmp << endl;
		Sleep(500);
		strcat(tmp, ".");
		if (i % 5 == 0) {
			strncpy(tmp, point, 8);
			cons.setCursorPos(x + 4, y);
			for (int i = 0;i < 12;i++)
				putchar(' ');
		}
		if (isExit) { isExit = false; break; }
	}
	cons.setColor();
}

//控制器,控制菜单和功能的连接
void control(UserInput& input, MUI& mui, Console& cons) {
	MString data;
	char choose = input.inputCh();
	cout << endl;
	switch (choose)
	{
	//添加
	case '1': {
		//加载添加标题
		mui.showTitleUi("添加");
		//获取用户输入
		MString str =inputAllData(input);
		//获取最大id+1
		int id = 0;
		try {
			id = tree.Max().mId + 1;
		}
		catch (MException& e) {
			return;
		}
		//连接字符串
		data = input.intToStr(id, 10);//将id转换字符串
		data.append(str);
		add(data,id); //调用函数添加输入文件
		cout << endl << "添加成功" << endl;
		break;
	}
	//删除
	case '2': {//删除
		mui.showTitleUi("删除");//显示当前标题
		mui.chooseUi();//显示选择界面UI
		char choose = input.inputCh();//获取用户输入的菜单号
		cout << endl << endl << endl;//用于界面排版
		switch (choose) {
		case '1': {
			cout << "请输入删除的学号: ";
			//获取用户输入
			MString idStr = input.inputStr(10); 

			//字符转整型
			int id = input.strToInt(idStr.getStr());
			if (del(id)) {
				cout << endl << "删除成功! " << endl;
			}
			break;
		}
		case '2':{
			cout << "请输入删除的姓名: ";
			MString name = input.inputStr(26); //获取用户输入
			//查找将删除的节点
			MTree<Mdata>::TreeNode* node = findName(name);

			MString idStr; //暂存id字符串
			Mdata tmp;
			if (node == nullptr) {
				return;
			}
			else if (node->next != nullptr) {
				//读写查询数据
				MFile mfile("nameEN.bin", ios::binary | ios::in);
				//显示查询结果
				mui.showDataTitle();
				while (node != nullptr) {
					tmp = node->data;
					data = mfile.read(tmp);
					mui.showData(data);
					node = node->next;
				}
				cout << "请输入以上重名的学号: ";
				idStr = input.inputStr(10);
				cout << endl;
			}
			else {
				data = read(node->data);
				idAndName(data, idStr, name);
			}	
			
			//如果当前节点没有相同直接删除
			int id = input.strToInt(idStr.getStr());
			if (del(id)) {
				cout << "删除成功! " << endl;;
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	//修改
	case '3': {//修改
		mui.showTitleUi("修改");
		mui.chooseUi();
		char choose = input.inputCh();
		cout << endl << endl << endl;
		switch (choose) {
		case '1': {
			cout << "请输入修改的学生学号: ";
			MString idStr = input.inputStr(10); //获取用户输入id
			//字符转整型
			int id = input.strToInt(idStr.getStr());
			if (!del(id)) {
				cout << "请输入正确的学号! " << endl;
				return;
			}
			//获取用户输入
			MString str = inputAllData(input);
			data.append(idStr).append(str);
			add(data,id);
			cout << endl << "修改成功!" << endl;
			break;
		}
		case '2': {
			cout << "请输入学生姓名: ";
			MString name = input.inputStr(26); //获取用户输入

			MTree<Mdata>::TreeNode* node = findName(name);

			MString idStr; //暂存id字符串
			Mdata tmp;

			if (node == nullptr) {
				return;
			}
			else if (node->next != nullptr) {
				//读写查询数据
				MFile mfile("nameEN.bin", ios::binary | ios::in);
				//显示查询结果
				mui.showDataTitle();
				while (node != nullptr) {
					tmp = node->data;
					data = mfile.read(tmp);
					mui.showData(data);
					node = node->next;
				}
				cout << "请输入以上重名的学号: ";
				idStr = input.inputStr(10);
			}
			else {
				data = read(node->data);
				idAndName(data, idStr, name);
			}
			//字符转整型
			int id = input.strToInt(idStr.getStr());
			if (!del(id)) {
				cout << "请输入正确的学号! " << endl;
				return;
			}
			//获取用户输入
			MString str = inputAllData(input);
			MString dataStr;
			dataStr.append(idStr).append(str);
			add(dataStr,id);//后天就
			cout <<endl<< "修改成功！" << endl;

			break;
		}
		default:
			break;
		}
		break;
	}
	//查询
	case '4': { //查询
		mui.showTitleUi("查询");
		mui.chooseUi();
		char choose = input.inputCh();
		cout << endl << endl << endl;
		switch (choose) {
		case '1': {
			cout << "请输入学号: ";
			MString ids = input.inputStr(10); //获取用户输入
			//字符转整型
			int id = input.strToInt(ids.getStr());
			Mdata tmp;
			tmp.mId = id;
			try {
				tmp = tree.find(tmp)->data; //查找ID
			}
			catch (MException& e) {
				cout << e.what() << endl;
				return;
			}
			//读写查询数据
			MFile mfile("nameEN.bin", ios::binary | ios::in);
			data = mfile.read(tmp);
			//显示查询结果
			mui.showDataTitle();
			mui.showData(data);
			
			break;
		}
		case '2': {
			cout << "请输入学生姓名: ";
			MString name = input.inputStr(26); //获取用户输入
			
			MTree<Mdata>::TreeNode* node = findName(name);

			if (node == nullptr) {
				return;
			}

			Mdata tmp;
			//读写查询数据
			MFile mfile("nameEN.bin", ios::binary | ios::in);
			//显示查询结果
			mui.showDataTitle();
			while (node != nullptr) {
				tmp = node->data;
				data = mfile.read(tmp);
				mui.showData(data);
				node = node->next;
			}
			
			break;
		}
		default:
			break;
		}
		break;
	}
	//整理
	case '5': {
		mui.showTitleUi("整理");
		system("cls");
		//加载数据
		thread t1(clearUp);
		auto loadingUiT = std::bind(loadingUi, cons); //函数绑定
		thread t2(loadingUiT, cons);
		t1.join();
		t2.join();
		loadingData(cons);//重新加载数据
		cout << endl << endl <<endl<<"整理成功" << endl;
		break;
	}
	//退出
	case '0': {
		cout << endl<< endl<< endl<<endl<<"退出" << endl;
		exit(0);
		break;
	}
	default:
		break;
	}
}