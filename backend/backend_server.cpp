#include "crow_all.h"
#include "json.hpp"
#include "QuestionBank.h"
#include "UserStats.h"
#include <iostream>
#include <string>

// Use the nlohmann/json library
using json = nlohmann::json;

// --- Global Objects ---
QuestionBank questionBank;
UserStats user("QuizUser");

void setupCORS(crow::App<crow::CORSHandler>& app) {
    app.get_middleware<crow::CORSHandler>()
        .global()
        .methods("POST"_method, "GET"_method)
        .headers("Content-Type", "Authorization");
}


int main() {
    try {
        questionBank.loadFromFile("questions.txt");
    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR loading questions.txt: " << e.what() << std::endl;
        return 1;
    }
    
    srand(static_cast<unsigned int>(time(0)));

    crow::App<crow::CORSHandler> app;
    setupCORS(app);

    // --- API Endpoints (Our Server's URLs) ---

    CROW_ROUTE(app, "/start_quiz")
    ([](){
        std::cout << "SERVER LOG: /start_quiz called. Resetting session." << std::endl;
        user.resetSession();
        
        json response;
        response["message"] = "New quiz session started. User stats reset.";
        return crow::response(200, response.dump());
    });


    /**
     * @brief API: /get_question
     * Gets the next adaptive question for the user.
     * --- THIS IS THE MODIFIED ROUTE ---
     */
    CROW_ROUTE(app, "/get_question")
    ([](){
        std::string topic;
        
        // --- NEW LOGIC ---
        // Check if we have enough data (e.g., 3+ attempts in one topic) to be adaptive.
        // If not, we stay in "Random Mode".
        if (user.hasSufficientData(3)) { 
            topic = user.getWeakestTopic();
            std::cout << "SERVER LOG: [ADAPTIVE MODE] Targeting weakest topic: " << topic << std::endl;
        } else {
            topic = questionBank.getRandomTopic();
            std::cout << "SERVER LOG: [RANDOM MODE] Picking random topic: " << topic << std::endl;
        }
        // --- END OF NEW LOGIC ---

        int difficulty = user.getNextDifficulty();
        std::vector<Question*>& usedQuestions = user.getUsedQuestions();
        Question* q = questionBank.getQuestion(topic, difficulty, usedQuestions);

        if (q == nullptr) {
            json errorResponse;
            errorResponse["error"] = "No more questions available!";
            return crow::response(404, errorResponse.dump());
        }

        user.setLastQuestion(q);

        json q_json;
        q_json["questionText"] = q->getQuestionText();
        q_json["topic"] = q->getTopic();
        q_json["difficulty"] = q->getDifficulty();
        q_json["type"] = q->getQuestionType(); 
        q_json["options"] = q->getOptions();   
        
        std::cout << "SERVER LOG: Sending: " << q->getQuestionText() << std::endl;

        return crow::response(200, q_json.dump());
    });


    CROW_ROUTE(app, "/submit_answer").methods("POST"_method)
    ([](const crow::request& req){
        json requestBody = json::parse(req.body);
        if (!requestBody.contains("answer")) {
            return crow::response(400, "{\"error\":\"Missing 'answer' in request body\"}");
        }
        std::string userAnswer = requestBody["answer"];

        Question* lastQ = user.getLastQuestion();
        if (lastQ == nullptr) {
            return crow::response(400, "{\"error\":\"No question has been asked yet. Call /get_question first.\"}");
        }

        bool isCorrect = lastQ->checkAnswer(userAnswer);

        user.updateStats(lastQ->getTopic(), isCorrect);
        user.adjustDifficulty(isCorrect); 

        json responseBody;
        responseBody["isCorrect"] = isCorrect;
        responseBody["correctAnswerString"] = lastQ->getCorrectAnswerString();

        std::cout << "SERVER LOG: /submit_answer called. User answered '" << userAnswer << "'. Correct: " << (isCorrect ? "yes" : "no") << std::endl;

        return crow::response(200, responseBody.dump());
    });


    CROW_ROUTE(app, "/get_stats")
    ([](){
        std::cout << "SERVER LOG: /get_stats requested." << std::endl;
        json stats = user.getStatsJson();
        return crow::response(200, stats.dump());
    });


    // --- Run the server ---
    std::cout << "Crow server is running on port 18080..." << std::endl;
    app.port(18080)
       .multithreaded()
       .run();

    return 0;
}