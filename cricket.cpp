#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

// Forward declaration of classes
class Person;
class Player;
class Umpire;
class Batsman;
class Bowler;
class AllRounder;
class Team;
class Match;

// Abstract base class Person
class Person {
public:
    virtual string getName() const = 0;
    virtual void showProfile() const = 0;
};

// Umpire class
class Umpire : public Person {
private:
    string name;

public:
    Umpire(string name) : name(name) {}

    virtual string getName() const override { return name; }

    virtual void showProfile() const override {
        cout << "Umpire: " << name << endl;
    }
};

// Abstract base class Player
class Player : public Person {
protected:
    string name;
    int age;

public:
    Player(string name, int age) : name(name), age(age) {}

    virtual string getName() const override { return name; }
    int getAge() const { return age; }

    virtual void showProfile() const override = 0;
};

// Batsman class
class Batsman : public Player {
public:
    Batsman(string name, int age) : Player(name, age) {}

    virtual void showProfile() const override {
        cout << "Batsman: " << name << ", Age: " << age << endl;
    }
};

// Bowler class
class Bowler : public Player {
public:
    Bowler(string name, int age) : Player(name, age) {}

    virtual void showProfile() const override {
        cout << "Bowler: " << name << ", Age: " << age << endl;
    }
};

// AllRounder class
class AllRounder : public Player {
public:
    AllRounder(string name, int age) : Player(name, age) {}

    virtual void showProfile() const override {
        cout << "All-Rounder: " << name << ", Age: " << age << endl;
    }
};

// Team class
class Team {
private:
    string name;
    vector<Player*> players;

public:
    Team(string name) : name(name) {}

    void addPlayer(Player* player) {
        players.push_back(player);
    }

    void showTeam() const {
        cout << "Team: " << name << endl;
        cout << "Players: " << endl;
        for (const auto& player : players) {
            player->showProfile();
        }
        cout << endl;
    }

    string getName() const {
        return name;
    }
};

// Abstract base class Match
class Match {
protected:
    Team* team1;
    Team* team2;

public:
    Match(Team* team1, Team* team2) : team1(team1), team2(team2) {}

    virtual void start() = 0;
};

// Test match class
class Test : public Match {
public:
    Test(Team* team1, Team* team2) : Match(team1, team2) {}

    virtual void start() override {
        cout << "Test Match Started!" << endl;
        cout << team1->getName() << " vs. " << team2->getName() << endl;
        cout << "Playing conditions: 5 days match" << endl;
        // ... Add more specific Test match functionality
    }
};

// ODI match class
class ODI : public Match {
public:
    ODI(Team* team1, Team* team2) : Match(team1, team2) {}

    virtual void start() override {
        cout << "ODI Match Started!" << endl;
        cout << team1->getName() << " vs. " << team2->getName() << endl;
        cout << "Playing conditions: 50 overs per side" << endl;
        // ... Add more specific ODI match functionality
    }
};

// T20 match class
class T20 : public Match {
public:
    T20(Team* team1, Team* team2) : Match(team1, team2) {}

    virtual void start() override {
        cout << "T20 Match Started!" << endl;
        cout << team1->getName() << " vs. " << team2->getName() << endl;
        cout << "Playing conditions: 20 overs per side" << endl;
        // ... Add more specific T20 match functionality
    }
};

// Singleton CricketMatch class
class CricketMatch {
private:
    static CricketMatch* instance;
    CricketMatch() {}

public:
    static CricketMatch* getInstance() {
        if (instance == nullptr) {
            instance = new CricketMatch;
        }
        return instance;
    }

    void play(Match* match) {
        if (!match) {
            throw invalid_argument("Invalid match");
        }
        match->start();
    }
};

CricketMatch* CricketMatch::instance = nullptr;

int main() {
    ifstream infile("input.txt");
    if (!infile) {
        cerr << "Unable to open file input.txt";
        return 1;
    }

    // Create teams
    string team1Name, team2Name;
    int numPlayers;
    infile >> team1Name >> numPlayers;
    Team* team1 = new Team(team1Name);
    for (int i = 0; i < numPlayers; ++i) {
        string type, name;
        int age;
        infile >> type >> name >> age;
        if (type == "Batsman") {
            team1->addPlayer(new Batsman(name, age));
        } else if (type == "Bowler") {
            team1->addPlayer(new Bowler(name, age));
        } else if (type == "AllRounder") {
            team1->addPlayer(new AllRounder(name, age));
        }
    }

    infile >> team2Name >> numPlayers;
    Team* team2 = new Team(team2Name);
    for (int i = 0; i < numPlayers; ++i) {
        string type, name;
        int age;
        infile >> type >> name >> age;
        if (type == "Batsman") {
            team2->addPlayer(new Batsman(name, age));
        } else if (type == "Bowler") {
            team2->addPlayer(new Bowler(name, age));
        } else if (type == "AllRounder") {
            team2->addPlayer(new AllRounder(name, age));
        }
    }

    // Create cricket match (Singleton instance)
    CricketMatch* cricketMatch = CricketMatch::getInstance();

    // Play matches
    string matchType;
    while (infile >> matchType) {
        Match* match = nullptr;
        if (matchType == "Test") {
            match = new Test(team1, team2);
        } else if (matchType == "ODI") {
            match = new ODI(team1, team2);
        } else if (matchType == "T20") {
            match = new T20(team1, team2);
        }
        if (match) {
            cricketMatch->play(match);
            delete match;
        }
    }

    // Clean up
    delete team1;
    delete team2;

    return 0;
}
