#ifndef PROMPTMANAGER_H
#define PROMPTMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <optional>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class PromptManager {
private:
    std::map<std::string, std::string> prompts;
    std::string databaseFile;

    // Singleton pattern
    static PromptManager* instance;
    PromptManager(const std::string& dbFile = "prompts.json");

public:
    // Singleton access
    static PromptManager* getInstance(const std::string& dbFile = "prompts.json");

    // Fonctionnalités principales
    std::optional<std::string> searchPrompt(const std::string& prompt);
    bool addPrompt(const std::string& prompt, const std::string& response);
    bool deletePrompt(const std::string& prompt);
    std::vector<std::string> listAllPrompts();

    // Persistence
    bool loadFromFile();
    bool saveToFile();


    // enregistrer les prompts et leurs reponses dans une base de données (not: ODB et SOC)
    bool LoadPromptToSql();



    // Utilitaires
    void displayStats();
    std::string toLowerCase(const std::string& str);
    double calculateSimilarity(const std::string& str1, const std::string& str2);

    // Recherche fuzzy
    std::vector<std::pair<std::string, double>> findSimilarPrompts(const std::string& prompt, double threshold = 0.6);

    ~PromptManager();
};

#endif // PROMPTMANAGER_H