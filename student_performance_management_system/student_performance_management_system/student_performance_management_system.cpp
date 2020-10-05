#include<iostream>
#include<winsock.h>
#include<mysql.h>
#include<string>
using namespace std;
MYSQL mysql;              //Mysql连接描述符
MYSQL_FIELD* fd;          //字段列数组
char field[100][100];           //存字段列数组
MYSQL_RES* res;             //查询结果集
MYSQL_ROW row;              //char**二维数组，存放一条条记录

bool connectDatabase();     //连接数据库
void freeConnect();         //关闭连接并释放数据库
void adminLogin(string loginId, string loginPassword);          //管理员登录
void studentLogin(string loginId, string loginPassword);        //学生登录
void adminRun();                                               //管理员管理
void studentRun(string loginId);         //学生管理
void runStudent();                                           //学生表管理
void addStudent();                                            //增加学生信息
void deleteStudent();                                         //删除学生信息
void updateStudent();                                        //更新学生信息
void selectStudent();                                        //查询学生信息
void runCourse();
void addCourse();
void deleteCourse();
void updateCourse();
void selectCourse();
void runPerformance();
void addPerformance();
void deletePerformance();
void updatePerformance();
void selectPerformance();
void alterAdminPsw();
void studentSelectStudent(string studentID);
void studentSelectPerformance(string studentID);
void alterStudentPsw(string studentID);
int main()
{
	if (!connectDatabase())
		return 0;
	cout << "请选择登录方式：" << endl;
	cout << "1.管理员登录" << endl;
	cout << "2.普通用户登录" << endl;
	int loginFlag;
	cin >> loginFlag;
	string loginId, loginPassword;
	cout << "请输入登录账号：";
	cin >> loginId;
	cout << "请输入登录密码：";
	cin >> loginPassword;
	switch (loginFlag) {
		case 1: 
			adminLogin(loginId, loginPassword);
			break;
		case 2: 
			studentLogin(loginId, loginPassword);
			break;
		default: break;
	}
	freeConnect();
	exit(0);

}

bool connectDatabase()
{
	mysql_init(&mysql);         //初始化数据库
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");  //设置编码方式
	const char host[]{ "localhost" };
	const char user[]{ "root" };
	const char psw[]{ "Luzhou123" };
	const char dbName[]{ "student" };
	const int port = 3306;
	//连接数据库
	if (!mysql_real_connect(&mysql, host, user, psw, dbName, port, NULL, 0)) {
		cout << "连接数据库失败！" << endl;
		return false;
	}
	else {
		cout << "连接数据库成功！" << endl;
		return true;
	}
}

void freeConnect()
{
	mysql_free_result(res);         //释放结果集使用内存
	mysql_close(&mysql);             //关闭连接
}

void adminLogin(string loginId, string loginPassword)
{
	string s = "SELECT * FROM administrator WHERE admin_id=" + loginId + " AND admin_password=" + loginPassword;
	if (!mysql_query(&mysql, s.c_str())) {
		res = mysql_store_result(&mysql);
		if (res != NULL) 
			adminRun();
		else
			cout << "登录失败！" << endl;
	}
}

void studentLogin(string loginId, string loginPassword)
{
	string s = "SELECT * FROM student WHERE stu_id=" + loginId + " AND stu_password=" + loginPassword;
	if (!mysql_query(&mysql, s.c_str())) {
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		if (row != NULL)
			studentRun(loginId);
		else
			cout << "登录失败！" << endl;
	}
}

void adminRun()
{
	int chooseFlag;
	while (true) {
		system("cls");
		cout << "学生成绩管理系统(管理员)" << endl;
		cout << "1.修改学生信息" << endl;
		cout << "2.修改课程信息" << endl;
		cout << "3.修改成绩信息" << endl;
		cout << "4.修改密码" << endl;
		cout << "0.退出系统" << endl;
		cin >> chooseFlag;
		if (!chooseFlag)  break;
		switch (chooseFlag) {
		case 1:
			runStudent();
			break;
		case 2:
			runCourse();
			break;
		case 3:
			runPerformance();
			break;
		case 4:
			alterAdminPsw();
		default:break;
		}
	}
}

void runStudent()
{
	int chooseFlag;
	while (true) {
		system("cls");
		cout << "1.添加学生信息" << endl;
		cout << "2.删除学生信息" << endl;
		cout << "3.修改学生信息" << endl;
		cout << "4.查询学生信息" << endl;
		cout << "0.退出" << endl;
		cin >> chooseFlag;
		if (!chooseFlag)  break;
		switch (chooseFlag) {
		case 1:
			addStudent();
			break;
		case 2:
			deleteStudent();
			break;
		case 3:
			updateStudent();
			break;
		case 4:
			selectStudent();
			break;
		default:break;
		}
	}
}    

void addStudent() {
	char chooseFlag = 'Y';
	string strStuId, strStuName, strStuSex;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入学生学号：";
		cin >> strStuId;
		cout << "请输入学生名字：";
		cin >> strStuName;
		cout << "请输入学生性别：";
		cin >> strStuSex;
		string s = "INSERT INTO student(stu_id,stu_name,stu_sex) VALUES('" + strStuId + "', '" + strStuName + "', '" + strStuSex + "')";
		if (!mysql_query(&mysql, s.c_str())) {
			cout << "插入成功！" << endl << "是否继续插入(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "插入失败！" << endl << "是否重新插入(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void deleteStudent()
{
	char chooseFlag = 'Y';
	string strStuId;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入学生学号：";
		cin >> strStuId;
		string s = "DELETE FROM student WHERE stu_id=" + strStuId;
		if (!mysql_query(&mysql, s.c_str())) {
			cout << "删除成功！" << endl << "是否继续删除(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "删除失败！" << endl << "是否重新删除(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void updateStudent()
{
	char chooseFlag = 'Y';
	string strOldStuId,strNewStuId,strStuName,strStuSex;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入需要修改的学生学号：";
		cin >> strOldStuId;
		cout << "请输入更新后的学生学号：";
		cin >> strNewStuId;
		cout << "请输入更新后学生名字：";
		cin >> strStuName;
		cout << "请输入更新后学生性别：";
		cin >> strStuSex;
		string s = "UPDATE  student SET stu_id=" + strNewStuId + ",stu_name='" + strStuName + "',stu_sex='" + strStuSex+"' WHERE stu_id="+strOldStuId;
		if (!mysql_query(&mysql, s.c_str())) {
			cout << "更新成功！" << endl << "是否继续更新(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "更新失败！" << endl << "是否重新更新(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void selectStudent()
{
	char chooseFlag = 'Y';
	string strStuId;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入学生学号：";
		cin >> strStuId;
		string s = "SELECT * FROM student WHERE stu_id=" + strStuId;
		if (!mysql_query(&mysql, s.c_str())) {
			res = mysql_store_result(&mysql);
			row = mysql_fetch_row(res);
			if (row != NULL){
				cout << "学号：" << *row << endl;
				cout << "姓名：" << *(row + 1) << endl;
				cout << "性别：" << *(row + 2) << endl;
				cout << "登录密码：" << *(row + 3) << endl;
			}	
			else {
				cout << "未查询到该学生信息" << endl;
			}
			cout<< "是否继续查询(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "查询失败！" << endl << "是否重新查询(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void runCourse()
{
	int chooseFlag;
	while (true) {
		system("cls");
		cout << "1.添加课程信息" << endl;
		cout << "2.删除课程信息" << endl;
		cout << "3.修改课程信息" << endl;
		cout << "4.查询课程信息" << endl;
		cout << "0.退出" << endl;
		cin >> chooseFlag;
		if (!chooseFlag)  break;
		switch (chooseFlag) {
		case 1:
			addCourse();
			break;
		case 2:
			deleteCourse();
			break;
		case 3:
			updateCourse();
			break;
		case 4:
			selectCourse();
			break;
		default:break;
		}
	}
}
void addCourse() {
	char chooseFlag = 'Y';
	string strCourseId, strCourseName, strCourseCredit,strCourseType;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入课程号：";
		cin >> strCourseId;
		cout << "请输入课程名：";
		cin >> strCourseName;
		cout << "请输入课程学分：";
		cin >> strCourseCredit;
		cout << "请输入课程类型(必修/选修)：";
		cin >> strCourseType;
		string s = "INSERT INTO course(course_id,course_name,course_credit,course_type) VALUES('" + strCourseId + "', '" + strCourseName + "', '" + strCourseCredit + "','"+strCourseType+"')";
		if (!mysql_query(&mysql, s.c_str())) {
			cout << "添加成功！" << endl << "是否继续添加(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "添加失败！" << endl << "是否重新添加(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void deleteCourse()
{
	char chooseFlag = 'Y';
	string strCourseId;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入课程号：";
		cin >> strCourseId;
		string s = "DELETE FROM course WHERE course_id=" + strCourseId;
		if (!mysql_query(&mysql, s.c_str())) {
			cout << "删除成功！" << endl << "是否继续删除(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "删除失败！" << endl << "是否重新删除(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void updateCourse()
{
	char chooseFlag = 'Y';
	string strOldCourseId, strNewCourseId, strCourseName, strCourseCredit,strCourseType;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入需要修改的课程号：";
		cin >> strOldCourseId;
		cout << "请输入更新后的课程号：";
		cin >> strNewCourseId;
		cout << "请输入更新后课程名：";
		cin >> strCourseName;
		cout << "请输入更新后课程学分：";
		cin >> strCourseCredit;
		cout << "请输入修改后课程类型：";
		cin >> strCourseType;
		string s = "UPDATE  course SET course_id=" + strNewCourseId + ",course_name='" + strCourseName + "',course_credit='" + strCourseCredit + "',course_type='"+strCourseType+"' WHERE course_id=" + strOldCourseId;
		if (!mysql_query(&mysql, s.c_str())) {
			cout << "更新成功！" << endl << "是否继续更新(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "更新失败！" << endl << "是否重新更新(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void selectCourse()
{
	char chooseFlag = 'Y';
	string strCourseId;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入课程号：";
		cin >> strCourseId;
		string s = "SELECT * FROM course WHERE course_id=" + strCourseId;
		if (!mysql_query(&mysql, s.c_str())) {
			res = mysql_store_result(&mysql);
			row = mysql_fetch_row(res);
			if (row != NULL) {
				cout << "课程号：" << *row << endl;
				cout << "课程名：" << *(row + 1) << endl;
				cout << "课程学分：" << *(row + 2) << endl;
				cout << "课程类型：" << *(row + 3) << endl;
			}
			else {
				cout << "未查询到该课程信息" << endl;
			}
			cout << "是否继续查询(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "查询失败！" << endl << "是否重新查询(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void runPerformance()
{
	int chooseFlag;
	while (true) {
		system("cls");
		cout << "1.添加成绩信息" << endl;
		cout << "2.删除成绩信息" << endl;
		cout << "3.修改成绩信息" << endl;
		cout << "4.查询成绩信息" << endl;
		cout << "0.退出" << endl;
		cin >> chooseFlag;
		if (!chooseFlag)  break;
		switch (chooseFlag) {
		case 1:
			addPerformance();
			break;
		case 2:
			deletePerformance();
			break;
		case 3:
			updatePerformance();
			break;
		case 4:
			selectPerformance();
			break;
		default:break;
		}
	}
}

void addPerformance() {
	char chooseFlag = 'Y';
	string strPstuId, strPcourseId, strPerformanceNormal, strPerformanceExam,strPerformanceTotal;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入学生学号：";
		cin >> strPstuId;
		cout << "请输入课程号：";
		cin >> strPcourseId;
		cout << "请输入平时成绩：";
		cin >> strPerformanceNormal;
		cout << "请输入考试成绩：";
		cin >> strPerformanceExam;
		string s = "INSERT INTO performance(pstu_id,pcourse_id,performance_normal,performance_exam) VALUES('" +strPstuId + "', '" + strPcourseId + "', '" + strPerformanceNormal + "','" + strPerformanceExam + "')";
		if (!mysql_query(&mysql, s.c_str())) {
			cout << "添加成功！" << endl << "是否继续添加(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "添加失败！" << endl << "是否重新添加(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void deletePerformance()
{
	char chooseFlag = 'Y';
	string strPstuId,strPcourseId;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入学生学号：";
		cin >> strPstuId;
		cout << "请输入课程名：";
		cin >> strPcourseId;
		string s = "DELETE FROM performance WHERE pstu_id=" + strPstuId+" AND pcourse_id="+strPcourseId;
		if (!mysql_query(&mysql, s.c_str())) {
			cout << "删除成功！" << endl << "是否继续删除(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "删除失败！" << endl << "是否重新删除(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void updatePerformance()
{
	char chooseFlag = 'Y';
	string strOldPstuId, strNewPstuId, strOldPcourseId, strNewPcourseId, strPerformanceNormal,strPerformanceExam;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入需要修改成绩的学生学号及课程号：";
		cin >> strOldPstuId>>strOldPcourseId;
		cout << "请输入更新后的学生学号：";
		cin >> strNewPstuId;
		cout << "请输入更新后课程号：";
		cin >> strNewPcourseId;
		cout << "请输入更新后平时成绩：";
		cin >> strPerformanceNormal;
		cout << "请输入修改后考试成绩：";
		cin >> strPerformanceExam;
		string s = "UPDATE  performance SET pstu_id=" + strNewPstuId + ",pcourse_id='" + strNewPcourseId + "',performance_normal='" + strPerformanceNormal + "',performance_exam='" + strPerformanceExam + "' WHERE pstu_id=" + strOldPstuId+" AND pcourse_id="+strOldPcourseId;
		if (!mysql_query(&mysql, s.c_str())) {
			cout << "更新成功！" << endl << "是否继续更新(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "更新失败！" << endl << "是否重新更新(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void selectPerformance()
{
	char chooseFlag = 'Y';
	string strPstuId,strPcourseId;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入查询的学生学号及课程号：";
		cin >> strPstuId>>strPcourseId;
		string s = "SELECT * FROM performance WHERE pstu_id=" + strPstuId+" AND pcourse_id="+strPcourseId;
		if (!mysql_query(&mysql, s.c_str())) {
			res = mysql_store_result(&mysql);
			row = mysql_fetch_row(res);
			if (row != NULL) {
				cout << "学生学号：" << *row << endl;
				cout << "课程号：" << *(row + 1) << endl;
				cout << "平时成绩：" << *(row + 2) << endl;
				cout << "考试成绩：" << *(row + 3) << endl;
				cout << "总成绩：" << *(row + 4) << endl;
			}
			else {
				cout << "未查询到该成绩信息" << endl;
			}
			cout << "是否继续查询(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "查询失败！" << endl << "是否重新查询(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void alterAdminPsw()
{
	string strAdminID, strOldPsw, strNewPsw;
	cout << "请输入账号：";
	cin >> strAdminID;
	cout << "请输入当前密码：";
	cin >> strOldPsw;
	string s = "SELECT * FROM administrator WHERE admin_id=" + strAdminID + " AND admin_password=" + strOldPsw;
	if (!mysql_query(&mysql, s.c_str())) {
		res = mysql_store_result(&mysql);
		if (res != NULL) {
			string temp;
			do {
				cout << "请输入新密码：";
				cin >> strNewPsw;
				cout << "请确认新密码：";
				cin >> temp;
				if (strNewPsw == temp)
					break;
				else
					cout << "密码不一致！" << endl;
			} while (true);
			string s2 = "UPDATE administrator SET admin_password=" + strNewPsw+" WHERE admin_id="+strAdminID;
			if (!mysql_query(&mysql, s2.c_str())) {
				system("cls");
				cout << "修改成功！" << endl;
				cout << "请重新登录!" << endl;
				main();
			}
			else
				cout << "修改失败! ";
		}
		else
			cout << "密码错误！" << endl;
	}
}

void studentRun(string studentID)
{
	int chooseFlag;
	while (true) {
		system("cls");
		cout << "学生成绩管理系统(学生版)" << endl;
		cout << "1.查看学籍信息" << endl;
		cout << "2.查看成绩信息" << endl;
		cout << "3.修改密码" << endl;
		cout << "0.退出系统" << endl;
		cin >> chooseFlag;
		if (!chooseFlag)  break;
		switch (chooseFlag) {
		case 1:
			studentSelectStudent(studentID);
			break;
		case 2:
			studentSelectPerformance(studentID);
			break;
		case 3:
			alterStudentPsw(studentID);
			break;
		default:break;
		}
	}
}

void studentSelectStudent(string studentID)
{
	string s = "SELECT * FROM student WHERE stu_id=" + studentID;
	if (!mysql_query(&mysql, s.c_str())) {
		res = mysql_store_result(&mysql);
		row = mysql_fetch_row(res);
		if (res != NULL) {
			cout << "学号：" << *row << endl;
			cout << "姓名：" << *(row + 1) << endl;
			cout << "性别：" << *(row + 2) << endl;
			cout << "登录密码：" << *(row + 3) << endl;
			cin.get();
			cin.get();
		}
		else
			cout << "查询失败！";
	}
	else
		cout << "查询失败!";
}

void studentSelectPerformance(string studentID)
{
	char chooseFlag = 'Y';
	string  strPcourseId;
	while (chooseFlag == 'Y' || chooseFlag == 'y') {
		cout << "请输入需要查询的课程号：";
		cin >> strPcourseId;
		string s = "SELECT * FROM performance WHERE pstu_id='" + studentID + "' AND pcourse_id='" + strPcourseId+"'";
		if (!mysql_query(&mysql, s.c_str())) {
			res = mysql_store_result(&mysql);
			row = mysql_fetch_row(res);
			if (row != NULL) {
				cout << "学生学号：" << *row << endl;
				cout << "课程号：" << *(row + 1) << endl;
				cout << "平时成绩：" << *(row + 2) << endl;
				cout << "考试成绩：" << *(row + 3) << endl;
				cout << "总成绩：" << *(row + 4) << endl;
			}
			else {
				cout << "未查询到该成绩信息" << endl;
			}
			cout << "是否继续查询(Y/N):";
			cin >> chooseFlag;
		}
		else {
			cout << "查询失败！" << endl << "是否重新查询(Y/N):";
			cin >> chooseFlag;
		}
	}
}

void alterStudentPsw(string studentID)
{
	string strOldPsw, strNewPsw;
	cout << "请输入当前密码：";
	cin >> strOldPsw;
	string s = "SELECT * FROM student WHERE stu_id=" + studentID + " AND stu_password=" + strOldPsw;
	if (!mysql_query(&mysql, s.c_str())) {
		res = mysql_store_result(&mysql);
		if (res != NULL) {
			string temp;
			do {
				cout << "请输入新密码：";
				cin >> strNewPsw;
				cout << "请确认新密码：";
				cin >> temp;
				if (strNewPsw == temp)
					break;
				else
					cout << "密码不一致！" << endl;
			} while (true);
			string s2 = "UPDATE student SET stu_password=" + strNewPsw + " WHERE stu_id=" + studentID;
			if (!mysql_query(&mysql, s2.c_str())) {
				system("cls");
				cout << "修改成功！" << endl;
				cout << "请重新登录!" << endl;
				main();
			}
			else
				cout << "修改失败! ";
		}
		else
			cout << "密码错误！" << endl;
	}
}