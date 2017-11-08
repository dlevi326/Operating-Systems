#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <iterator>
#include <ctime>

using namespace std;

int main(){
  string filename1,filename2;
  cin>>filename1>>filename2;

  ifstream file;
    file.open(filename1);
    if (!file.is_open()){
      cout<<"ERROR"<<endl;
    } 



    ofstream output(filename2.c_str());
    if (!output) {
      cerr << "Error: could not open " << filename2 << endl;
      exit(1);
    }

    

    string word;
    while (file >> word){
      transform(word.begin(), word.end(), word.begin(), ::toupper);
      output<<word<<endl;

    }

    output.close();











  return 0;
}