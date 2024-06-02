//mainServer.cpp

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void writee(const string &filePath, const string &ct) {    
    ofstream outFile(filePath);

    if (!outFile) {
        cerr << "Unable to open file: " << filePath << endl;
        return;
    }
    outFile << ct;
    outFile.close();

    cout << "Successfully wrote to the file: " << filePath << endl;
}

void server() {
    string cmd = "nc -lk -u 127.0.0.1 3030 >> received_messages.txt";
    system(cmd.c_str());
}

void split(const string &input, string &s1, string &s2, string &s3) {
    size_t cp = input.find(':');
    size_t dp = input.find("--");
    if (cp != string::npos && dp != string::npos) {
        s1 = input.substr(0, cp);
        s2 = input.substr(cp + 2, dp - (cp + 2));
        s3 = input.substr(dp + 2);
    } else {
        cerr << "Invalid input format" << endl;
    }
}


string chumma(string inpts){
    string gtx;
    for(char cc:inpts){
        int ddm=cc-'0';
        char cvvc=char(ddm);
        // ddm=int(cvvc);
        // cvvc=ddm+'0';
        gtx+=cvvc;
    }
    return gtx;
}


void helper() {
    while (true) {
        ifstream infile("received_messages.txt");
        string line;
        bool nm = false;

        while (getline(infile, line)) {
            nm = true;
            string s1, s2, s3, s4;
            split(line, s1, s2, s3);
            s4=s2;
            s4=chumma(s4);
            cout << "sender : " << s1 << endl;
            //cout << "message : " << s2 << endl;
            cout << "message : " << s4 << endl;
            cout << "receiver : " << s3 << endl << endl;
            int a = s1.back() - '0', b = s3.back() - '0';
            // s2 = support(a, b, s2);
            string ct = s1 + " : " + s2;
            string locPut = "../" + s3 + "/messages.txt";
            writee(locPut, ct);
        }
        infile.close();
        if (nm) {
            ofstream outfile("received_messages.txt", ios::trunc);
            outfile.close();
        }

        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    cout << "Running server..." << endl;
    thread t(server);
    helper();
    t.join();

    return 0;
}
