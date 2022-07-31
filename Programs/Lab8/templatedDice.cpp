// Games, Template Method example
// Seth Winkhart
// 3/20/2022

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>

using std::cout;
using std::endl;

// template for any game where players take
// turns to make moves
// and there is a winner
class Game
{
public:
    Game() : playersCount_(0), movesCount_(0), playerWon_(noWinner) {}

    // template method
    void playGame(const int playersCount = 0)
    {
        playersCount_ = playersCount;
        movesCount_ = 0;

        initializeGame();

        for (int i = 0; !endOfGame(); i = (i + 1) % playersCount_)
        {
            makeMove(i);
            if (i == playersCount_ - 1)
                ++movesCount_;
        }
        printWinner();
    }

    virtual ~Game() {}

protected:
    // primitive operations
    virtual void initializeGame() = 0;
    virtual void makeMove(int player) = 0;
    virtual void printWinner() = 0;
    virtual bool endOfGame() { return playerWon_ != noWinner; } // this is a hook
                                                                // returns true if winner is decided
    static const int noWinner = -1;

    int playersCount_;
    int movesCount_;
    int playerWon_;
};

// Monopoly - a concrete game implementing primitive
// operations for the template method
class Monopoly : public Game
{
public:
    // implementing concrete methods
    void initializeGame()
    {
        playersCount_ = rand() % numPlayers_ + 1; // initialize players
    }

    void makeMove(int player)
    {
        if (movesCount_ > minMoves_)
        {
            const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
            if (chance < movesCount_)
                playerWon_ = player;
        }
    }

    void printWinner()
    {
        cout << "Monopoly, player " << playerWon_ << " won in "
             << movesCount_ << " moves." << endl;
    }

private:
    static const int numPlayers_ = 8; // max number of players
    static const int minMoves_ = 20;  // nobody wins before minMoves_
    static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing
// primitive operations
class Chess : public Game
{
public:
    void initializeGame()
    {
        playersCount_ = numPlayers_; // initalize players
        for (int i = 0; i < numPlayers_; ++i)
            experience_[i] = rand() % maxExperience_ + 1;
    }

    void makeMove(int player)
    {
        if (movesCount_ > minMoves_)
        {
            const int chance = (rand() % maxMoves_) / experience_[player];
            if (chance < movesCount_)
                playerWon_ = player;
        }
    }

    void printWinner()
    {
        cout << "Chess, player " << playerWon_
             << " with experience " << experience_[playerWon_]
             << " won in " << movesCount_ << " moves over"
             << " player with experience " << experience_[playerWon_ == 0 ? 1 : 0]
             << endl;
    }

private:
    static const int numPlayers_ = 2;
    static const int minMoves_ = 2;      // nobody wins before minMoves_
    static const int maxMoves_ = 100;    // somebody wins before maxMoves_
    static const int maxExperience_ = 5; // player's experience
                                         // the higher, the greater probability of winning
    int experience_[numPlayers_];
};

// primitive operations for Dice
class Dice : public Game
{
public:
    void initializeGame()
    {
        playersCount_ = numPlayer_;
    }

    void makeMove(int player)
    {
        if (movesCount_ < maxMoves_)
        {
            // computers turn
            if (cpuNextMove_ == true && player == 0)
            {
                // computers roll
                int cpuTotal = 0;
                cout << "Computer rolled: ";
                for (int i = 0; i < 5; ++i)
                {
                    int roll = rand() % 7;
                    cout << roll << " ";
                    cpuTotal += roll;
                }
                cout << "= " << cpuTotal;
                if (cpuTotal > cpuHighestScore_)
                {
                    cpuHighestScore_ = cpuTotal;
                }
                cout << ", computers highest score = " << cpuHighestScore_ << endl;

                // find if computer wants to roll again
                if (movesCount_ < maxMoves_ - 1)
                {
                    int again = rand() % 11;
                    if (again >= 5)
                    {
                        cpuNextMove_ = false;
                    }
                }
            }
            else if (cpuNextMove_ == false && player == 0)
            {
                cout << "Computer rolled: passed, computers highest score = " << cpuHighestScore_ << endl;
                cpuNextMove_ = true;

                // find if computer wants to roll again
                if (movesCount_ < maxMoves_ - 1)
                {
                    int again = rand() % 11;
                    if (again >= 5)
                    {
                        cpuNextMove_ = false;
                    }
                }
            }

            // humans turn
            if (humanNextMove_ == true && player == 1)
            {
                // human roll
                int humanTotal = 0;
                cout << "You rolled: ";
                for (int i = 0; i < 5; ++i)
                {
                    int roll = rand() % 7;
                    cout << roll << " ";
                    humanTotal += roll;
                }
                cout << "= " << humanTotal;
                if (humanTotal > humanHighestScore_)
                {
                    humanHighestScore_ = humanTotal;
                }
                cout << ", your highest score = " << humanHighestScore_ << endl;

                // find if human player wants to roll again
                if (movesCount_ < maxMoves_ - 1)
                {
                    char rollAgain;
                    cout << "Roll again? [y/n] ";
                    std::cin >> rollAgain;
                    if (rollAgain == 'n')
                    {
                        humanNextMove_ = false;
                    }
                }
            }
            else if (humanNextMove_ == false && player == 1)
            {
                cout << "You rolled: passed, your highest score = " << humanHighestScore_ << endl;
                humanNextMove_ = true;

                // find if human player wants to roll again
                if (movesCount_ < maxMoves_ - 1)
                {
                    char rollAgain;
                    cout << "Roll again? [y/n] ";
                    std::cin >> rollAgain;
                    if (rollAgain == 'n')
                    {
                        humanNextMove_ = false;
                    }
                }
            }
        }
        else
        {
            if (cpuHighestScore_ > humanHighestScore_)
            {
                playerWon_ = 1;
            }

            if (humanHighestScore_ > cpuHighestScore_)
            {
                playerWon_ = 2;
            }

            if (humanHighestScore_ == cpuHighestScore_)
            {
                playerWon_ = 3;
            }
        }
    }

    void printWinner()
    {
        if (playerWon_ == 1)
        {
            cout << "You Lost ):" << endl;
        }

        if (playerWon_ == 2)
        {
            cout << "You Win (:" << endl;
        }

        if (playerWon_ == 3)
        {
            cout << "Tie" << endl;
        }
    }

private:
    static const int numPlayer_ = 2; // human and computer
    static const int minMoves_ = 1;
    static const int maxMoves_ = 3;
    int cpuHighestScore_ = 0;
    int humanHighestScore_ = 0;
    bool cpuNextMove_ = true;
    bool humanNextMove_ = true;
};

int main()
{
    srand(time(nullptr));

    Game *gp = nullptr;

    /*
    // play chess 10 times
    for (int i = 0; i < 10; ++i)
    {
        gp = new Chess;
        gp->playGame();
        delete gp;
    }

    // play monopoly 8 times
    for (int i = 0; i < 8; ++i)
    {
        gp = new Monopoly;
        gp->playGame();
        delete gp;
    }
    */

    // dice game here
    gp = new Dice;
    gp->playGame();
    delete gp;
}