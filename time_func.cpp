#include "header.h"

// 시스템의 현재 시간 반환하는 함수 정의

const std::string get_current_time() { //현재 시간 구하는 함수
	time_t now = time(0); //현재시간을 time_t타입으로 설정
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 string

	return buf;
}