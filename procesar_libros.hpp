#include <string>
#include "tinyxml2.h"
#include "tree.hpp"

class ProcesarLibros {
public:
    void procesarLibro(tinyxml2::XMLElement* book, Tree& tree, Tree::Node* parent, int index);
    static Tree construirArbol();

private:
    static std::string safeText(tinyxml2::XMLElement* element);
    static void addCampo(Tree& tree, Tree::Node* parent, const std::string& tag, const std::string& value);
    static void procesarSimilarBooks(tinyxml2::XMLElement* similarBooks, Tree& tree, Tree::Node* parent);
};

