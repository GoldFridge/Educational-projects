#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <memory>
#include <map>



std::random_device rd; // Obtain a random number from hardware
std::mt19937 engine(rd()); // Seed the generator

class Mine {
    const double MIN_FACTOR = 0.7; // minimum factor for gold extraction
    const double MAX_FACTOR = 1.3; // maximum factor for gold extraction
    std::shared_ptr<std::uniform_real_distribution<>> goldDistribution;

    int ordinal_number; // mine number
    double trueValue; // true expected value of the mine
    bool underThreat{}; // whether the mine is captured by rebels
    int daysWorked{}; // number of days the mine has been exploited
    double totalExtractedGold{}; // total gold extracted from the mine
    double captureDay = -1; // day when the mine was captured
public:

    explicit Mine(int number, double trueValue) : trueValue(trueValue), ordinal_number(number) {
        goldDistribution = std::make_shared<std::uniform_real_distribution<>>(MIN_FACTOR, MAX_FACTOR);
    }

    double extractGold() {
        daysWorked++;
        // get random gold value according to the distribution
        double extracted_today = trueValue * (*goldDistribution)(engine);
        totalExtractedGold += extracted_today;
        return extracted_today;
    }

    [[nodiscard]] bool isUnderThreat() const {
        return underThreat;
    }

    [[nodiscard]] int getDaysWorked() const {
        return daysWorked;
    }

    [[nodiscard]] double getTotalExtractedGold() const {
        return totalExtractedGold;
    }

    [[nodiscard]] double getCaptureDay() const {
        return captureDay;
    }

    [[nodiscard]] int getOrdinalNumber() const {
        return ordinal_number;
    }

    void capture(int day) {
        underThreat = true;
        captureDay = day;
    }

};

class Rebels {

    const int ATTACK_PERIOD = 10; // rebels attack every 10 days
    const double ATTACK_SUCCESS_PROBABILITY = 0.05; // probability of successful attack on a mine
    std::unique_ptr<std::uniform_real_distribution<>> attackDistribution; // distribution for attack probability


public:
    Rebels() {
        // initialize the distribution for attack probability
        attackDistribution = std::make_unique<std::uniform_real_distribution<>>(0.0, 1.0);
    }

    void move(int day, std::vector<std::shared_ptr<Mine>> &mines) {
        //every tenth day, rebels attempt to attack mines
        if (day % ATTACK_PERIOD != 0) {
            return;
        }
        for (auto &mine: mines) {
            // if the mine is already under threat, skip it
            if (mine->isUnderThreat() || (*attackDistribution)(engine) > ATTACK_SUCCESS_PROBABILITY)
                continue;
            mine->capture(day);
        }
    }
};


class SupremeLeader {
public:
    double targetEarnings; // amount of miney to amass by the end of the year
    double totalEarned{}; // total amount of money earned by exploiting mines

    std::vector<std::shared_ptr<Mine>> mines;
    std::map<int, double> mineExpectedValues;


    SupremeLeader(int numMines, double targetEarnings) : targetEarnings(targetEarnings) {
        // initialize the mines with random true values
        const int MIN_GOLD = 1000;
        const int MAX_GOLD = 4000;
        std::uniform_int_distribution<> distr(MIN_GOLD, MAX_GOLD);
        for (int i = 0; i < numMines; i++) {
            double randomTrueValue = distr(engine);
            mines.emplace_back(std::make_shared<Mine>(i, randomTrueValue));
        }
    }

    std::shared_ptr<Mine> selectMine(int day) {
        double explorationFactor = 2740; 
        std::shared_ptr<Mine> bestMine;
        double bestUCB = -1e9;
        for (const auto& mine : mines) {
            if (mine->isUnderThreat()) continue;
            double exploitationValue = mine->getTotalExtractedGold() / (mine->getDaysWorked() + 1);
            double explorationValue = sqrt((2 * log(day + 1)) / (mine->getDaysWorked() + 1));
            double ucb = exploitationValue + explorationFactor * explorationValue;
            if (ucb > bestUCB) {
                bestUCB = ucb;
                bestMine = mine;
            }
        }
      //  std::cout << bestUCB << std::endl;
        return bestMine;
    }

    void exploitMines(int day) {
        std::shared_ptr<Mine> mineToExploit = selectMine(day);
        if (mineToExploit) {
            double extractedGold = mineToExploit->extractGold();
            totalEarned += extractedGold;
            std::cout << "Mine " << mineToExploit->getOrdinalNumber() + 1 
                    << " extracted $" << extractedGold << std::endl;
        }
    }

    void printStatistics() const {
        std::cout << std::fixed << "Total earnings: $" << std::setprecision(0) << totalEarned << std::endl;
        std::cout << "Mine contributions:" << std::endl;
        for (const auto& mine: mines) {
            std::cout << "Mine " << mine->getOrdinalNumber() + 1 << " earned $" << std::setprecision(0)
                      << mine->getTotalExtractedGold() << " and worked for " << mine->getDaysWorked() << " days";
            if (mine->isUnderThreat()) {
                std::cout << ", but was attacked on day " << mine->getCaptureDay();
            }
            std::cout << std::endl;
        }
    }


    void playGame() {
        Rebels rebels;

        for (int day = 0; day < 365; day++) {
            exploitMines(day);
            rebels.move(day, mines);
        }

        if (totalEarned > targetEarnings) {
            std::cout << "Bravo! You've amassed sufficient wealth for a wonderful retirement." << std::endl;
        } else {
            std::cout << "You lost. No more luxury life for you." << std::endl;
        }

        printStatistics();
    }

};

int main() {
    int numMines = 10;
    double targetEarnings = 1000000;

    SupremeLeader leader(numMines, targetEarnings);
    leader.playGame();

    return 0;
}
