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
#endif /* __PROGTEST__ */

bool IsNumber(char character){
    if(character < '0' || character > '9')
        return false;
    else
        return true;
}

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
    int num = 0;
    bool interval = false;
    int lastYear = 0;
    for(unsigned int i = 0; i < str.length(); i++){
        
        if(IsNumber(str[i]))
            tmp += str[i];
        
        if(str[i] == ','){
            if(interval){
                int start = lastYear;
                for(int j = start; j <= stoi(tmp); j++){
                    num++;
                }
                tmp = "";
                interval = false;
            }else{
                if(tmp != ""){
                    num++;
                    tmp = "";
                }
            }
        }
        
        if(str[i] == '-'){
            if(tmp != ""){
                interval = true;
                lastYear = stoi(tmp);
                tmp = "";
            }
        }
        
        if(i == str.length()-1){
            
            if(interval){
                int start = lastYear;
                for(int j = start; j <= stoi(tmp); j++){
                    num++;
                }
                tmp = "";
                interval = false;
            }else{
                if(tmp != ""){
                    num++;
                    tmp = "";
                }
            }
            
        }
        
    }
    return num;
}

void ParseYears(const string& str, int* parsedYears){
    
    string tmp;
    int k = 0;
    bool interval = false;
    for(unsigned int i = 0; i < str.length(); i++){
    
        if(IsNumber(str[i]))
            tmp += str[i];
        
        if(str[i] == ','){
            if(interval){
                int start = parsedYears[k];
                for(int j = start; j <= stoi(tmp); j++,k++){
                    parsedYears[k] = j;
                }
                tmp = "";
                interval = false;
            }else{
                if(tmp != ""){
                    parsedYears[k] = stoi(tmp);
                    k++;
                    tmp = "";
                }
            }
        }
        
        if(str[i] == '-'){
            if(tmp != ""){
                interval = true;
                parsedYears[k] = stoi(tmp);
                tmp = "";
            }
        }
        
        if(i == str.length()-1){
            
            if(interval){
                int start = parsedYears[k];
                for(int j = start; j <= stoi(tmp); j++,k++){
                    parsedYears[k] = j;
                }
                tmp = "";
                interval = false;
            }else{
                if(tmp != ""){
                    parsedYears[k] = stoi(tmp);
                    k++;
                    tmp = "";
                }
            }
            
        }
        
    }
    
}




void EasterDate(int year, int& day, int& month){
    month = 3;
    int G = year % 19 + 1;
    int C = year / 100 + 1;
    int X = ( 3 * C ) / 4 - 12;
    int Y = ( 8 * C + 5 ) / 25 - 5;
    int Z = ( 5 * year ) / 4 - X - 10;
    int E = ( 11 * G + 20 + Y - X ) % 30;
    
    if (E == 24){
        E++;
    }
    
    if ((E == 25) && (G > 11)){
        E++;
    }
    
    int N = 44 - E;
    
    if (N < 21){
        N = N + 30;
    }
    
    int P = ( N + 7 ) - ( ( Z + N ) % 7 );
    
    if ( P > 31 ){
        P = P - 31;
        month = 4;
    }
    day = P;
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
        if(yearsArray[i] <= 1582 || yearsArray[i] >= 2200){
            delete[] yearsArray;
            yearsArray = NULL;
            return EASTER_INVALID_YEARS;
        }
    }

    // OTEVRENI SOUBORU A KONTROLA IO
    ofstream file(outFileName);
    if(file.fail()){
        delete[] yearsArray;
        yearsArray = NULL;
        return EASTER_IO_ERROR;
    }
    
    // VKLADANI DO SOUBORU
    
    file << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">" << endl << "<html>" << endl << "<head>" << endl << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" << endl << "<title>C++</title>" << endl << "</head>" << endl << "<body>" << endl << "<table width=\"300\">" << endl;
    
    file << "<tr><th width=\"99\">den</th><th width=\"99\">mesic</th><th width=\"99\">rok</th></tr>" << endl;
    
    for(int i = 0; i < numOfYears; i++){
        int day, month;
        EasterDate(yearsArray[i], day, month);
        string mothCZ;
        if(month == 3)
            mothCZ = "brezen";
        else
            mothCZ = "duben";
        file << "<tr><td>" << day << "</td><td>" << mothCZ << "</td><td>" << yearsArray[i] << "</td></tr>" << endl;
    }
    
    file << "</table>" << endl << "</body>" << endl << "</html>" << endl;
    
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
    
     cout << easterReport ("2012,2013,2015- 2020","/Users/lukstankovic/ahoj.html") << endl;
/*
    string years = "1999, 2010, 2015-2018, 2018,,, , ,, ,, ,,,    as          ";
    int numOfYears = CountYears(years);
    int* yearsArray = new int[100];
    
    // NAPLNENI ROKAMA
    cout << CountYears(years);
    ParseYears(years, yearsArray);*/
    /*
    cout << "pocet: " << numOfYears << endl;
    
    for(int i = 0; i < numOfYears;i++)
        cout << yearsArray[i] << ". "<< endl;
*/
    
    return 0;
    
}
#endif /* __PROGTEST__ */
