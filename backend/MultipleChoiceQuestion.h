#ifndef MULTIPLE_CHOICE_QUESTION_H
#define MULTIPLE_CHOICE_QUESTION_H

#include "Question.h"
#include <vector>

/**
 * @class MultipleChoiceQuestion
 * @brief A concrete implementation of Question for multiple-choice formats.
 *
 * Inherits from the abstract Question class and implements its
 * pure virtual functions.
 */
class MultipleChoiceQuestion : public Question {
private:
    std::vector<std::string> options; // The answer choices (e.g., "A. ...", "B. ...")
    std::string correctAnswer;      // The correct letter (e.g., "A", "B", "C", "D")

public:
    /**
     * @brief Constructor for MultipleChoiceQuestion.
     * @param text The question text.
     * @param top The topic.
     * @param diff The difficulty level.
     * @param opts A vector of strings containing the answer options.
     * @param correctAns The string representing the correct option (e.g., "A").
     */
    MultipleChoiceQuestion(const std::string& text, const std::string& top, int diff,
                           const std::vector<std::string>& opts, const std::string& correctAns);

    /**
     * @brief Displays the question and all its options.
     * Overrides the pure virtual function from the Question base class.
     */
    void display() const override;

    /**
     * @brief Checks if the user's answer matches the correct option.
     * Overrides the pure virtual function from the Question base class.
     * This check is case-insensitive.
     * @param userAnswer The user's input (e.g., "a", "C").
     * @return true if the answer is correct, false otherwise.
     */
    bool checkAnswer(const std::string& userAnswer) const override;

    /**
     * @brief Gets the correct answer string (e.g., "A. Polymorphism").
     * Overrides the pure virtual function from the Question base class.
     */
    std::string getCorrectAnswerString() const override;

    /**
     * @brief Gets the options vector.
     * Overrides the base class virtual function.
     */
    std::vector<std::string> getOptions() const override {
        return options;
    }

    /**
     * @brief Gets the question type.
     * Overrides the base class virtual function.
     */
    std::string getQuestionType() const override {
        return "MC";
    }
};

#endif // MULTIPLE_CHOICE_QUESTION_H