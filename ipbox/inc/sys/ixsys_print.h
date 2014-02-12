/* ******************************************************************************************************************
 * Copyright (c) 2013, JBSignal Co.
 * All rights reserved.
 * ******************************************************************************************************************
 * wuyong.yi@jbsignal.com / 2013.2.7.
 * ******************************************************************************************************************
 * debug message hander
 * ****************************************************************************************************************** */

#ifndef IXSYS_PRINT_H_
#define IXSYS_PRINT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ****************************************************************************************************************** */

#include "ix_stddefs.h"
#include <stdarg.h>
#include <string.h>

/* ****************************************************************************************************************** */

#ifdef PROJECT_DEBUG
/// 단순 trace
# define print_here()               ixsys_print(0, "#TRACE# %s : %s() - %d\n", __FILE__, __FUNCTION__, __LINE__);
/// 일반적인 메세지 logging
# define print_debug(msg, args...)  ixsys_print(0, msg, ##args)
/// 에러 상황시 에러임을 표시하고 개행문자를 자동으로 삽입
# define print_error(msg, args...)  ixsys_print(1, "#ERR#[%s:%d] "msg"\n", __FUNCTION__, __LINE__, ##args)
/// 에러는 아니지만 상황 인식이 필요하다 판단. 개행문자 자동 삽입
# define print_warn(msg, args...)   ixsys_print(1, "#WARN#[%s:%d] "msg"\n", __FUNCTION__, __LINE__, ##args)
/// binary stream의 dump용
# define print_dump(t, d, s)        ixsys_print_dump(t, d, s)
#else   // PROJECT_DEBUG
/// 단순 trace
# define print_here()
/// 일반적인 메세지 logging
# define print_debug(msg, args...)
/// 에러 상황시 에러임을 표시하고 개행문자를 자동으로 삽입
# define print_error(msg, args...)
/// 에러는 아니지만 상황 인식이 필요하다 판단. 개행문자 자동 삽입
# define print_warn(msg, args...)
/// binary stream의 dump용
# define print_dump(t, d, s)
#endif  /* PROJECT_DEBUG */

/* ****************************************************************************************************************** */

/// system logging 함수 초기화.
void ixsys_print_init();
/// logging용 메세지 출력 - 단문 메세지.
/// @param err_flag stderr를 통해 출력할 것인지 결정
/// @param format   printf 형 함수인자 인계받음
void ixsys_print(int err_flag, char *format, ...);
/// binary stream을 16진수와 ASCII형태로 출력.
/// @param title dump시 title
/// @param data binary stream
/// @param size stream size
void ixsys_print_dump(char *title, uint8_t *data, size_t size);

/* ****************************************************************************************************************** */

#ifdef __cplusplus
}
#endif

#endif  /* IX_PRINT_H_ */

/* end of file ****************************************************************************************************** */
