# C++ Full-Stack Adaptive Quiz Generator

This is a full-stack web application that serves an adaptive quiz to users. The **backend API and all quiz logic** are built in C++ (using the Crow web framework), and the **frontend user interface** is built in React.

This project was built to demonstrate core Object-Oriented Programming (OOP) principles in C++, including:
* **Abstraction:** A `Question` abstract base class defines a common interface.
* **Inheritance:** `MultipleChoiceQuestion`, `TrueFalseQuestion`, and `FillInTheBlankQuestion` all inherit from `Question`.
* **Polymorphism:** The `QuestionBank` and `backend_server` handle all questions as base `Question*` pointers, allowing new question types to be added without changing the core logic.
* **Encapsulation:** The `UserStats` class fully encapsulates the adaptive logic for tracking weak topics and adjusting difficulty.

## Tech Stack

* **Backend (C++)**
    * **Core Logic:** C++17
    * **Web Server:** [Crow (C++ micro-framework)](https://github.com/CrowCpp/Crow)
    * **JSON Handling:** [nlohmann/json](https://github.com/nlohmann/json)
    * **Libraries:** Asio (for networking)

* **Frontend (React)**
    * **Framework:** React
    * **Build Tool:** Vite
    * **Animations:** Framer Motion
    * **Icons:** Lucide-React

## Project Structure

adaptive-quiz-fullstack/
├── backend/
│   ├── asio/
│   ├── asio.hpp
│   ├── crow_all.h
│   ├── json.hpp
│   ├── backend_server.cpp (Main server file)
│   ├── Question.h
│   ├── UserStats.h
│   ├── UserStats.cpp
│   ├── ... (all other C++ source files) ...
│   ├── questions.txt
│   └── .vscode/
│       └── tasks.json
│
├── frontend/
│   ├── src/
│   │   ├── App.css
│   │   ├── App.jsx
│   │   ├── index.css
│   │   └── main.jsx
│   ├── .gitignore
│   ├── package.json
│   └── vite.config.js
│
├── .gitignore
└── README.md

## How to Run

You must run **both** the backend server and the frontend server at the same time in two separate terminals.

### 1. Run the Backend (C++ Server)

1.  **Navigate to the backend folder:**
    ```bash
    cd backend
    ```
2.  **Build the C++ code.** You will need the `g++` compiler. This command links all necessary libraries (on Windows):
    ```bash
    g++.exe -g -std=c++17 -I. backend_server.cpp MultipleChoiceQuestion.cpp TrueFalseQuestion.cpp FillInTheBlankQuestion.cpp QuestionBank.cpp UserStats.cpp -o quiz_server.exe -lmswsock -lws2_32
    ```
    *(Alternatively, if using VS Code, run the "Build Quiz Server" build task (`Ctrl+Shift+B`)).*

3.  **Run the server:**
    ```bash
    .\quiz_server.exe
    ```
4.  The terminal should show: `Crow server is running on port 18080...`
    **Leave this terminal running.**

### 2. Run the Frontend (React App)

1.  **Open a new, separate terminal.**
2.  **Navigate to the frontend folder:**
    ```bash
    cd frontend
    ```
3.  **Install dependencies** (only need to do this once):
    ```bash
    npm install
    ```
4.  **Run the app:**
    ```bash
    npm run dev
    ```
5.  The terminal will give you a local URL. Open it in your browser:
    **`http://localhost:5173/`**

You can now use the full application in your browser!