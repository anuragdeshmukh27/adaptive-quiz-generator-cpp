#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>
#include <iostream>

/**
 * @class Question
 * @brief An abstract base class for all question types in the quiz.
 *
 * This class defines the common interface (contract) that all concrete
 * question types (like MultipleChoice, TrueFalse) must implement.
 * It cannot be instantiated on its own.
 */
class Question {
protected:
    std::string questionText;
    std::string topic;
    int difficulty; // e.g., 1 (easy), 2 (medium), 3 (hard)

public:
    /**
     * @brief Constructor for the Question base class.
     * @param text The text of the question.
     * @param top The topic (e.g., "Pointers", "OOP", "Data Structures").
     * @param diff The difficulty level.
     */
    Question(const std::string& text, const std::string& top, int diff)
        : questionText(text), topic(top), difficulty(diff) {}

    /**
     * @brief Virtual destructor.
     * This is crucial for a polymorphic base class to ensure
     * derived class destructors are called correctly.
     */
    virtual ~Question() {}

    /**
     * @brief Displays the question to the console.
     * This is a pure virtual function, making Question an abstract class.
     * Derived classes MUST provide their own implementation.
     */
    virtual void display() const = 0;

    /**
     * @brief Checks the user's answer.
     * This is also a pure virtual function.
     * @param userAnswer The answer provided by the user via console input.
     * @return true if the answer is correct, false otherwise.
     */
    virtual bool checkAnswer(const std::string& userAnswer) const = 0;

    /**
     * @brief Gets the correct answer as a string for display.
     * NEW function required for the web server to send feedback.
     * This is a pure virtual function.
     * @return A string representing the correct answer (e.g., "A", "true").
     */
    virtual std::string getCorrectAnswerString() const = 0;

    // --- Getters ---

    std::string getTopic() const { return topic; }
    int getDifficulty() const { return difficulty; }
    std::string getQuestionText() const { return questionText; }
    
    // Virtual getter for options, needed by the server.
    // Returns an empty vector by default, overridden by MultipleChoiceQuestion.
    virtual std::vector<std::string> getOptions() const {
        return {}; // Return an empty vector
    }

    // Virtual getter for question type, needed by the server.
    virtual std::string getQuestionType() const = 0;
};

#endif // QUESTION_H