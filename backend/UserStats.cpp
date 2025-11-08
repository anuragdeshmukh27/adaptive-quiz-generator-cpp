#include "UserStats.h"
#include "Question.h" 
#include "json.hpp" 
#include <iostream>
#include <iomanip> 
#include <limits>  
#include <algorithm> 
#include <cstdlib>   

UserStats::UserStats(const std::string& username)
    : username(username), currentDifficulty(1), lastQuestionAsked(nullptr) {
}

void UserStats::updateStats(const std::string& topic, bool correct) {
    topicAttempts[topic]++;
    if (correct) {
        topicScores[topic]++;
    }
}

// --- NEW FUNCTION IMPLEMENTATION ---
bool UserStats::hasSufficientData(int minAttempts) const {
    if (topicAttempts.empty()) {
        return false;
    }
    
    // Check if *any* topic has been attempted at least minAttempts times
    for (auto const& pair : topicAttempts) {
        if (pair.second >= minAttempts) {
            return true; // Found a topic with enough data
        }
    }
    
    return false; // No single topic has enough data yet
}
// --- END OF NEW FUNCTION ---


std::string UserStats::getWeakestTopic() const {
    // This function will now only be called if hasSufficientData() is true,
    // so we don't need the topicAttempts.empty() check, but we'll keep it
    // as a safety fallback.
    
    if (topicAttempts.empty()) {
        return "OOP Concepts"; // Safety fallback
    }

    std::string weakestTopic = "";
    double minScore = std::numeric_limits<double>::max();

    for (auto const& pair : topicAttempts) {
        const std::string& topic = pair.first; 
        int attempts = pair.second;           

        int correct = 0;
        if (topicScores.count(topic)) {
            correct = topicScores.at(topic);
        }

        double score = (attempts > 0) ? (static_cast<double>(correct) / attempts) : 0.0;

        if (score < minScore) {
            minScore = score;
            weakestTopic = topic;
        }
    }
    
    // If all scores are 0, weakestTopic might be the first one found.
    // If no topic is found, fallback.
    return weakestTopic.empty() ? "OOP Concepts" : weakestTopic;
}

void UserStats::adjustDifficulty(bool correct) {
    if (correct) {
        currentDifficulty++;
        if (currentDifficulty > 3) currentDifficulty = 3; // Clamp max
        std::cout << "[Difficulty increased to level " << currentDifficulty << "]" << std::endl;
    } else {
        currentDifficulty--;
        if (currentDifficulty < 1) currentDifficulty = 1; // Clamp min
        std::cout << "[Difficulty decreased to level " << currentDifficulty << "]" << std::endl;
    }
}

int UserStats::getNextDifficulty() const {
    return currentDifficulty;
}

nlohmann::json UserStats::getStatsJson() const {
    nlohmann::json statsJson;
    statsJson["username"] = this->username;
    statsJson["overallDifficulty"] = this->currentDifficulty;
    
    nlohmann::json topicsArray = nlohmann::json::array(); 

    if (topicAttempts.empty()) {
        statsJson["message"] = "No stats to display yet.";
    }

    for (auto const& pair : this->topicAttempts) {
        const std::string& topic = pair.first;
        int attempts = pair.second;

        int correct = 0;
        if (this->topicScores.count(topic)) {
            correct = this->topicScores.at(topic);
        }

        double scorePercent = (attempts > 0) ? (static_cast<double>(correct) / attempts) * 100.0 : 0.0;

        nlohmann::json topicObj;
        topicObj["topic"] = topic;
        topicObj["correct"] = correct;
        topicObj["attempted"] = attempts;
        topicObj["score"] = scorePercent;
        
        topicsArray.push_back(topicObj);
    }

    statsJson["topics"] = topicsArray; 
    return statsJson;
}

void UserStats::resetSession() {
    lastQuestionAsked = nullptr;
    usedQuestionsThisSession.clear();
}

void UserStats::setLastQuestion(Question* q) {
    lastQuestionAsked = q;
    if (q != nullptr) {
        usedQuestionsThisSession.push_back(q);
    }
}

Question* UserStats::getLastQuestion() const {
    return lastQuestionAsked;
}

std::vector<Question*>& UserStats::getUsedQuestions() {
    return usedQuestionsThisSession;
}