#include "MultipleChoiceQuestion.h"
#include "Utils.h" // For trim() and toLower()
#include <iostream>

// Constructor - Uses initializer list to pass data to base Question class
MultipleChoiceQuestion::MultipleChoiceQuestion(const std::string& text, const std::string& topic, int difficulty,
                                               const std::vector<std::string>& options, const std::string& answer)
    : Question(text, topic, difficulty) {
    
    // Use 'this->' to solve the variable shadowing bug
    this->options = options;
    this->correctAnswer = answer;
}

void MultipleChoiceQuestion::display() const {
    // 1. Display the question text (from base class)
    std::cout << "Topic: " << this->topic << " | Difficulty: " << this->difficulty << std::endl;
    std::cout << this->questionText << "\n" << std::endl; // Print the question

    // Print options with letters
    char optionLetter = 'A';
    for (const std::string& option : this->options) {
        // This check is for the "A. Encapsulation" vs "Encapsulation" fix
        // We will just print the options as they appear in the file.
        std::cout << option << std::endl;
        optionLetter++; // Increment letter for next option
    }

    std::cout << "Your answer (A, B, C, etc.): ";
}

bool MultipleChoiceQuestion::checkAnswer(const std::string& userAnswer) const {
    std::string cleanUserAnswer = toLower(trim(userAnswer));
    
    // 1. Check if the answer is the correct letter (e.g., "a" == "a")
    if (cleanUserAnswer == toLower(this->correctAnswer)) {
        return true;
    }

    // 2. Check if the answer is the full text of the correct option
    // Find the correct option index (A=0, B=1, etc.)
    int correctIndex = toLower(this->correctAnswer)[0] - 'a'; // 'a' - 'a' = 0, 'b' - 'a' = 1

    if (correctIndex >= 0 && correctIndex < options.size()) {
        // Need to clean the option text, removing the "B. " prefix
        std::string optionTextOnly = options[correctIndex];
        size_t firstSpace = optionTextOnly.find(" ");
        if (firstSpace != std::string::npos && optionTextOnly.length() > 2 && optionTextOnly[1] == '.') {
             optionTextOnly = optionTextOnly.substr(firstSpace + 1);
        }
        
        std::string cleanOptionText = toLower(trim(optionTextOnly));
        
        if (cleanUserAnswer == cleanOptionText) {
            return true;
        }
    }
    
    return false; // Neither matched
}

// --- THIS IS THE FIXED FUNCTION ---
std::string MultipleChoiceQuestion::getCorrectAnswerString() const {
    // Find the correct option text
    // 'a' - 'a' = 0, 'b' - 'a' = 1, 'c' - 'a' = 2, 'd' - 'a' = 3
    int correctIndex = toLower(this->correctAnswer)[0] - 'a'; 

    if (correctIndex >= 0 && correctIndex < options.size()) {
        // The option string (e.g., "B. A pointer...") already has the prefix.
        // Just return the option string directly.
        return options[correctIndex];
    }
    
    // Fallback in case something is wrong
    return this->correctAnswer;
}