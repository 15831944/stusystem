#include "Action.h"
bool isExit = false;
MTree<Mdata> tree;
MTree<Mdata> treeName;

//�ļ������������
void add(MString& data, int id) {
	Mdata tmp;
	{ //����������ǰ�ͷŴ򿪵��ļ�
		//���ļ�д���ļ�
		MFile mfile("nameEN.bin", ios::out | ios::binary | ios::app);
		tmp = mfile.write(data);
	}
	tmp.mId = id;
	//�������ڵ�
	tree.insert(tmp);//����id���ڵ�

	MString tmpId;
	MString tmpName;
	MString str = read(tmp);//��ȡ�ļ�����
	idAndName(str, tmpId, tmpName);//��ȡ�ļ�����id��name
	tmp.mId = mHash(tmpName.getStr(), tmpName.length());
	treeName.insert(tmp);
}

//����ɾ��
bool del(int id) {
	Mdata data;
	data.mId = id;
	try {
		data = tree.find(data)->data; //���ҽڵ�
	}
	catch (MException& e) {
		cout << e.what() << endl;
		return false;
	}
	{//��������ǰ���ļ�
		//���ļ�ɾ������
		MFile mfile("nameEN.bin", ios::out | ios::binary | ios::in);
		mfile.write((char)0xFB, data.mOffset);
	}
	//�������ڵ�
	tree.remove(data);

	MString tmpId;
	MString tmpName;
	MString str = read(data);//��ȡ�ļ�����
	idAndName(str, tmpId, tmpName);//��ȡ�ļ�����id��name
	data.mId = mHash(tmpName.getStr(), tmpName.length());
	MTree<Mdata>::TreeNode* node = treeName.find(data);
	if (node->next == nullptr) {
		treeName.remove(data);
		return true;
	}
	
	str = read(node->data);//��ȡ�ļ�����
	idAndName(str, tmpId, tmpName);//��ȡ�ļ�����id��name
	int curId = strtol(tmpId.getStr(), nullptr, 10);//ת��int
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
		str = read(next->data);//��ȡ�ļ�����
		idAndName(str, tmpId, tmpName);//��ȡ�ļ�����id��name
		curId = strtol(tmpId.getStr(), nullptr, 10);//ת��int
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

		char sign = '\0'; //�����־λ
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

//�ָ��ַ����е�ѧ�ź�����
void idAndName(MString& des, MString& id, MString& name) {
	char szId[16] = { 0 };
	char szName[26] = { 0 };
	sscanf(des.getStr(), "%s \"%[^\"]\"", szId, szName);
	id = szId;
	name = szName;
}

//��ȡ�ַ���
MString read(Mdata& data){
	MFile mfile("nameEN.bin", ios::in | ios::binary );
	return mfile.read(data);
}

//������hash���ҽڵ�
MTree<Mdata>::TreeNode* findName(MString& name) {
	int hash = mHash(name.getStr(), name.length());
	Mdata tmp;
	tmp.mId = hash;
	MTree<Mdata>::TreeNode* node = nullptr;
	try {
		node = treeName.find(tmp); //����ID
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
	auto loadingUiT = std::bind(loadingUi, cons); //������
	thread t2(loadingUiT, cons);
	t1.detach();
	t3.detach();
	t2.join();
}



//��ȡ�û�����������������ں��Ա�
MString inputAllData(UserInput& input) {
	MString name;
	while (true) {
		cout << "����(�������Ȳ��ܳ���25�ֽڣ����򽫽�ȡǰ25�ֽڴ���): ";
		name = input.inputStr(26);
		if (name.length() <= 0 || name[0] == ' ') {
			cout << "error: ��������Ϊ�ջ����������Կո�ͷ! " << endl;
		}
		else {
			break;
		}
	}

	MString date; //�����û����������
	struct DATE { //���ڽṹ��
		int nYear; //��ʾ��
		int nMonth;//��ʾ��
		int nDay;  //��ʾ��
		int nMonthOfDays[12];
	} dataStruct = { -1,-1,-1,31,28,31,30,31,30,31,31,30,31,30,31 };
	//�ж��Ƿ�Ϊ���겢�Ѷ��¸ĳ�29��
	while (true) {
		cout << "��������(YYYY-MM-DD) : ";
		date = input.inputStr(16);
		sscanf(date.getStr(), "%d-%d-%d", &dataStruct.nYear, &dataStruct.nMonth, &dataStruct.nDay);//��ʽ�����ַ���
		//�ж��Ƿ�Ϊ���겢�Ѷ��¸ĳ�29��
		if (dataStruct.nYear % 4 == 0 && dataStruct.nYear % 100 != 0) {
			dataStruct.nMonthOfDays[1] = 29;
		}

		if (dataStruct.nYear == -1 || dataStruct.nMonth == -1 || dataStruct.nDay == -1) {//�ж������ʽ
			cout << "error: �������ڸ�ʽ��YYYY-MM-DD��!" << endl;
			continue;
		}
		else {
			if (dataStruct.nYear > 2019) {
				cout << "error: ��ݴ���!" << endl;
				continue;
			}
			if (dataStruct.nMonth < 1 || dataStruct.nMonth >12) {
				cout << "error: һ��ֻ��12���£�1~12��!" << endl;
				continue;
			}

			//�жϱ����Ƿ����ĳ��
			if (dataStruct.nDay > dataStruct.nMonthOfDays[dataStruct.nMonth - 1] || dataStruct.nDay <= 0) {
				if (dataStruct.nMonth == 2) {
					if (dataStruct.nMonthOfDays[1] = 29) {
						cout << "error: " <<"����2�·�ֻ��29��! " << endl;
					}
					else {
						cout << "error: " << "ƽ��2�·�ֻ��28��! " << endl;
					}
				}
				else {
					cout << "error: " << dataStruct.nMonth << "����û�� " << dataStruct.nDay << "��! " << endl;
				}
				continue;
			}
			break;
		}
	}
	
	//��ʽ���ڸ�ʽ������2λ��0
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
	//�ж��Ա�����
	while (true) {
		cout << "�Ա�( �� M, Ů F ): ";
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
		cout << "error: �Ա��ǣ��� M/m, Ů F/f��!" << endl;
	}
	cout << endl;
	MString data;
	data.append(" \"").append(name).append("\" ").append(date).append(" ").append(sex);
	return std::move(data);
}

//����hash�㷨
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

//��������
void loading() {
	MFile file("nameEN.bin", ios::in | ios::binary);
	fstream& ifs = file.getFileP();
	
	long long curOffset = 0; //��¼��ǰ�ļ��α�λ��
	short curSize = 0; //��¼��ǰ���ݳ���
	char sign = '\0'; //��¼��־λ
	char szId[16] = { 0 };//��¼ѧ��id
	char szName[26] = { 0 };
	MString s;
	while (true) {
		curOffset = ifs.tellg();//��ȡ��ǰ�α�λ��
		ifs.read((char*)& sign, sizeof(char));//��ȡ��־λ
		ifs.read((char*)& curSize, sizeof(short));//��ȡ���ݳ���

		//�����Ѿ�ɾ������
		if (sign == (char)0xFB) {//�жϱ�־λ
			ifs.seekg(curSize, ios::cur);//������ǰ����
			sign = '\0'; //��ʼ����־λ
			continue;//������ǰѭ��
		}
		//�ж��ļ�����
		if (ifs.eof()) {
			break;
		}

		//��ȡ�û�����
		Mdata tmp;
		
		tmp.mOffset = curOffset;
		tmp.mSize = curSize;

		s = file.read(tmp);
		sscanf(s.getStr(), "%s \"%[^\"]\"", szId, szName);
		tmp.mId = strtol(szId, nullptr, 10);
		//д����
		tree.insert(tmp);
	}
	isExit = true; //���ͽ�����Ϣ
}

//��������
void loadingName() {
	MFile file("nameEN.bin", ios::in | ios::binary);
	fstream& ifs = file.getFileP();

	long long curOffset = 0; //��¼��ǰ�ļ��α�λ��
	short curSize = 0; //��¼��ǰ���ݳ���
	char sign = '\0'; //��¼��־λ
	char szId[16] = { 0 };//��¼ѧ��id
	char szName[26] = { 0 };
	MString s;
	while (true) {
		curOffset = ifs.tellg();//��ȡ��ǰ�α�λ��
		ifs.read((char*)& sign, sizeof(char));//��ȡ��־λ
		ifs.read((char*)& curSize, sizeof(short));//��ȡ���ݳ���

		//�����Ѿ�ɾ������
		if (sign == (char)0xFB) {//�жϱ�־λ
			ifs.seekg(curSize, ios::cur);//������ǰ����
			sign = '\0'; //��ʼ����־λ
			continue;//������ǰѭ��
		}
		//�ж��ļ�����
		if (ifs.eof()) {
			break;
		}

		//��ȡ�û�����
		Mdata tmp ;

		tmp.mOffset = curOffset;
		tmp.mSize = curSize;

		s = file.read(tmp);
		sscanf(s.getStr(), "%s \"%[^\"]\"", szId, szName);
		tmp.mId = mHash(szName,strlen(szName));
		//д����
		treeName.insert(tmp);
	}
	//isExit = false; //���ͽ�����Ϣ
}

//�������ݵȴ�ui
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

//������,���Ʋ˵��͹��ܵ�����
void control(UserInput& input, MUI& mui, Console& cons) {
	MString data;
	char choose = input.inputCh();
	cout << endl;
	switch (choose)
	{
	//���
	case '1': {
		//������ӱ���
		mui.showTitleUi("���");
		//��ȡ�û�����
		MString str =inputAllData(input);
		//��ȡ���id+1
		int id = 0;
		try {
			id = tree.Max().mId + 1;
		}
		catch (MException& e) {
			return;
		}
		//�����ַ���
		data = input.intToStr(id, 10);//��idת���ַ���
		data.append(str);
		add(data,id); //���ú�����������ļ�
		cout << endl << "��ӳɹ�" << endl;
		break;
	}
	//ɾ��
	case '2': {//ɾ��
		mui.showTitleUi("ɾ��");//��ʾ��ǰ����
		mui.chooseUi();//��ʾѡ�����UI
		char choose = input.inputCh();//��ȡ�û�����Ĳ˵���
		cout << endl << endl << endl;//���ڽ����Ű�
		switch (choose) {
		case '1': {
			cout << "������ɾ����ѧ��: ";
			//��ȡ�û�����
			MString idStr = input.inputStr(10); 

			//�ַ�ת����
			int id = input.strToInt(idStr.getStr());
			if (del(id)) {
				cout << endl << "ɾ���ɹ�! " << endl;
			}
			break;
		}
		case '2':{
			cout << "������ɾ��������: ";
			MString name = input.inputStr(26); //��ȡ�û�����
			//���ҽ�ɾ���Ľڵ�
			MTree<Mdata>::TreeNode* node = findName(name);

			MString idStr; //�ݴ�id�ַ���
			Mdata tmp;
			if (node == nullptr) {
				return;
			}
			else if (node->next != nullptr) {
				//��д��ѯ����
				MFile mfile("nameEN.bin", ios::binary | ios::in);
				//��ʾ��ѯ���
				mui.showDataTitle();
				while (node != nullptr) {
					tmp = node->data;
					data = mfile.read(tmp);
					mui.showData(data);
					node = node->next;
				}
				cout << "����������������ѧ��: ";
				idStr = input.inputStr(10);
				cout << endl;
			}
			else {
				data = read(node->data);
				idAndName(data, idStr, name);
			}	
			
			//�����ǰ�ڵ�û����ֱͬ��ɾ��
			int id = input.strToInt(idStr.getStr());
			if (del(id)) {
				cout << "ɾ���ɹ�! " << endl;;
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	//�޸�
	case '3': {//�޸�
		mui.showTitleUi("�޸�");
		mui.chooseUi();
		char choose = input.inputCh();
		cout << endl << endl << endl;
		switch (choose) {
		case '1': {
			cout << "�������޸ĵ�ѧ��ѧ��: ";
			MString idStr = input.inputStr(10); //��ȡ�û�����id
			//�ַ�ת����
			int id = input.strToInt(idStr.getStr());
			if (!del(id)) {
				cout << "��������ȷ��ѧ��! " << endl;
				return;
			}
			//��ȡ�û�����
			MString str = inputAllData(input);
			data.append(idStr).append(str);
			add(data,id);
			cout << endl << "�޸ĳɹ�!" << endl;
			break;
		}
		case '2': {
			cout << "������ѧ������: ";
			MString name = input.inputStr(26); //��ȡ�û�����

			MTree<Mdata>::TreeNode* node = findName(name);

			MString idStr; //�ݴ�id�ַ���
			Mdata tmp;

			if (node == nullptr) {
				return;
			}
			else if (node->next != nullptr) {
				//��д��ѯ����
				MFile mfile("nameEN.bin", ios::binary | ios::in);
				//��ʾ��ѯ���
				mui.showDataTitle();
				while (node != nullptr) {
					tmp = node->data;
					data = mfile.read(tmp);
					mui.showData(data);
					node = node->next;
				}
				cout << "����������������ѧ��: ";
				idStr = input.inputStr(10);
			}
			else {
				data = read(node->data);
				idAndName(data, idStr, name);
			}
			//�ַ�ת����
			int id = input.strToInt(idStr.getStr());
			if (!del(id)) {
				cout << "��������ȷ��ѧ��! " << endl;
				return;
			}
			//��ȡ�û�����
			MString str = inputAllData(input);
			MString dataStr;
			dataStr.append(idStr).append(str);
			add(dataStr,id);//�����
			cout <<endl<< "�޸ĳɹ���" << endl;

			break;
		}
		default:
			break;
		}
		break;
	}
	//��ѯ
	case '4': { //��ѯ
		mui.showTitleUi("��ѯ");
		mui.chooseUi();
		char choose = input.inputCh();
		cout << endl << endl << endl;
		switch (choose) {
		case '1': {
			cout << "������ѧ��: ";
			MString ids = input.inputStr(10); //��ȡ�û�����
			//�ַ�ת����
			int id = input.strToInt(ids.getStr());
			Mdata tmp;
			tmp.mId = id;
			try {
				tmp = tree.find(tmp)->data; //����ID
			}
			catch (MException& e) {
				cout << e.what() << endl;
				return;
			}
			//��д��ѯ����
			MFile mfile("nameEN.bin", ios::binary | ios::in);
			data = mfile.read(tmp);
			//��ʾ��ѯ���
			mui.showDataTitle();
			mui.showData(data);
			
			break;
		}
		case '2': {
			cout << "������ѧ������: ";
			MString name = input.inputStr(26); //��ȡ�û�����
			
			MTree<Mdata>::TreeNode* node = findName(name);

			if (node == nullptr) {
				return;
			}

			Mdata tmp;
			//��д��ѯ����
			MFile mfile("nameEN.bin", ios::binary | ios::in);
			//��ʾ��ѯ���
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
	//����
	case '5': {
		mui.showTitleUi("����");
		system("cls");
		//��������
		thread t1(clearUp);
		auto loadingUiT = std::bind(loadingUi, cons); //������
		thread t2(loadingUiT, cons);
		t1.join();
		t2.join();
		loadingData(cons);//���¼�������
		cout << endl << endl <<endl<<"����ɹ�" << endl;
		break;
	}
	//�˳�
	case '0': {
		cout << endl<< endl<< endl<<endl<<"�˳�" << endl;
		exit(0);
		break;
	}
	default:
		break;
	}
}