#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#define EASTER_OK                0
#define EASTER_INVALID_FILENAME  1
#define EASTER_INVALID_YEARS     2
#define EASTER_IO_ERROR          3

using year_t = unsigned;

#endif /* __PROGTEST__ */


bool IsHTML(const string& path){
    unsigned long i = path.length()-1; // potom změnit na int!!!!
    string fileExtension;
    
    while (path[i] != '.') {
        fileExtension += path[i];
        i--;
        if(i == 0)
            return false;
    }
    for(unsigned int i = 0; i < fileExtension.length(); i++)
        if(fileExtension[i] >= 'A' && fileExtension[i] <= 'Z')
            fileExtension[i] = fileExtension[i]+32;
    if( (fileExtension == "lmth") )
        return true;
    else
        return false;
}

int CountYears(const string& str){
    
    string tmp;
    int k = 0;
    bool interval = false;
    for(unsigned int i = 0; i < str.length(); i++){
        int yearNow;
        tmp += str[i];
        
        if(str[i] == ','){
            
            if(interval){
                int start = yearNow;
                for(int j = start+1; j <= stoi(tmp); j++,k++){
                    yearNow = j;
                }
                tmp = "";
            }else{
                yearNow = stoi(tmp);
                k++;
                tmp = "";
            }
        }
        
        if(str[i] == '-'){
            interval = true;
            yearNow = stoi(tmp);
            tmp = "";
            k++;
        }
        
        if(i == str.length()-1){
            
            if(interval){
                int start = yearNow;
                for(int j = start+1; j <= stoi(tmp); j++,k++){
                    yearNow = j;
                }
                tmp = "";
            }else{
                yearNow = stoi(tmp);
                k++;
                tmp = "";
            }
            
        }
        
    }
    return k;
}

void ParseYears(const string& str, int* parsedYears){
    
    string tmp;
    int k = 0;
    bool interval = false;
    for(unsigned int i = 0; i < str.length(); i++){
    
        tmp += str[i];
        
        if(str[i] == ','){
            
            if(interval){
                int start = parsedYears[k];
                for(int j = start+1; j <= stoi(tmp); j++,k++){
                    parsedYears[k] = j;
                }
                tmp = "";
            }else{
                parsedYears[k] = stoi(tmp);
                k++;
                tmp = "";
            }
        }
        
        if(str[i] == '-'){
            interval = true;
            parsedYears[k] = stoi(tmp);
            tmp = "";
        }
        
        if(i == str.length()-1){
            
            if(interval){
                int start = parsedYears[k];
                for(int j = start; j <= stoi(tmp); j++,k++){
                    parsedYears[k] = j;
                }
                tmp = "";
            }else{
                parsedYears[k] = stoi(tmp);
                k++;
                tmp = "";
            }
            
        }
        
    }
    
}


int easterReport (const string& years, const string& outFileName){
   
    if(!IsHTML(outFileName))
        return EASTER_INVALID_FILENAME;
    
    int numOfYears = CountYears(years);
    int* yearsArray = new int[numOfYears];
    
    // NAPLNENI ROKAMA
    
    ParseYears(years, yearsArray);
    
    // KONTROLA ZDA JSOU OK - nevim jestli i rovno?
    for(int i = 0; i < numOfYears; i++){
        if(yearsArray[i] <= 1582 || yearsArray[i] >= 2200)
            return EASTER_INVALID_YEARS;
    }
    
    
    
    
    
    
    
    // OTEVRENI SOUBORU A KONTROLA IO
    ofstream file (outFileName);
    if(file.fail()){
        delete[] yearsArray;
        yearsArray = NULL;
        return EASTER_IO_ERROR;
    }

    // ZAVRENI SOUBORU
    file.close();
    
    // ODALOKOVANI
    delete[] yearsArray;
    yearsArray = NULL;
    return EASTER_OK;
}


#ifndef __PROGTEST__
int main ( int argc, char * argv[] )
{
    
    cout << easterReport ("2000-3000","out.html") << endl;



    return 0;
    
}
#endif /* __PROGTEST__ */
