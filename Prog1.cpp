#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <queue>
#include <set>
#include <map>
using namespace std;

struct funcObj{
    bool operator()(const string &a, const string &b) {
        return a.length() < b.length();
    }
};

using Word = string; 
using Word_List = set<Word>; 
using AllWords = vector<Word>;
using Char_Freq_List = map<char,int>; 
using Freq_Word_List = multimap<int,Word>;
using Counter = tuple<int,int,int>; 
using Longest = priority_queue<Word,vector<Word>,funcObj>;

struct Diction { 
Counter stats; 
Word_List words; 
AllWords AllWords;
Char_Freq_List charFreq; 
Freq_Word_List freqWord; 
Longest longWord; 
};

string File = "WordFile.txt";
int WordCount;
int CharCount;
int LineCount;
ifstream S;
int Most;
int MaxLong = 0;
int Long;

Word getNextWord(Diction &d, string f) { // fills the set with each different word in the original file
// Not sure why but my file has a mystery blank character that I dont know how to get ride of so I adjusted the stats around it
Word w; 
char ch; 
bool inWord = false; 
S.get(ch);
if(isalpha(ch)){
Long = 0;
while( !S.eof() ) 
    { 
    if ( isalpha(ch) )     //  is ch in [A-Z, a-z] 
        {
        w.push_back(ch); 
        Long++;
        if (MaxLong < Long){
            MaxLong = Long;
        }
        inWord = true; 
    } 
    else if (inWord)
    return w; 
    S.get(ch); 
    }
}
return w; 
}

Word getAllWords(Diction &d, string f) { // fills a vector wiith every word in the file
Word w; 
char ch;
S.get(ch);
while(ch == ' ' || ch == '\n' || isdigit(ch) || ispunct(ch)){
    get<1>(d.stats)--;
    S.get(ch);
} 
while( !S.eof() ) 
    {
    if ( isalpha(ch) )     //  is ch in [A-Z, a-z] 
        {
        w.push_back(ch); 
    } 
    else{
        return w;
    }
    S.get(ch); 
    while(ch == '\n'){
    S.get(ch);
}
    }
return w; 
} 

void buildDictionary(Diction &d){ // prints out each different word in the Dictionary and calculates the stats of the file
    Word w;
    char ch;
    S.open(File);
    while (S){
    d.words.insert(getNextWord(d, File));
    get<1>(d.stats)++;
    }
    S.close();
    S.open(File);
    while (S){
    auto i = d.AllWords.end();
    d.AllWords.insert(i,getAllWords(d, File));
    }
    S.close();
    S.open(File);
    while (S){
        ch = S.get();
        if (ch == '\n'){
            get<2>(d.stats)++;
        }
        get<0>(d.stats)++;
    }
    get<0>(d.stats)--;
    get<1>(d.stats)--;
    get<2>(d.stats)++;
    cout << "Words in dictionary - ";
    for (auto i = d.words.begin(); i != d.words.end(); i++){
        cout << *i << ",";
    }
    cout << "\b "<<"\n \n";
}

void showstats(Diction &d){ // prints the statistics of the file provided
tie (CharCount, WordCount, LineCount) = d.stats;
    cout << "Number of Characters  : " << CharCount << endl;
    cout << "Number of Words       : " << WordCount << endl;
    cout << "Number of Lines       : " << LineCount << "\n \n";
}

void CreateMap(Diction &d){ // prints out how many of each letter there is in the file
    cout << "/------------------\\ " << endl;
    cout << "| Letter Frequency | "<< endl;
    cout << "\\------------------/" << endl;
    Char_Freq_List m;
    string s;
    int T;
    bool P = false;
for (char ch = 'a' ; ch <= 'z' ; ch++){
    m[ch] = 0;
    m[char(toupper(ch))] = 0;
}
    for (auto i : d.AllWords){
        for (auto c: i){
            m[c]++;
        }
    }
    for (auto e : m){
        if (e.second > 10){
            T = e.second - 10;
            e.second -= T;
            P = true;
        }
        cout << e.first << " | " << string(e.second, '*'); 
        if(P) cout << "(" << T << ")";
        P = false;
        cout << endl;
    }
}

multimap<int,Word> m;

map<int,std::string>::iterator findIt( string w) { // checks if the word provided is already in the multimap
// the copying the multimap code you prived was the only way i could get a for loop to work for multimap
 
        for (auto i = m.begin(); i != m.end(); i++ ) { // checks each element of the multimap
            if (w == i->second) return i; // checks if the given word already is in the multimap if it is it returns the iterator of its location
        }
        return m.end(); // if the word is not in the multimap it returns the end location of the multimap
}

void CreateMultiMap(Diction &d){ // prints out the how many of each word is in the file
    cout << "/------------\\ " << endl;
    cout << "| Dictionary | "<< endl;
    cout << "\\------------/" << endl;
    int V;
    int L;
    for (auto i: d.AllWords){
        auto F = findIt(i);
            if (F == m.end()) m.insert(std::pair{1, i});
            else {
                V = F->first;
                V++;
                m.insert(pair{V,i});
                m.erase(F);
                if (Most < V){
                    Most = V;
                }
            }
    }
    for (auto e: m){
        cout << e.second;
        L = e.second.length();
        for (int l = L; l < MaxLong + 2; l++){ // spacing
            cout << " ";
        }
        cout << e.first << endl;
    }
    cout << endl;
}

void CreatePriorityQueue(Diction &d){ // prints out each different word in the order from longest to shortest
    Longest K;
    int V = 1;
    int D = d.words.size() -2;
    for (auto i: d.words){
        K.push(i);
    }
    
    for (int G = D + MaxLong + 2; G >= 0; G--){ // prints the line that the vertical histogram is on
        cout << "-";
    }
    cout << endl;
    for (int H = 0; H <= MaxLong ; H++){ // spacing
        cout << " ";
    }
    for (auto h: d.words){ // prints the arrows that point to the different columns of stars
        cout << "^";
    }
    cout << endl;
    while (!K.empty()){
        cout << K.top();
        auto S = K.top();
        int L = S.length();
        K.pop();
        for (int l = L; l < MaxLong; l++){ // spacing
            cout << " ";
        }
        for (int I = 0; I< V;I++){ 
            cout << "-";
            if (I+1 == V){
                cout << "/";
                for (int i = D; i >= 0 ; i--){
                    cout << "|";
                }
            }
            
        }
        V += 1;
        D -= 1;
        cout << endl;
    }

}

void verticalStars(Diction &d){ // prints out the the stars for the vertical histogram 
    cout << "/-----------\\ " << endl;
    cout << "| Histogram | "<< endl;
    cout << "\\-----------/" << endl;
    Longest K;
    Freq_Word_List m;
    for (auto i: d.words){
        K.push(i);
    }
    for (int C = Most; C > 0; C--){ 
        for (int H = 0; H <= MaxLong ; H++){ // makes sure the stars are in the right spot
            cout << " ";
        }
        for (auto i = m.begin(); i != m.end(); i++){ // Honestly dont know why this isnt working 
            cout << "2";
            if( i->second == K.top()){
                if (C <= i->first){
                    cout << "*";
                }
                else{
                    cout << " ";
                }
            }
            K.pop();
        }
    cout << endl;
    }
}

void drawHistograms(Diction &d){
CreateMap(d);
CreateMultiMap(d);
verticalStars(d);
CreatePriorityQueue(d);
}
//#endif
int main(){
    Diction d{{0,0,0}};
    buildDictionary(d);
    showstats(d);
    drawHistograms(d);
    return 0;
}
    