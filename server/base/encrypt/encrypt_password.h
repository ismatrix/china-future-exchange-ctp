/*************************************************************************
    > File Name: encrypt_password.h
    > Created Time: Mon 22 Aug 2016 10:13:49 AM CST
    > Author: 
    > description: 
 ************************************************************************/
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "convert.h"

using namespace std;

string decrypt_account_password(const uint8_t* ibuf, int len);
string encrypt_account_password(const uint8_t* ibuf, const int len);


