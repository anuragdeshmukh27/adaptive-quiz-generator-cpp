import { useState, useEffect } from 'react'; // Added useEffect
import { motion, AnimatePresence } from 'framer-motion';
import { 
  CheckCircle2, XCircle, BrainCircuit, 
  BookOpen, Code, Layers3, ChevronsRight, BarChart3,
  TrendingUp, TrendingDown, Minus
} from 'lucide-react';
import './App.css';

const API_URL = 'http://localhost:18080';

// Helper component for topic icons
const TopicIcon = ({ topic }) => {
  switch (topic) {
    case 'OOP Concepts':
      return <BrainCircuit size={16} />;
    case 'Pointers':
      return <Code size={16} />;
    case 'Data Structures':
      return <Layers3 size={16} />;
    case 'C++ Basics':
      return <BookOpen size={16} />;
    default:
      return <BookOpen size={16} />;
  }
};

// Helper component for difficulty
const DifficultyIcon = ({ level }) => {
  if (level === 1) return <TrendingDown size={16} title="Easy" />;
  if (level === 2) return <Minus size={16} title="Medium" />;
  if (level === 3) return <TrendingUp size={16} title="Hard" />;
  return null;
}

export default function App() {
  // 'settings', 'loading', 'active', 'results'
  const [quizState, setQuizState] = useState('settings');
  const [numQuestions, setNumQuestions] = useState(10);
  
  const [question, setQuestion] = useState(null);
  const [feedback, setFeedback] = useState('');
  const [isCorrect, setIsCorrect] = useState(null);
  const [score, setScore] = useState(0);
  const [currentQuestionIndex, setCurrentQuestionIndex] = useState(0);
  const [stats, setStats] = useState(null);

  const [selectedAnswer, setSelectedAnswer] = useState(null);
  const [correctAnswer, setCorrectAnswer] = useState(null);
  
  // --- NEW STATE FOR FIB ---
  const [fibAnswer, setFibAnswer] = useState('');
  // --- END NEW STATE ---

  // --- API Functions ---

  const handleStartQuiz = async () => {
    try {
      setQuizState('loading');
      setFeedback('Starting new quiz...');
      setStats(null);
      setScore(0);
      setCurrentQuestionIndex(0);
      setSelectedAnswer(null);
      setCorrectAnswer(null);
      setFibAnswer(''); // <-- NEW: Reset FIB input

      await fetch(`${API_URL}/start_quiz`);
      
      handleGetQuestion();
    } catch (error) {
      console.error('Error starting quiz:', error);
      setFeedback('Failed to connect to C++ server. Is it running?');
      setQuizState('settings'); 
    }
  };

  const loadNext = () => {
    // Stop if we've reached the set number of questions
    if (currentQuestionIndex >= numQuestions) {
      handleShowStats();
    } else {
      handleGetQuestion();
    }
  };

  const handleGetQuestion = async () => {
    try {
      setFeedback(''); 
      setIsCorrect(null);
      setQuestion(null); 
      setSelectedAnswer(null);
      setCorrectAnswer(null);
      setFibAnswer(''); // <-- NEW: Reset FIB input
      
      const response = await fetch(`${API_URL}/get_question`);
      
      if (!response.ok) {
        const errorData = await response.json();
        throw new Error(errorData.error || 'No more questions!');
      }
      
      const data = await response.json();
      setQuestion(data);
      setCurrentQuestionIndex(i => i + 1); // Increment index
      setQuizState('active'); 
    } catch (error) {
      console.error('Error getting question:', error);
      setFeedback(error.message);
      handleShowStats();
    }
  };

  const handleSubmitAnswer = async (answer) => {
    if (isCorrect !== null) return; 

    setSelectedAnswer(answer); 

    try {
      const response = await fetch(`${API_URL}/submit_answer`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ answer: answer }),
      });
      
      if (!response.ok) throw new Error('Server responded with an error.');
      
      const result = await response.json();
      
      setIsCorrect(result.isCorrect);
      setCorrectAnswer(result.correctAnswerString); 

      if (result.isCorrect) {
        setFeedback('Correct!');
        setScore(s => s + 1);
      } else {
        // For FIB, show the correct answer in the feedback
        if (question.type === 'FIB') {
          setFeedback(`Incorrect. The answer was: ${result.correctAnswerString}`);
        } else {
          setFeedback(`Incorrect.`);
        }
      }
      
      // --- AUTO-ADVANCE ---
      setTimeout(loadNext, 1500); 

    } catch (error) {
      console.error('Error submitting answer:', error);
      setFeedback('Failed to submit answer.');
    }
  };

  const handleShowStats = async () => {
    setQuizState('loading');
    setQuestion(null);
    try {
      const response = await fetch(`${API_URL}/get_stats`);
      const data = await response.json();
      setStats(data);
      setQuizState('results'); 
    } catch (error) {
      console.error('Error getting stats:', error);
      setFeedback('Failed to retrieve stats.');
    }
  };

  // --- Render Functions ---

  const renderQuestion = () => {
    if (!question) return <p>Loading question...</p>;

    const showFeedback = isCorrect !== null;

    if (question.type === "MC") {
      const correctLetter = correctAnswer ? correctAnswer.charAt(0) : '';

      return (
        <div className="options-container" data-type="MC">
          {question.options.map((option, index) => {
            const letter = String.fromCharCode(65 + index); // A, B, C, D
            
            let btnClass = "option-button";
            if (showFeedback) {
              if (letter === correctLetter) {
                btnClass += " correct"; 
              } else if (letter === selectedAnswer) {
                btnClass += " incorrect-selected"; 
              }
            }

            return (
              <button 
                key={letter}
                className={btnClass}
                onClick={() => handleSubmitAnswer(letter)}
                disabled={showFeedback}
              >
                <span>{letter}.</span> {option.substring(option.indexOf(' ') + 1)}
              </button>
            );
          })}
        </div>
      );
    }

    if (question.type === "TF") {
      const getBtnClass = (answer) => {
        let btnClass = "option-button";
        if (showFeedback) {
          if (answer === correctAnswer) {
            btnClass += " correct";
          } else if (answer === selectedAnswer) {
            btnClass += " incorrect-selected";
          }
        }
        return btnClass;
      };

      return (
        <div className="options-container" data-type="TF">
          <button 
            className={getBtnClass('true')}
            onClick={() => handleSubmitAnswer('true')}
            disabled={showFeedback}
          >
            True
          </button>
          <button 
            className={getBtnClass('false')}
            onClick={() => handleSubmitAnswer('false')}
            disabled={showFeedback}
          >
            False
          </button>
        </div>
      );
    }

    // --- NEW: RENDER LOGIC FOR FILL-IN-THE-BLANK ---
    if (question.type === "FIB") {
      const handleSubmitFib = (e) => {
        e.preventDefault(); // Stop page from reloading on form submit
        if (fibAnswer.trim() === '') return; // Don't submit empty
        handleSubmitAnswer(fibAnswer);
      };

      let inputClass = "fib-input";
      if (showFeedback && isCorrect === true) {
        inputClass += " correct";
      } else if (showFeedback && isCorrect === false) {
        inputClass += " incorrect-selected";
      }

      return (
        <form className="fib-container" onSubmit={handleSubmitFib}>
          <input
            type="text"
            className={inputClass}
            value={fibAnswer}
            onChange={(e) => setFibAnswer(e.target.value)}
            placeholder="Type your answer..."
            disabled={showFeedback}
            autoFocus
          />
          <button 
            type="submit" 
            className="primary-button fib-submit-button" 
            disabled={showFeedback || fibAnswer.trim() === ''}
          >
            Submit
          </button>
        </form>
      );
    }
    // --- END OF NEW LOGIC ---

    return null;
  };

  const renderStats = () => {
    if (!stats) return null;
    return (
      <div className="stats-container">
        <h3>Final Stats for {stats.username}</h3>
        <p style={{textAlign: 'center', marginBottom: '1.5rem', color: 'var(--text-muted)'}}>
          Overall Difficulty Level: {stats.overallDifficulty}
        </p>
        <table>
          <thead>
            <tr>
              <th>Topic</th>
              <th>Correct</th>
              <th>Attempted</th>
              <th>Score</th>
            </tr>
          </thead>
          <tbody>
            {stats.topics && stats.topics.length > 0 ? (
              stats.topics.map((topicStat) => (
                <tr key={topicStat.topic}>
                  <td>{topicStat.topic}</td>
                  <td>{topicStat.correct}</td>
                  <td>{topicStat.attempted}</td>
                  <td>{topicStat.score.toFixed(0)} %</td>
                </tr>
              ))
            ) : (
              <tr>
                <td colSpan="4" style={{textAlign: 'center', color: 'var(--text-muted)'}}>
                  {stats.message || "No topics attempted."}
                </td>
              </tr>
            )}
          </tbody>
        </table>
      </div>
    );
  };
  
  // --- Main App UI ---

  return (
    <div className="app-container">
      <motion.h1 layout>C++ <span>Adaptive Quiz</span></motion.h1>
      
      <AnimatePresence mode="wait">
        
        {/* --- SETTINGS SCREEN --- */}
        {quizState === 'settings' && (
          <motion.div
            key="settings"
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            exit={{ opacity: 0, y: -20 }}
            className="settings-container"
          >
            <h2>Quiz Settings</h2>
            <label className="settings-label">Number of Questions:</label>
            <div className="settings-options">
              {[5, 10, 20].map(num => (
                <button
                  key={num}
                  className={numQuestions === num ? 'selected' : ''}
                  onClick={() => setNumQuestions(num)}
                >
                  {num}
                </button>
              ))}
            </div>
            <button className="primary-button" style={{width: '100%'}} onClick={handleStartQuiz}>
              Start Quiz
            </button>
          </motion.div>
        )}

        {/* --- LOADING SCREEN --- */}
        {quizState === 'loading' && (
           <motion.div
            key="loading"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            className="quiz-card"
            style={{display: 'flex', alignItems: 'center', justifyContent: 'center', minHeight: '350px'}}
          >
            <p style={{fontSize: '1.2rem', color: 'var(--text-muted)'}}>{feedback || 'Loading...'}</p>
          </motion.div>
        )}

        {/* --- QUIZ ACTIVE SCREEN --- */}
        {quizState === 'active' && question && (
          <motion.div
            key={currentQuestionIndex} 
            initial={{ opacity: 0, x: -50 }}
            animate={{ opacity: 1, x: 0 }}
            exit={{ opacity: 0, x: 50 }}
            transition={{ duration: 0.3 }}
          >
            <div className="progress-bar-container">
              <motion.div 
                className="progress-bar-inner"
                initial={{ width: 0 }}
                animate={{ width: `${(currentQuestionIndex / numQuestions) * 100}%` }}
                transition={{ duration: 0.5, ease: "easeOut" }}
              />
            </div>
            <div className="quiz-card">
              <div className="question-header">
                <span className="topic-badge">
                  <TopicIcon topic={question.topic} />
                  {question.topic}
                </span>
                <span className="difficulty-badge">
                  <DifficultyIcon level={question.difficulty} />
                  Difficulty: {question.difficulty}
                </span>
              </div>
              <h2 className="question-text">{question.questionText}</h2>
              {renderQuestion()}
            </div>

            <div className="feedback-container">
              {isCorrect === true && (
                <motion.span initial={{scale: 0.5, opacity: 0}} animate={{scale: 1, opacity: 1}} className="correct">
                  <CheckCircle2 /> {feedback}
                </motion.span>
              )}
              {isCorrect === false && (
                <motion.span initial={{scale: 0.5, opacity: 0}} animate={{scale: 1, opacity: 1}} className="incorrect">
                  <XCircle /> {feedback}
                </motion.span>
              )}
            </div>
            
            <div className="controls-container">
              <button className="secondary-button" onClick={handleShowStats}>
                End Quiz
              </button>
            </div>
          </motion.div>
        )}

        {/* --- RESULTS SCREEN --- */}
        {quizState === 'results' && (
          <motion.div
            key="results"
            initial={{ opacity: 0, scale: 0.9 }}
            animate={{ opacity: 1, scale: 1 }}
          >
            <h2>Quiz Complete!</h2>
            <p className="final-score">Your final score: {score} / {numQuestions}</p>
            {renderStats()}
            <button className="primary-button" onClick={() => setQuizState('settings')}>
              Play Again
            </button>
          </motion.div>
        )}

      </AnimatePresence>
    </div>
  );
}