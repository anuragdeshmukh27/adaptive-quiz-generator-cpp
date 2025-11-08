#ifndef TRUE_FALSE_QUESTION_H
#define TRUE_FALSE_QUESTION_H

#include "Question.h"

/**
 * @class TrueFalseQuestion
 * @brief A concrete implementation of Question for True/False formats.
 *
 * Inherits from the abstract Question class.
 */
class TrueFalseQuestion : public Question {
private:
    bool correctAnswer; // true or false

public:
    /**
     * @brief Constructor for TrueFalseQuestion.
     * @param text The question text.
     * @param top The topic.
     * @param diff The difficulty level.
     * @param correctAns The correct boolean answer.
     */
    TrueFalseQuestion(const std::string& text, const std::string& top, int diff, bool correctAns);

    /**
     * @brief Displays the question and the (True/False) prompt.
     * Overrides the pure virtual function from the Question base class.
     */
    void display() const override;

    /**
     * @brief Checks if the user's answer matches the correct boolean value.
     * Overrides the pure virtual function from the Question base class.
     * Accepts "true", "t", "false", or "f" (case-insensitive).
     * @param userAnswer The user's input.
     * @return true if the answer is correct, false otherwise.
     */
    bool checkAnswer(const std::string& userAnswer) const override;

    /**
     * @brief Gets the correct answer string ("true" or "false").
     * Overrides the pure virtual function from the Question base class.
     */
    std::string getCorrectAnswerString() const override;

    /**
     * @brief Gets the question type.
     * Overrides the base class virtual function.
     */
    std::string getQuestionType() const override {
        return "TF";
    }
};

#endif // TRUE_FALSE_QUESTION_H