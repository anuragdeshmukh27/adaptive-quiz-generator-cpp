#ifndef QUESTIONBANK_H
#define QUESTIONBANK_H

#include "Question.h"
#include "MultipleChoiceQuestion.h"
#include "TrueFalseQuestion.h"
#include "FillInTheBlankQuestion.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set> // <-- NEWLY ADDED

class QuestionBank {
private:
    std::vector<Question*> allQuestions;
    std::set<std::string> allTopics; // <-- NEW: To store unique topic names

    // Private helper functions for parsing
    void parseMCQuestion(std::ifstream& file, const std::string& topic, int difficulty, const std::string& text);
    void parseTFQuestion(std::ifstream& file, const std::string& topic, int difficulty, const std::string& text);
    void parseFIBQuestion(std::ifstream& file, const std::string& topic, int difficulty, const std::string& text); 

public:
    QuestionBank();
    ~QuestionBank();
    void loadFromFile(const std::string& filename);
    Question* getQuestion(const std::string& topic, int difficulty, const std::vector<Question*>& usedQuestions);
    
    // --- NEW FUNCTION ---
    /**
     * @brief Gets a random topic from all loaded topics.
     * @return A string with a random topic name.
     */
    std::string getRandomTopic() const;
};

#endif // QUESTIONBANK_H