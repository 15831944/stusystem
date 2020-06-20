#pragma once
#include <iostream>
#include <fstream>
#include <conio.h>
#include <climits>
#include <thread>
#include <functional>
#include <strsafe.h>
#include "MString.h"
#include "MFile.h"
#include "Mdata.h"
#include "UserInput.h"
#include "MTree.hpp"
#include "MUI.h"
#include "MException.h"
using namespace std;
extern bool isExit;
extern MTree<Mdata> tree;
extern MTree<Mdata> treeName;

//数据添加
void add(MString& data, int id);
//数据删除
bool del(int id);
//分割字符串中的学号和姓名
void idAndName(MString& des, MString& id, MString& name);
//读取文件字符串
MString read(Mdata& data);
//清理数据
void clearUp();
//获取用户输入的姓名出生日期和性别
MString inputAllData(UserInput& input);
//加载数据
void loading();
void loadingName();
//加载数据等待ui
void loadingUi(Console& cons);
//控制器,控制菜单和功能的连接
void control(UserInput& input, MUI& mui, Console& cons);
//计算字符串hash值
unsigned int mHash(char* str, unsigned int len);
//获取姓名hash所在的节点
MTree<Mdata>::TreeNode* findName(MString& name);

void loadingData(Console& cons);