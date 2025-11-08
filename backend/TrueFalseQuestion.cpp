#include "TrueFalseQuestion.h"
#include "Utils.h" // For trim() and toLower()
#include <iostream>

// Constructor
TrueFalseQuestion::TrueFalseQuestion(const std::string& text, const std::string& topic, int difficulty, bool answer)
    : Question(text, topic, difficulty), correctAnswer(answer) {
    // Body is empty
}

void TrueFalseQuestion::display() const {
    std::cout << "Topic: " << this->topic << " | Difficulty: " << this->difficulty << std::endl;
    std::cout << this->questionText << std::endl;
    std::cout << "Your answer (true/false): ";
}

bool TrueFalseQuestion::checkAnswer(const std::string& userAnswer) const {
    std::string lowerUser = toLower(trim(userAnswer));
    
    // Check for "t" or "true"
    if (lowerUser == "t" || lowerUser == "true") {
        return this->correctAnswer == true;
    }
    
    // Check for "f" or "false"
    if (lowerUser == "f" || lowerUser == "false") {
        return this->correctAnswer == false;
    }
    
    return false; // Not a valid answer
}

// New function implementation
std::string TrueFalseQuestion::getCorrectAnswerString() const {
    return this->correctAnswer ? "true" : "false";
}