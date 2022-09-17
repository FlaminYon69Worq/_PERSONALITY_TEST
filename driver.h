////////
// David Mendoza, UIC Computer Science, Fall 2021
// Project 2 - Personality Tests
// Creative component: Athlete Personality Test
////////

#include <cmath>
#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size()-1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

//
// randomQuestionFrom:
//
// Returns a random question from question.txt.
//
Question randomQuestionFrom(set<Question>& unaskedQuestions) {
    if (unaskedQuestions.empty()) {
        throw runtime_error("set: empty");
    }
    Question question = randomElement(unaskedQuestions);
    unaskedQuestions.erase(question);
    return question;
}

//
// scoresFrom:
//
// Calculates the OCEAN score of a person.
// Updates score by making AddOn for the score in each key // of the map.
//
map<char, int> scoresFrom(map<Question, int>& answers) {
    map <char, int> scores;
    int multiple, addOn;
    for (auto &i : answers) {
        if (i.second == 5) {  // assign multiple based on answer
            multiple = 2;
        } else if (i.second == 4) {
              multiple = 1;
        } else if (i.second == 3) {
              multiple = 0;
        } else if (i.second == 2) {
              multiple = -1;
        } else if (i.second == 1) {
              multiple = -2;
        }
        for (auto s : i.first.factors) {  // multiply
            addOn = multiple * s.second;
            // ^^ number to add on score
            scores[s.first] += addOn;
        }
    }
    return scores;
}

//
// normalize:
//
// Takes as input a set of raw scores, then returns a
// normalized version of the scores.
//
map<char, double> normalize(map<char, int>& scores) {
    double normal = 0;  // normal variable
    for (auto &pair : scores) {
        normal += pow(pair.second, 2);  // square everything
    }
    map<char, double> normalScores;  // allocate norms
    if (normal == 0) {
      throw invalid_argument("cannot normalize all zeroes");
    }
    double root = sqrt(normal);
    for (auto &pair : scores) {  // allocate scores
        double ans = pair.second / root;
        // ^^ divide by root
        normalScores.emplace(pair.first, ans);
        // ^^ store them in normal scores
    }
    return normalScores;
}

//
// cosinSimilarityOf:
//
// Takes as input two sets of normalized scores, then
// returns their cosine similarity using this formula:
// similarity = O1O2 + C1C2... + N1N2
//
double cosineSimilarityOf(const map<char, double>& lhs,
const map<char, double>& rhs) {
    double similarity = 0;
    for (auto l : lhs) {
        for (auto r : rhs) {
            if (l.first == r.first) {  // check if char exists
            // ^^ ex. if there is O for question, and for every other char
                similarity += l.second * r.second;
            }
        }
    }
    return similarity;
}

//
// mostSimilarTo:
//
// Returns the person who is most similar for the results
// of the questions based on normalized scores and cosine
// similarity.
//
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
    if (people.empty()) {
        throw runtime_error("set: empty");
    }
    // ^^ map of similar scores
    double max = lowest_double;  // current max
    map<char, double> normalScores = normalize(scores);
    Person similar;  // person we are returning
    for (auto p : people) {
        map<char, double>normalP = normalize(p.scores);
        // ^^ normalized scores of current person
        double sim = cosineSimilarityOf(normalScores,normalP);  // normalized of person vs. all
        if (sim >= max) {
          similar.name = p.name;  // set name and scores =
          // to person we are returning
          similar.scores = p.scores;
          max = sim;
        }
    }
    return similar;
}
