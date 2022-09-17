//
// David Mendoza, UIC Computer Science, Fall 2021
// Project 2 - Personality Tests
// Creative component: Athlete Personality Test
//

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"

using namespace std;

//
// ratingAns:
//
// Displays 1-5, strongly disagree to strongly agree
//
void ratingAns() {
  cout << "1. Strongly disagree" << endl;
  cout << "2. Disagree" << endl;
  cout << "3. Neutral" << endl;
  cout << "4. Agree" << endl;
  cout << "5. Strongly agree" << endl;
  cout << endl;
  cout << "Enter your answer here (1-5): " << endl;
}

//
// questionsTxt:
//
// Opens questions.txt,
// Organized chars and factors
// Put in map of factors
//
void questionsTxt(set<Question>& unaskedQuestions) {
    ifstream infile("questions.txt");
    string line, question, factorChar;
    int factorInt;
    // line = read the line, question is statement before .
    // f is for factor and number
    map<char, int> fact_map;
    getline(infile, line);
    while (!infile.eof()) {
        stringstream ss(line);
        getline(ss, question, '.');  // get read the whole statement
        Question q;
        q.questionText = question;
        while (!ss.eof()) {
            ss >> factorChar;  // read the character and factor (E: -1...)
            char f = factorChar[0];  // get char of OCEAN character
            factorInt = stoi(factorChar.substr(2, factorChar.size()));
            fact_map[f] = factorInt;  // store factor in Question struct map
        }
        q.factors = fact_map;  // set factors = to map created
        unaskedQuestions.insert(q);  // put in unaskedQuestions
        fact_map.clear();
        getline(infile, line);  // read line repeat
    }
}

//
// readPplData:
//
// reads .people files, and each score for O, C, E, A, N
//
void readPplData(string& filename, set<Person>& people) {
    ifstream infile(filename);
    string line, name, factorChar;
    int factorInt;
    map <char, int> temp_scores;  // where scores for each person is scored
    getline(infile, line);
    while (!infile.eof()) {
        stringstream ss(line);
        getline(ss, name, '.');
        Person p;
        p.name = name;
        while (!ss.eof()) {
            ss >> factorChar;  // read factor and their int score
            char f = factorChar[0];  // get char of OCEAN character
            factorInt = stoi(factorChar.substr(2, factorChar.size()));
            temp_scores[f] = factorInt;  // store factor in Question struct map
        }
        p.scores = temp_scores;  // set scores = to current scores right now
        people.insert(p);  // insert in people's scores data
        temp_scores.clear();  // clear for new alloc
        getline(infile, line);  // read line repeat
    }
}

//
// peopleFilesCommand:
//
// Displays options for which personality test to run
// When picked, opens and reads the data for people in that file and set.
//
void peopleFilesCommand(set<Person>& people, int& file) {
    string filename;
        if (file == 1) {
            filename = "BabyAnimals.people";
        } else if (file == 2) {
              filename = "Brooklyn99.people";
        } else if (file == 3) {
              filename = "Disney.people";
        } else if (file == 4) {
              filename = "Hogwarts.people";
        } else if (file == 5) {
              filename = "MyersBriggs.people";
        } else if (file == 6) {
              filename = "SesameStreet.people";
        } else if (file == 7) {
              filename = "StarWars.people";
        } else if (file == 8) {
              filename = "Vegetables.people";
        } else if (file == 9) {
              filename = "mine.people";
        }
    readPplData(filename, people);
}

//
// PersonalityTest:
//
// Runs the personality test. Asks the user a series
// of questions, and compares them to a list of people in
// .people files
//
void PersonalityTest(set<Person>& people, map <char, int>& scores) {
    int file;
    cout << "1. BabyAnimals" << endl << "2. Brooklyn99" << endl <<
    "3. Disney" << endl << "4. Hogwarts" << endl <<
    "5. MyersBriggs" << endl << "6. SesameStreet" << endl <<
    "7. StarWars" << endl << "8. Vegetables" << endl <<
    "9. mine" << endl << "0. To end program." << endl << endl;
    cout << "Choose test number (1-9, or 0 to end): ";
    cin >> file;
    while (file != 0) {  // while loop of commands like
                         // project 1
        peopleFilesCommand(people, file);
        Person p = mostSimilarTo(scores, people);
        cout << "You got " << p.name << "!" << endl;
        people.clear();  // clear for next person to analyze
        cout << "1. BabyAnimals" << endl << "2. Brooklyn99" << endl <<
        "3. Disney" << endl << "4. Hogwarts" << endl <<
        "5. MyersBriggs" << endl << "6. SesameStreet" << endl <<
        "7. StarWars" << endl << "8. Vegetables" << endl <<
        "9. mine" << endl << "0. To end program." << endl << endl;
        cout << "Choose test number (1-9, or 0 to end): ";
        cin >> file;
    }
    cout << "Goodbye!" << endl;
}

// User Interface in main

int main() {
    map<Question, int> answers;
    set<Question> unaskedQuestions;
    set<Person> people;
    string numQs, test;
    cout << "Welcome to the Personality Quiz!" << endl << endl;
    cout << "Choose number of questions: " << endl;
    getline(cin, numQs);
    int numQ = stoi(numQs);
    questionsTxt(unaskedQuestions);
    for (int i = 0; i < numQ; i++) {
        Question q;
        q = randomQuestionFrom(unaskedQuestions);
        cout << "How much do you agree with this statement?" << endl;
        cout << '"' << q.questionText << '.' << '"' << endl;
        cout << endl;
        ratingAns();
        getline(cin, test);
        int t = stoi(test);
        answers[q] = t;
    }
    map<char, int> scores = scoresFrom(answers);
    PersonalityTest(people, scores);
    return 0;
}
