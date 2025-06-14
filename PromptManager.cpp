#include "PromptManager.h"
#include <algorithm>
#include <sstream>
#include <cmath>

PromptManager* PromptManager::instance = nullptr;

PromptManager::PromptManager(const std::string& dbFile) : databaseFile(dbFile) {
    loadFromFile();
}

PromptManager* PromptManager::getInstance(const std::string& dbFile) {
    if (instance == nullptr) {
        instance = new PromptManager(dbFile);
    }
    return instance;
}

std::optional<std::string> PromptManager::searchPrompt(const std::string& prompt) {
    std::string lowerPrompt = toLowerCase(prompt);

    // Recherche exacte d'abord
    auto it = prompts.find(lowerPrompt);
    if (it != prompts.end()) {
        return it->second;
    }

    // Recherche par mots-clés
    for (const auto& pair : prompts) {
        if (pair.first.find(lowerPrompt) != std::string::npos ||
            lowerPrompt.find(pair.first) != std::string::npos) {
            return pair.second;
        }
    }

    return std::nullopt;
}

bool PromptManager::addPrompt(const std::string& prompt, const std::string& response) {
    if (prompt.empty() || response.empty()) {
        return false;
    }

    std::string lowerPrompt = toLowerCase(prompt);
    prompts[lowerPrompt] = response;
    return saveToFile();
}

bool PromptManager::deletePrompt(const std::string& prompt) {
    std::string lowerPrompt = toLowerCase(prompt);
    auto it = prompts.find(lowerPrompt);

    if (it != prompts.end()) {
        prompts.erase(it);
        return saveToFile();
    }
    return false;
}

std::vector<std::string> PromptManager::listAllPrompts() {
    std::vector<std::string> result;
    for (const auto& pair : prompts) {
        result.push_back(pair.first);
    }
    return result;
}

bool PromptManager::loadFromFile() {
    std::ifstream file(databaseFile);
    if (!file.is_open()) {
        // Créer un fichier vide si il n'existe pas
        json emptyJson = json::object();
        std::ofstream newFile(databaseFile);
        newFile << emptyJson.dump(4);
        newFile.close();
        return true;
    }

    try {
        json jsonData;
        file >> jsonData;
        file.close();

        prompts.clear();
        for (auto& element : jsonData.items()) {
            prompts[element.key()] = element.value();
        }

        std::cout << " Base de données chargée : " << prompts.size() << " prompts\n";
        return true;
    } catch (const std::exception& e) {
        std::cerr << " Erreur lors du chargement : " << e.what() << std::endl;
        return false;
    }
}

bool PromptManager::saveToFile() {
    try {
        json jsonData;
        for (const auto& pair : prompts) {
            jsonData[pair.first] = pair.second;
        }

        std::ofstream file(databaseFile);
        file << jsonData.dump(4);
        file.close();

        return true;
    } catch (const std::exception& e) {
        std::cerr << " Erreur lors de la sauvegarde : " << e.what() << std::endl;
        return false;
    }
}

bool PromptManager::LoadPromptToSql() {

}




std::string PromptManager::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

double PromptManager::calculateSimilarity(const std::string& str1, const std::string& str2) {
    // Algorithme de similarité simple basé sur les mots communs
    std::istringstream iss1(str1), iss2(str2);
    std::vector<std::string> words1, words2;
    std::string word;

    while (iss1 >> word) words1.push_back(toLowerCase(word));
    while (iss2 >> word) words2.push_back(toLowerCase(word));

    int commonWords = 0;
    for (const auto& w1 : words1) {
        for (const auto& w2 : words2) {
            if (w1 == w2) {
                commonWords++;
                break;
            }
        }
    }

    return (double)commonWords / std::max(words1.size(), words2.size());
}

std::vector<std::pair<std::string, double>> PromptManager::findSimilarPrompts(const std::string& prompt, double threshold) {
    std::vector<std::pair<std::string, double>> similar;

    for (const auto& pair : prompts) {
        double similarity = calculateSimilarity(prompt, pair.first);
        if (similarity >= threshold) {
            similar.push_back({pair.first, similarity});
        }
    }

    // Trier par similarité décroissante
    std::sort(similar.begin(), similar.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    return similar;
}

void PromptManager::displayStats() {
    std::cout << "\n Statistiques de la base de données :\n";
    std::cout << "Total de prompts : " << prompts.size() << std::endl;

    if (!prompts.empty()) {
        int totalChars = 0;
        for (const auto& pair : prompts) {
            totalChars += pair.first.length() + pair.second.length();
        }
        std::cout << "Taille moyenne : " << totalChars / prompts.size() << " caractères" << std::endl;
    }
}

PromptManager::~PromptManager() {
    saveToFile();
}