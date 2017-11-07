﻿/***********************************************************************************************
// 文件名:     syserror.h
// 创建者:     蔡振球
// Email:      zqcai@w.cn
// 创建时间:   2012/2/17 9:03:39
// 内容描述:   定义系统信息函数的错误代码
// 版本信息:   1.0V
************************************************************************************************/
#ifndef BASIC_SYSERROR_H
#define BASIC_SYSERROR_H

#pragma once

__NS_BASIC_START
/////////////////////////////////////////////////////////////////////////////////////////////
//声明
/////////////////////////////////////////////////////////////////////////////////////////////
//定义注册表函数的错误代码
#define REG_ERROR					(0xE0000000 | _ERROR_SYS)	    //系统错误

#define	BASIC_REG_OK					0								//成功，没有错误

#define	BASIC_REG_ERR_GENERIC			(REG_ERROR | 0x0001)			//一般性错误，即未知的错误
#define	BASIC_REG_ERR_MORE_DATA		(REG_ERROR | 0x0002)			//输入的缓冲区空间不够
#define	BASIC_REG_ERR_KEY_NOTFOUND		(REG_ERROR | 0x0003)			//找不到需要查找的Key
#define	BASIC_REG_ERR_FIND_STOP		(REG_ERROR | 0x0004)			//遍历查询终止

/////////////////////////////////////////////////////////////////////////////////////////////
__NS_BASIC_END
#endif 



