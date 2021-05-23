#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[]){

    vector<string> GLOBAL;
    vector<string> LOCAL;
    vector<string> PERFORMANCE;
    int count = 0;

    ifstream input(argv[1]);
    if(!input)
    {
        cout << "File does not exist!";
        return 0;
    }

    string token;

    while(getline(input, token))
    {
        istringstream record(token);
        string tmp;

        getline(record,tmp,',');
        GLOBAL.push_back(tmp);
        //cout << GLOBAL[count] << endl;

        getline(record,tmp,',');
        LOCAL.push_back(tmp);
        //cout << LOCAL[count] << endl;

        getline(record,tmp,',');
        PERFORMANCE.push_back(tmp);
        //cout << PERFORMANCE[count] << endl;

        count++;
    }

    ofstream output;
    output.open("result-finish.csv",ios::app);
    output << "" << ',' << GLOBAL[0] << ',' << GLOBAL[7] << ',' << GLOBAL[14] << ',' << GLOBAL[21] 
                << ',' << GLOBAL[28] << ',' << GLOBAL[35] << ',' << GLOBAL[42] << ',' << GLOBAL[49]
                << endl;
    for(int i = 0; i < 7 ;i++)
    {
        output << LOCAL[i] << ',' << PERFORMANCE[i+0] << ',' << PERFORMANCE[i+7] << ',' << PERFORMANCE[i+14] << ',' << PERFORMANCE[i+21] 
                            << ',' << PERFORMANCE[i+28] << ',' << PERFORMANCE[i+35] << ',' << PERFORMANCE[i+42] << ',' << PERFORMANCE[i+49]
                            << endl;
    }
    output.close();
    input.close();
    return 0;
}