#ifndef FILL_IN_THE_BLANK_QUESTION_H
#define FILL_IN_THE_BLANK_QUESTION_H

#include "Question.h"

/**
 * @class FillInTheBlankQuestion
 * @brief A concrete implementation of Question for fill-in-the-blank formats.
 */
class FillInTheBlankQuestion : public Question {
private:
    std::string correctAnswer; // The exact word or phrase

public:
    FillInTheBlankQuestion(const std::string& text, const std::string& top, int diff, const std::string& answer);

    // Overridden pure virtual functions
    void display() const override;
    bool checkAnswer(const std::string& userAnswer) const override;
    std::string getCorrectAnswerString() const override;
    
    std::string getQuestionType() const override {
        return "FIB";
    }
};

#endif // FILL_IN_THE_BLANK_QUESTION_H