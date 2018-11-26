//
//  util.cpp
//  
//
//  Created by Mac on 08/11/2018.
//

#include "util.hpp"
using namespace std;
unsigned long getValue(std::string s){
    unsigned long result = 0;
    unsigned long k = 16 * 16 * 16 * 16 * 16;
    for(int i = 0 ; i < s.length(); i++){
        char temp = s[i];
        
        switch (temp) {
            case 'a':
                result += 10 * k;
                break;
            case 'b':
                result += 11 * k;
                break;
            case 'c':
                result += 12 * k;
                break;
            case 'd':
                result += 13 * k;
                break;
            case 'e':
                result += 14 * k;
                break;
            case 'f':
                result += 15 * k;
                break;
            default:
                int tempi = (int)temp - 48;
                result += tempi * k;
                break;
        }
        if(k !=1) k /= 16;
    }
    return result;
}


