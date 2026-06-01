#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "tinyxml2.h"
#include "tree.hpp"
#include "procesar_libros.hpp"

using namespace tinyxml2;

void ProcesarLibros::addCampo(Tree& tree, Tree::Node* parent, const std::string& tag, const std::string& value) {
    Tree::Node* tagNode = tree.insertChild(parent, tag);
    tree.insertChild(tagNode, value);
}

// Algunos libros poseen campos vacios que devuelven nullptr, en ese caso retornamos un string vacio.
std::string ProcesarLibros::safeText(XMLElement* element) { 
    const char* text = element ? element->GetText() : nullptr;
    return text ? text : "";
}

void ProcesarLibros::procesarSimilarBooks(XMLElement* similarBooks, Tree& tree, Tree::Node* parent) {
    Tree::Node* similarNode = tree.insertChild(parent, "similar_books");

    XMLElement* book = similarBooks->FirstChildElement("book");
    while (book) {
        Tree::Node* bookNode = tree.insertChild(similarNode, "book");
        addCampo(tree, bookNode, "title", safeText(book->FirstChildElement("title")));
        addCampo(tree, bookNode, "isbn", safeText(book->FirstChildElement("isbn")));
        addCampo(tree, bookNode, "publication_year", safeText(book->FirstChildElement("publication_year")));
        book = book->NextSiblingElement("book");
    }
}

void ProcesarLibros::procesarLibro(XMLElement* book, Tree& tree, Tree::Node* parent, int index) {

    Tree::Node* bookNode = tree.insertChild(parent, "book");

    addCampo(tree, bookNode, "id", safeText(book->FirstChildElement("id")));
    addCampo(tree, bookNode, "title", safeText(book->FirstChildElement("title")));
    addCampo(tree, bookNode, "isbn", safeText(book->FirstChildElement("isbn")));
    addCampo(tree, bookNode, "publication_year", safeText(book->FirstChildElement("publication_year")));
    addCampo(tree, bookNode, "language_code", safeText(book->FirstChildElement("language_code")));
    addCampo(tree, bookNode, "description", safeText(book->FirstChildElement("description")));
    addCampo(tree, bookNode, "average_rating", safeText(book->FirstChildElement("average_rating")));
    addCampo(tree, bookNode, "num_pages", safeText(book->FirstChildElement("num_pages")));

    procesarSimilarBooks(book->FirstChildElement("similar_books"), tree, bookNode);
}

Tree ProcesarLibros::construirArbol() {
    const std::string dir = "books_xml";
    Tree tree(10000);
    Tree::Node* rootNode = tree.insertRoot("GoodreadsResponse");
    Tree::Node* booksNode = tree.insertChild(rootNode, "books");

    ProcesarLibros procesador;

    int index = 1;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        XMLDocument doc;
        XMLError eResult = doc.LoadFile(entry.path().string().c_str());
        XMLElement* root = doc.FirstChildElement("GoodreadsResponse");
        XMLElement* book = root->FirstChildElement("book");
        procesador.procesarLibro(book, tree, booksNode, index);
        index++;
    }

    return tree;
}

