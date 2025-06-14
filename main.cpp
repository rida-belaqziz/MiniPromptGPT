#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include "PromptManager.h"

class MiniPromptGPT {
private:
    PromptManager* manager;
    bool running;

public:
    MiniPromptGPT() : running(true) {
        manager = PromptManager::getInstance();
        displayWelcome();
    }

    void displayWelcome() {
        std::cout << "\n Bienvenue dans MiniPromptGPT !\n";
        std::cout << "=====================================\n";
        std::cout << "Une IA locale simple en C++\n\n";
        manager->displayStats();
        displayMenu();
    }

    void displayMenu() {
        std::cout << "\n Commandes disponibles :\n";
        std::cout << "1. Poser une question\n";
        std::cout << "2. Ajouter un nouveau prompt\n";
        std::cout << "3. Supprimer un prompt\n";
        std::cout << "4. Lister tous les prompts\n";
        std::cout << "5. Recherche similaire\n";
        std::cout << "6. Statistiques\n";
        std::cout << "0. Quitter\n";
        std::cout << "=============================\n";
    }

    void run() {
        while (running) {
            std::cout << "\n> Que voulez-vous faire ? (tapez 'menu' pour les options) : ";
            std::string input;
            std::getline(std::cin, input);

            if (input.empty()) continue;

            // Nettoyer l'input
            std::transform(input.begin(), input.end(), input.begin(), ::tolower);

            if (input == "menu" || input == "m") {
                displayMenu();
            } else if (input == "0" || input == "quit" || input == "exit" || input == "q") {
                handleExit();
            } else if (input == "1" || input == "question" || input == "ask") {
                handleQuestion();
            } else if (input == "2" || input == "add" || input == "ajouter") {
                handleAddPrompt();
            } else if (input == "3" || input == "delete" || input == "supprimer") {
                handleDeletePrompt();
            } else if (input == "4" || input == "list" || input == "lister") {
                handleListPrompts();
            } else if (input == "5" || input == "similar" || input == "similaire") {
                handleSimilarSearch();
            } else if (input == "6" || input == "stats" || input == "statistiques") {
                manager->displayStats();
            } else {
                // Traiter comme une question directe
                processDirectQuestion(input);
            }
        }
    }

private:
    void handleQuestion() {
        std::cout << "\n Entrez votre question : ";
        std::string question;
        std::getline(std::cin, question);

        if (question.empty()) {
            std::cout << " Question vide !\n";
            return;
        }

        processDirectQuestion(question);
    }

    void processDirectQuestion(const std::string& question) {
        auto response = manager->searchPrompt(question);

        if (response.has_value()) {
            std::cout << "\n Réponse : " << response.value() << "\n";
        } else {
            std::cout << "\n Prompt inconnu...\n";

            // Rechercher des prompts similaires
            auto similar = manager->findSimilarPrompts(question, 0.3);
            if (!similar.empty()) {
                std::cout << " Prompts similaires trouvés :\n";
                for (size_t i = 0; i < std::min(size_t(3), similar.size()); i++) {
                    std::cout << "  - " << similar[i].first
                              << " (similarité: " << (int)(similar[i].second * 100) << "%)\n";
                }
            }

            std::cout << "\n Souhaitez-vous ajouter une réponse pour ce prompt ? (y/n) : ";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "y" || choice == "yes" || choice == "oui" || choice == "o") {
                addNewPromptResponse(question);
            }
        }
    }

    void addNewPromptResponse(const std::string& prompt) {
        std::cout << " Entrez la réponse : ";
        std::string response;
        std::getline(std::cin, response);

        if (response.empty()) {
            std::cout << " Réponse vide, annulation.\n";
            return;
        }

        if (manager->addPrompt(prompt, response)) {
            std::cout << " Prompt ajouté avec succès !\n";
        } else {
            std::cout << " Erreur lors de l'ajout du prompt.\n";
        }
    }

    void handleAddPrompt() {
        std::cout << "\n Ajouter un nouveau prompt\n";
        std::cout << "Entrez le prompt : ";
        std::string prompt;
        std::getline(std::cin, prompt);

        if (prompt.empty()) {
            std::cout << " Prompt vide !\n";
            return;
        }

        // Vérifier si le prompt existe déjà
        auto existing = manager->searchPrompt(prompt);
        if (existing.has_value()) {
            std::cout << "⚠  Ce prompt existe déjà avec la réponse : " << existing.value() << "\n";
            std::cout << "Voulez-vous le remplacer ? (y/n) : ";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice != "y" && choice != "yes" && choice != "oui" && choice != "o") {
                return;
            }
        }

        addNewPromptResponse(prompt);
    }

    void handleDeletePrompt() {
        std::cout << "\n  Supprimer un prompt\n";
        std::cout << "Entrez le prompt à supprimer : ";
        std::string prompt;
        std::getline(std::cin, prompt);

        if (prompt.empty()) {
            std::cout << " Prompt vide !\n";
            return;
        }

        if (manager->deletePrompt(prompt)) {
            std::cout << "Prompt supprimé avec succès !\n";
        } else {
            std::cout << " Prompt non trouvé.\n";
        }
    }

    void handleListPrompts() {
        auto prompts = manager->listAllPrompts();

        if (prompts.empty()) {
            std::cout << "\n Aucun prompt enregistré.\n";
            return;
        }

        std::cout << "\n Liste de tous les prompts (" << prompts.size() << ") :\n";
        std::cout << "=====================================\n";

        for (size_t i = 0; i < prompts.size(); i++) {
            std::cout << (i + 1) << ". " << prompts[i] << "\n";
        }
    }

    void handleSimilarSearch() {
        std::cout << "\n Recherche par similarité\n";
        std::cout << "Entrez votre recherche : ";
        std::string search;
        std::getline(std::cin, search);

        if (search.empty()) {
            std::cout << " Recherche vide !\n";
            return;
        }

        auto similar = manager->findSimilarPrompts(search, 0.2);

        if (similar.empty()) {
            std::cout << " Aucun prompt similaire trouvé.\n";
        } else {
            std::cout << "\n Prompts similaires :\n";
            for (const auto& pair : similar) {
                std::cout << "  • " << pair.first
                          << " (similarité: " << (int)(pair.second * 100) << "%)\n";
            }
        }
    }

    void handleExit() {
        std::cout << "\n Merci d'avoir utilisé MiniPromptGPT !\n";
        std::cout << "Sauvegarde en cours...\n";
        running = false;
    }
};

int main() {
    try {
        MiniPromptGPT app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << " Erreur critique : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}