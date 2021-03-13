#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;

void Read_Input_File(string file_location);
void Read_Input_Cin();

void Read_Input_File(string file_location){
    ifstream file(file_location);
    if(file.is_open()){
        unsigned short int N,M,T;
        string line;

        getline(file,line);
        N=stoi(line);
        for(int i=0;i<N;i++){
            getline(file,line);
            line=line.substr(1,line.length()-2);

            // (name, #cpu, #memory, #h_cost, #m_cost)
            string name;                // string length is at most 20 characters (number&alphabet)
            unsigned short cpu, memory; // range [1, 1024] positive number
            unsigned int h_cost; // range [1, 5*1e5] positive number
            unsigned short m_cost; // range [1, 5000]  positive number

            unsigned short start = 0;
            name = line.substr(0, (start=line.find(" "))-1);
            cpu = stoi(line.substr(start+1, start=line.find(" ",start+1)));
            memory = stoi(line.substr(start+1, start=line.find(" ",start+1)));
            h_cost = stoi(line.substr(start+1, start=line.find(" ",start+1)));
            m_cost = stoi(line.substr(start+1, start=line.find(" ",start+1)));
        }


        getline(file,line);
        M=stoi(line);
        for(int i=0;i<M;i++){
            getline(file,line);
            line=line.substr(1,line.length()-2);

            // (name, #cpu, #memory, #SD)
            string name;                // string length is at most 20 characters (number&alphabet&.)
            unsigned short cpu, memory; // range [1, 1024] positive number (can be contained in at least one server)
            bool iSingle;

            unsigned short start = 0;
            name = line.substr(0, (start=line.find(" "))-1);
            cpu = stoi(line.substr(start+1, start=line.find(" ",start+1)));
            memory = stoi(line.substr(start+1, start=line.find(" ",start+1)));
            iSingle = line.substr(start+1, start=line.find(" ",start+1)) == "0" ? true : false;
        }

        getline(file,line);
        T=stoi(line);
        for(int i=0;i<T;i++){
            getline(file,line);
            unsigned short R = stoi(line);     // the total requests over all days < 1e5
            for(int j=0;j<R;j++){
                getline(file,line);
                line=line.substr(1,line.length()-2);

                // (add, name, id) / (del, id)
                if(line[0]=='a'){
                    // add request
                    string name;
                    int id;       // ID is 32bit integer BUT NOT NECESSARILY POSITIVE

                    int start=line.find(" ")+1;
                    name = line.substr(5, line.find_last_of(", ")-6);
                    id = stoi(line.substr(line.find_last_of(", ")+1));                    
                }else{
                    // delete requst (the vm with this id is guarenteed to exist)
                    int id = stoi(line.substr(line.find_last_of(", ")+1));       // id is 32bit integer BUT NOT NECESSARILY POSITIVE
                }
            }
        }
    }else{
        cout<<file.is_open()<<endl;
        cerr << "Error: "<<strerror(errno)<<endl;
    }
}

void Read_Input_Cin(){

}