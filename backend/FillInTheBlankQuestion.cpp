#include "FillInTheBlankQuestion.h"
#include "Utils.h" // For trim() and toLower()
#include <iostream>

FillInTheBlankQuestion::FillInTheBlankQuestion(const std::string& text, const std::string& topic, int difficulty, const std::string& answer)
    : Question(text, topic, difficulty), correctAnswer(answer) {
    // Body is empty
}

// This is for the old console app, but good to implement
void FillInTheBlankQuestion::display() const {
    std::cout << "Topic: " << this->topic << " | Difficulty: " << this->difficulty << std::endl;
    std::cout << this->questionText << std::endl;
    std::cout << "Your answer: ";
}

bool FillInTheBlankQuestion::checkAnswer(const std::string& userAnswer) const {
    // A simple, case-insensitive check
    return toLower(trim(userAnswer)) == toLower(trim(this->correctAnswer));
}

std::string FillInTheBlankQuestion::getCorrectAnswerString() const {
    // Just return the answer string
    return this->correctAnswer;
}