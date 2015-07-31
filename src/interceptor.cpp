/*************************************************************************
	> File Name: interceptor.cpp
	> Author: qianguozheng
	> Mail: guozhengqian0825@126.com
	> Created Time: Thu Jul 30 17:48:33 2015
 ************************************************************************/

#include<iostream>
#include "alpuc0a9.h"
//#include "interceptor.h"
extern unsigned char alpuc_process(unsigned char *tx, unsigned char *rx);
extern "C"{
//unsigned char mid_alpuc_process(unsigned char *tx, unsigned char *rx);

unsigned char mid_alpuc_process(unsigned char *tx, unsigned char *rx)
{
        return alpuc_process(tx, rx);
}

}
