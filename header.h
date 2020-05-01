#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> //fscanf,fprintf,fwrite,fread
#include <stdlib.h> //standard lib
#include <iostream> //�����
#include <string.h> //strcpy
#include <sstream> //stringstream
#include <windows.h> //sys command
#include <time.h> //time
#include <stdbool.h> //boolean
#include <type_traits> //std::underlying_type

//NULL_VALUE : �����ڵ�, �й�, �̸�, �а��� ���� ���� �Ҵ����� �ʱ� ���� ���� ��� ����
 //EOF_KEY_VALUE : (����)������ ���� ��Ÿ���� EOF�� � ���ڵ� Ű ������ ũ��
constexpr auto NULL_VALUE = "";
constexpr auto EOF_KEY_VALUE = "-1"; //������ ���� ��� ���ڵ� Ű�� �Ҵ��ϴ� ��

#define EXCEPTION_HANDLING_VALUE -999 //�г�, ������ ���� �� ���� ���� �� ����
#define SINGLE_LINE_BUFFER_SIZE 512 //ȿ������ ���� �� �� ���� �� ���� ũ�⸦ byte������ ��� ���ġ ũ��� ����

#define OP_LENGTH 1 //���� �ڵ� ���� ����
#define DPT_LENGTH 2 //�а� ���� ����
#define NAME_LENGTH 10 //�̸� ���� ����
#define ID_LENGTH 8 //�й� ���� ����

//Insert, Delete, Correct ������ ����
enum class OPERATION_CODE : char{
	//�л� ��ü ���� �ÿ� �����ڵ带 ���ǵ� OPERATION_CODE�߿� �Ҵ�
	no_operation = NULL,
	Insert = 'I',
	Delete = 'D',
	Correct = 'C'
};

//������ => char ��ȯ ���ø�
template<typename Enum>
constexpr auto to_char(Enum value) -> typename std::underlying_type<Enum>::type
{
	return static_cast<typename std::underlying_type<Enum>::type>(value);
};

class Student_string_data //�Է°� �˻縦 ���� �л� ������ ���ڿ� Ŭ����
{
public:
	Student_string_data();

	std::string str_op_code; //�����ڵ�
	std::string str_student_id; //�й�
	std::string str_name; //�̸�
	std::string str_dpt; //�а�
	std::string str_grade; //�г�
	std::string str_score; //����

	void clear_all(); //��� ���ڿ��� ���� ����
};

class Student
{
public:
	//EDIT ���� ������ ��ģ �Ŀ� ����

	Student();
	Student(const char* ID, const char* name, const char* dpt, int grade, int score);
	
	char name[NAME_LENGTH + 1]; //�̸� ('\0' ���� 10�ڸ�)
	char department[DPT_LENGTH + 1]; //�а� ('\0' ���� 2�ڸ�)
	int grade; //�г�(1 ~ 4)
	int score; //����(0 ~ 100)

	void get_Student_ID(char* dst_buffer); //dst_buffer�� �й� ��ȯ

protected: //�й��� ���� �� �񱳸� ���� ����,���ڷ� ������ ������ ������ ��
	char Student_ID[ID_LENGTH + 1]; //�й�(����,���� ����, '\0' ���� 8�ڸ�)
};

class Student_list : public Student //���� ����� ���� �л� ���� ���� ���Ḯ��Ʈ �� ���� �ڵ带 ����
{
public:
	Student_list();
	Student_list(const char* op_code, const char* ID, const char* name, const char* dpt, int grade, int score);
	
	enum OPERATION_CODE op_code; //���� �ڵ�(Insert, Delete, Correct)

	class Student_list* next; //���� �л� ����
	class Student_list* pre; //���� �л� ����
};

/*** student function ***/
bool chk_dpt_list(char** dpt_list_array, int dpt_list_count, char* user_input_dpt); //����ڷκ����� �а� �Է� �� �˻� �Լ�
void insertion_sort(Student_list** src_list, const char* op_code, const char* ID, const char* name, const char* dpt, int grade, int score); //Edit ���� ������ ��ģ �����͵�κ��� ���ο� �л� ��ü�� ���� �� ���������Ͽ� ����Ʈ ����
void create_student_list(Student_list** src_list, char** dpt_list_array, int dpt_list_count, bool flag_use_opcode); //����ڷκ��� �л� �����͸� �Է¹ް� Edit ���� ���� ���� �� �����͸� insertion_sort ����
void deallocate_memory(Student_list** student_list, char** dpt_list_array, int dpt_list_count, int type); //�޸� ����

/*** time function ***/
const std::string get_current_time(); //���� �ý��� �ð� ��ȯ �Լ�

/*** screen I/O function ***/
//flag_use_opcode : true - �����ڵ带 ������ �л� ����Ʈ ����, false - �л� ����Ʈ ����
void disp_main_menu(); //���� �޴� ���
void disp_student_description_bar(int file_mode); //�л� ���� ���� ���
void disp_student_trans_input_bar(char** dpt_list_array, int dpt_list_count); //Ʈ����� ���� �ۼ� �� �Է��ؾ� �� ������ ���
void disp_student_input_bar(char** dpt_list_array, int dpt_list_count); //������ ���� �ۼ� �� �Է��ؾ� �� ������ ���
void disp_log_description_bar(); //�α� Ÿ�Կ� ���� ����
void disp_trans_log_description_bar(); //�α� Ÿ�Կ� ���� ����
void disp_error_msg(const char* message); //���� �޼��� ���
int input_log_file_mode(); //�α� ����� ���� �α� Ÿ�� �Է�
bool input_replace_master_confirm(); //������ ���� ���� ��ü Ȯ��
bool input_init_master_confirm(); //�ʱ� ������ ���� ���� Ȯ��
void input_main_menu(Student_list** src_list, char** dpt_list_array, int dpt_list_count); //���� �޴��� ���� ����� �Է�

/*** file function : �л� ���� �Է�, ������ ���� ������Ʈ �� ȣ�� ***/
//file_mode : 0 - ������ ����, 1 - Ʈ����� ����, 2 - Edit ���� �α�, 3 - Transaction ���� �α�
bool chk_edit_error(Student_list** src_list, Student_string_data** src_data, char** dpt_list_array, int dpt_list_count); //Editing ���� ���� �� �α� ��� �Լ�
bool chk_trans_error(const char* masterKey, const char* transKey, const char* op_code); //Transaction ���� ���� �� �α� ��� �Լ�
//file function : ���ο��� ȣ��
char** allocate_dpt_list_array(int& dpt_list_count); //�а� ����Ʈ ���Ϸκ��� ���� �迭 �Ҵ�
bool create_file(Student_list** src_list, char** dpt_list_array, int dpt_list_count, int file_mode); //�ʱ� ������ ���� ���� �Ǵ� Ʈ����� ���� �ۼ� �Լ�
void print_file(int file_mode); //������ ����, Ʈ����� ����, �α� ���� ��� �Լ�
void update_master(bool flag_replace_master); //������ ���� ���� �Լ�