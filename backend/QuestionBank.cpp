#include "QuestionBank.h"
#include "MultipleChoiceQuestion.h"
#include "TrueFalseQuestion.h"
#include "FillInTheBlankQuestion.h" 
#include "Utils.h" 
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib> 
#include <ctime>   
#include <algorithm> 
#include <set>       // <-- NEWLY ADDED
#include <iterator>  // <-- NEWLY ADDED for std::advance

// Constructor
QuestionBank::QuestionBank() {
    srand(static_cast<unsigned int>(time(0)));
}

// Destructor
QuestionBank::~QuestionBank() {
    for (Question* q : allQuestions) {
        delete q;
    }
    allQuestions.clear();
}

Question* QuestionBank::getQuestion(const std::string& topic, int difficulty, const std::vector<Question*>& usedQuestions) {
    
    std::vector<Question*> candidates;
    for (Question* q : allQuestions) {
        bool criteriaMatch = (q->getTopic() == topic && q->getDifficulty() == difficulty);
        bool isUsed = (std::find(usedQuestions.begin(), usedQuestions.end(), q) != usedQuestions.end());

        if (criteriaMatch && !isUsed) {
            candidates.push_back(q);
        }
    }

    // Fallback 1: Same topic, any difficulty
    if (candidates.empty()) {
        for (Question* q : allQuestions) {
            bool topicMatch = (q->getTopic() == topic);
            bool isUsed = (std::find(usedQuestions.begin(), usedQuestions.end(), q) != usedQuestions.end());
            
            if (topicMatch && !isUsed) {
                candidates.push_back(q);
            }
        }
    }
    
    // Fallback 2: Any topic, any difficulty
    if (candidates.empty()) {
        for (Question* q : allQuestions) {
            bool isUsed = (std::find(usedQuestions.begin(), usedQuestions.end(), q) != usedQuestions.end());
            if (!isUsed) {
                candidates.push_back(q);
            }
        }
    }

    // Final Fallback: All questions used, just pick a random one
    if (candidates.empty()) {
        if (allQuestions.empty()) {
             std::cerr << "Error: No questions loaded into question bank at all." << std::endl;
             return nullptr;
        }
        candidates.push_back(allQuestions[rand() % allQuestions.size()]);
    }

    int randomIndex = rand() % candidates.size();
    return candidates[randomIndex];
}

// --- NEW FUNCTION IMPLEMENTATION ---
std::string QuestionBank::getRandomTopic() const {
    if (allTopics.empty()) {
        return "OOP Concepts"; // Safety fallback
    }
    
    // Get a random iterator from the set
    int randomIndex = rand() % allTopics.size();
    auto it = allTopics.begin();
    std::advance(it, randomIndex); // Move the iterator to the random position
    return *it; // Return the topic string
}
// --- END OF NEW FUNCTION ---


void QuestionBank::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open question file: " << filename << std::endl;
        return;
    }

    std::string line, type, topic, text;
    int difficulty;

    while (std::getline(file, line)) {
        type = trim(line);
        if (type.empty() || type[0] == '#') continue; 

        if (!std::getline(file, topic)) break;
        topic = trim(topic);

        // --- NEWLY ADDED ---
        // Add the topic to our set of unique topics
        if (!topic.empty()) {
            allTopics.insert(topic);
        }
        // --- END OF ADDITION ---

        if (!(file >> difficulty)) break;
        file.ignore(1000, '\n'); 

        if (!std::getline(file, text)) break;
        text = trim(text);

        if (type == "MC") {
            parseMCQuestion(file, topic, difficulty, text);
        } else if (type == "TF") {
            parseTFQuestion(file, topic, difficulty, text);
        } else if (type == "FIB") { 
            parseFIBQuestion(file, topic, difficulty, text);
        }
    }

    std::cout << "Loaded " << allQuestions.size() << " questions from " << filename << std::endl;
    file.close();
}

void QuestionBank::parseMCQuestion(std::ifstream& file, const std::string& topic, int difficulty, const std::string& text) {
    std::vector<std::string> options;
    std::string line;
    std::string answer;

    for (int i = 0; i < 4; ++i) {
        if (std::getline(file, line)) {
            options.push_back(trim(line));
        }
    }
    if (std::getline(file, answer)) {
        answer = trim(answer);
    }
    allQuestions.push_back(new MultipleChoiceQuestion(text, topic, difficulty, options, answer));
}

void QuestionBank::parseTFQuestion(std::ifstream& file, const std::string& topic, int difficulty, const std::string& text) {
    std::string answerStr;
    bool answer = false;

    if (std::getline(file, answerStr)) {
        answerStr = toLower(trim(answerStr));
        if (answerStr == "true") {
            answer = true;
        }
    }
    allQuestions.push_back(new TrueFalseQuestion(text, topic, difficulty, answer));
}

void QuestionBank::parseFIBQuestion(std::ifstream& file, const std::string& topic, int difficulty, const std::string& text) {
    std::string answer;
    if (std::getline(file, answer)) {
        answer = trim(answer);
    }
    allQuestions.push_back(new FillInTheBlankQuestion(text, topic, difficulty, answer));
}