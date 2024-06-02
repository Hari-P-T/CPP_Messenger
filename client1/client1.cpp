#include <bits/stdc++.h>
#include <sys/stat.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <cstdlib>

using namespace std;

string myId;
string filePath;
atomic<bool> stopWatching(false);


int closestLargerPrime(int num);
int findPublicKeyExponent(int phi);
long long modExp(long long base, long long exp, long long mod);
long long encrypt(int a, int b, char msg_char);
string encryptMessage(int a, int b, const string &s);
long long decrypt(int a, int b, long long encrypted);
string decryptMessage(int a, int b, const string &s);
void client(const string &msg_ct);
void message(const string &myId);
time_t getTime(const string &filePath);
void split(const string &input, string &s1, string &s2, string &s3);
int gcd(int a, int b);
int modInverse(int a, int m);
void printFile(const string &filePath);
void watchFile(const string &filePath);

void client(const string &msg_ct) {
    string cmd = "echo \"" + msg_ct + "\" | nc -w 1 -u 127.0.0.1 3030";
    if (system(cmd.c_str()) == 0) {
        cout << "\nMessage sent successfully\n\n";
    } else {
        cerr << "\nFailed to send message\n\n";
    }
}

long long encrypt(int a, int b, char msg_char) {
    int msg = msg_char - 'a';
    int p = closestLargerPrime(a);
    int q = closestLargerPrime(b);

    long long n = static_cast<long long>(p) * static_cast<long long>(q);

    int phi = (p - 1) * (q - 1);

    int e = findPublicKeyExponent(phi);

    long long encrypted = modExp(msg, e, n);
    return encrypted;
}

string encryptMessage(int a, int b, const string &s) {
    stringstream ss;
    for (char c : s) {
        long long enc = encrypt(a, b, c);
        ss << enc << " ";
    }
    string result = ss.str();
    result.pop_back();
    return result;
}

void message(const string &myId) {
    while (true) {
        string msg_ct, loc_id;
        char ck;
        cout << "Do you wish to send a message? (y/n): ";
        cin >> ck;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        if (ck != 'y' && ck != 'Y') {
            break;
        }
        while (ck == 'y' || ck == 'Y') {
            cout << "\nEnter Receiver id: ";
            getline(cin, loc_id);
            cout << "\nEnter Message: ";
            getline(cin, msg_ct);
            msg_ct = encryptMessage(myId.back() - '0', loc_id.back() - '0', msg_ct);
            msg_ct = myId + ": " + msg_ct + " --" + loc_id;
            client(msg_ct);
            this_thread::sleep_for(chrono::seconds(3));
            cout << "\nDo you wish to continue? (y/n): ";
            cin >> ck;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

time_t getTime(const string &filePath) {
    struct stat fileInfo;
    if (stat(filePath.c_str(), &fileInfo) != 0) {
        cerr << "Unable to get file properties for " << filePath << endl;
        return 0;
    }
    return fileInfo.st_mtime;
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

int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

int modInverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        q = a / m;
        t = m;

        m = a % m, a = t;
        t = x0;

        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;

    return x1;
}

int findPublicKeyExponent(int phi) {
    int e = 2;
    while (e < phi) {
        if (gcd(e, phi) == 1)
            return e;
        else
            e++;
    }
    return -1;
}

long long modExp(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

bool isPrime(int num) {
    if (num <= 1) {
        return false;
    }
    if (num == 2) {
        return true;
    }
    if (num % 2 == 0) {
        return false;
    }
    for (int i = 3; i <= sqrt(num); i += 2) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

int closestLargerPrime(int num) {
    int candidate = num + 1;
    while (true) {
        if (isPrime(candidate)) {
            return candidate;
        }
        candidate++;
    }
}

long long decrypt(int a, int b, long long encrypted) {
    int p = closestLargerPrime(a);
    int q = closestLargerPrime(b);

    long long n = static_cast<long long>(p) * static_cast<long long>(q);

    int phi = (p - 1) * (q - 1);

    int e = findPublicKeyExponent(phi);

    int d = modInverse(e, phi);

    long long decrypted = modExp(encrypted, d, n);

    return decrypted;
}

string decryptMessage(int a, int b, const string &s) {
    stringstream ss(s);
    string token;
    string result;

    while (ss >> token) {
        long long encrypted_char = stoll(token);
        long long decrypted_char = decrypt(a, b, encrypted_char);
        result += static_cast<char>(decrypted_char + 'a');
    }

    return result;
}

void printFile(const string &filePath) {
    ifstream file(filePath);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            line += " --" + myId;
            string s1, s2, s3;
            split(line, s1, s2, s3);
            int a = s1.back() - '0', b = s3.back() - '0';
            s2 = decryptMessage(a, b, s2);
            cout << s1 << " : " << s2 << endl;
        }
        file.close();
    } else {
        cerr << "Unable to open file " << filePath << endl;
    }
}

void watchFile(const string &filePath) {
    time_t l_mod = getTime(filePath);

    while (!stopWatching) {
        this_thread::sleep_for(chrono::seconds(1));
        time_t c_mod = getTime(filePath);

        if (c_mod != l_mod) {
            cout << "\n\n---------------Message Alert !!!---------------\n\n";
            this_thread::sleep_for(chrono::seconds(2));
            printFile(filePath);
            cout << "\n------------------------------\n";
            l_mod = c_mod;
            message(myId);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <client_id> <file_path>" << endl;
        return 1;
    }

    myId = argv[1];
    filePath = argv[2];

    thread t1(watchFile, filePath);
    message(myId);

    stopWatching = true;
    if (t1.joinable()) {
        t1.join();
    }

    return 0;
}

