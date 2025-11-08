#ifndef USERSTATS_H
#define USERSTATS_H

#include <string>
#include <map>
#include <vector>
#include "json.hpp"

// Forward declaration
class Question; 

class UserStats {
private:
    std::string username;
    std::map<std::string, int> topicScores;
    std::map<std::string, int> topicAttempts;
    int currentDifficulty;

    Question* lastQuestionAsked;
    std::vector<Question*> usedQuestionsThisSession;

public:
    UserStats(const std::string& username);

    void updateStats(const std::string& topic, bool correct);
    std::string getWeakestTopic() const;
    void adjustDifficulty(bool correct);
    int getNextDifficulty() const;
    nlohmann::json getStatsJson() const; 

    // --- NEW FUNCTION ---
    /**
     * @brief Checks if there's enough data to make an adaptive choice.
     * @param minAttempts The minimum number of attempts a topic needs to be considered.
     * @return true if any topic has at least minAttempts, false otherwise.
     */
    bool hasSufficientData(int minAttempts = 3) const;

    void resetSession();
    void setLastQuestion(Question* q);
    Question* getLastQuestion() const;
    std::vector<Question*>& getUsedQuestions();
};

#endif // USERSTATS_H