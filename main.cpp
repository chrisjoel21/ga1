#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "ArgumentManager.h"
#include <fstream>
#include <stack>
#include <queue>
#include <cmath>
using namespace std;

int checkForBrac(char a){
    if(a == '{' || a == '[' || a == '('){
        return 1;
    }
    else if(a == '}' || a == ']' || a == ')'){
        return 2;
    }
    else{
        return 0;
    }
}

string replace_bracs(string s){
    int size = s.size();
    int temp1;
    int temp2;
    bool F = true;
    for(int i = 0; i < size; i++){
        int b = checkForBrac(s.at(i));
        if(b == 1){
            s.at(i) = '(';
            temp1 = i;
        }
        else if(b == 2){
            s.at(i) = ')';
            temp2 = i;
            string inBetween = s.substr(temp1+1, temp2 - temp1 - 1);
            for(int x = 0; x < inBetween.size(); x++){
                if(!isdigit(inBetween.at(x))){
                    F = false;
                    break;
                }
                else{
                    F = true;
                }
            }
            if(F){
                s.erase(temp2, 1);
                s.erase(temp1, 1);
                size -= 2;
                i--;
            }
        }
        else if(b == 0){
            ;//do nothing
        }
    }
    return s;
}

bool isValid(string expr) {
    stack<char> temp;
    bool operPopped;
    bool numPopped;
    int count = 0;
    for (int i = 0; i < expr.length(); i++) {
        if (isdigit(expr[i])) {
            temp.push(expr[i]);
        }
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/')
            temp.push(expr[i]);
        else if (expr[i] == '(' || expr[i] == '{' || expr[i] == '[') {
            temp.push(expr[i]);
            count++;
        }
        else if (expr[i] == ')' || expr[i] == '}' || expr[i] == ']') {
            operPopped = false;
            numPopped = false;
            while (true) {
                if (temp.empty())
                    return false;
                else if (temp.top() == '+' || temp.top() == '-' || temp.top() == '*' || temp.top() == '/') {\
                    temp.pop();
                    operPopped = true;
                }
                else if (isdigit(temp.top())) {
                    temp.pop();
                    numPopped = true;
                }
                else if ((temp.top() == '(' && expr[i] == ')') || (temp.top() == '{' && expr[i] == '}') || (temp.top() == '[' && expr[i] == ']')) {
                    count--;
                    temp.pop();
                    break;
                }
                else
                    return false;
            }
            if (!(operPopped || numPopped))
                return false;
        }
    }
    if (count > 0)
        return false;
    return true;
}

int priority(char let){
    if(let == '^'){
        return 3;
    }
    else if(let == '/' || let == '*'){
        return 2;
    }
    else if(let == '+' || let == '-'){
        return 1;
    }
    else{
        return -1;
    }
}

int performMath(char op, int num1, int num2){
    if(op == '+'){
        return num1 + num2;
    }
    else if(op == '-'){
        return num1 - num2;
    }
    else if(op == '*'){
        return num1 * num2;
    }
    else if(op == '/'){
        return num1 / num2;
    }
}

string infixToPostFix(string s){
    stack<char> t;
    string expression = "";
    int size = s.size();
    for(int i = 0; i < size; i++){
        if(isdigit(s.at(i))){
            expression += s.at(i);
            s.substr(1, s.size() - 1);
        }
        else if(s.at(i) == '('){
            t.push(s.at(i));
        }
        else if(s.at(i) == ')'){
            while(t.top() != '('){
                expression += t.top();
                t.pop();
            }
            t.pop();
        }
        else{
            if(t.size() == 0){
                t.push(s.at(i));
            }
            else{
                while(t.size() > 0 && priority(s.at(i)) <= priority(t.top())){
                    expression += t.top();
                    t.pop();
                }
                t.push(s.at(i));
            }
        }
    }
    while(t.size() > 0){
        expression += t.top();
        t.pop();
    }
return expression;
}


int doPostFix(string s){
    stack<string> t;
    int solution;

    for(int i = 0; i < s.length(); i++){
        if(isdigit(s[i])){
            string c = "";
            c += s[i];
            t.push(c);
        }
        else{
            string dig1 = t.top();
            t.pop();
            string dig2 = t.top();
            t.pop();

            t.push(to_string(performMath(s[i], stoi(dig2), stoi(dig1))));
        }
    }

    solution = stoi(t.top());
    return solution;
}

int func(string s){
    string edited;
    edited = replace_bracs(s);
    edited = infixToPostFix(edited);
    return doPostFix(edited);

}

int main(int argc, char *argv[]){
    ArgumentManager am(argc, argv);
    ifstream input(am.get("input"));
    //ifstream input("input1.txt");
    ofstream out(am.get("output"));
    //ofstream out("output1.txt");   
    string line;
    string post;
    string eval;
    priority_queue <pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> answers; 

    while(getline(input, line)){
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        if (line.length() == 0) {
            continue;
        }
        //cout << line << endl;
        else if (isValid(line)) {
            answers.push(make_pair(func(line), line));
            //cout << "valid" << endl;
        }
        
    }
    if(answers.empty()){
        out << "No Valid Expression";
    }
    while (!answers.empty()) {
        pair<int, string> output = answers.top();
        answers.pop();
        if(answers.empty()){
            out << output.second; 
        }
        else{
            out << output.second << endl;
        }
    }
    return 0;
}